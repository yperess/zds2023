/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <pw_stream/stream.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/ring_buffer.h>

class RingReaderWriter : public pw::stream::NonSeekableReaderWriter {
 public:
  RingReaderWriter(pw::ByteSpan buffer, const char* name = nullptr) : name_(name) {
    k_mutex_init(&mutex_);
    k_condvar_init(&condvar_);
    ring_buf_init(&buff_, buffer.size_bytes(), reinterpret_cast<uint8_t*>(buffer.data()));
    if (name_ == nullptr) {
      name_ = kDefaultName;
    }
  };

  int BeginWrite() {
    return k_mutex_lock(&mutex_, K_FOREVER);
  }

  void EndWrite() {
    k_condvar_signal(&condvar_);
    k_mutex_unlock(&mutex_);
  }

 protected:
  pw::StatusWithSize DoRead(pw::ByteSpan destination) override {
    uint32_t bytes_read = 0;
    do {
      int rc = k_mutex_lock(&mutex_, K_FOREVER);
      if (rc != 0) {
        return pw::StatusWithSize(PW_STATUS_ABORTED, bytes_read);
      }
      bytes_read = ring_buf_get(&buff_, reinterpret_cast<uint8_t*>(destination.data()), destination.size_bytes());
      if (bytes_read == 0) {
        // Can't read anything, wait for data.
        k_condvar_wait(&condvar_, &mutex_, K_FOREVER);
        k_mutex_unlock(&mutex_);
      } else {
        // Read some data, notify any blocked writers there's room now.
        k_condvar_signal(&condvar_);
        k_mutex_unlock(&mutex_);
      }
    } while (bytes_read == 0);

    return pw::StatusWithSize(PW_STATUS_OK, bytes_read);
  }

  pw::Status DoWrite(pw::ConstByteSpan data) override {
    if (data.size_bytes() == 0) {
      return pw::OkStatus();
    }

    const std::byte* data_ptr = data.data();
    uint32_t bytes_remaining = data.size_bytes();
    uint8_t* output_ptr;
    uint32_t output_size;

    do {
      output_size = ring_buf_put_claim(&buff_, &output_ptr, bytes_remaining);

      if (output_size == 0) {
        printk("%s::DoWrite() failed to allocate space\n", name_);
        ring_buf_put_finish(&buff_, 0);
        k_condvar_signal(&condvar_);
        k_mutex_unlock(&mutex_);
        k_mutex_lock(&mutex_, K_FOREVER);
        continue;
      }
      memcpy(output_ptr, reinterpret_cast<const void*>(data_ptr), output_size);
      ring_buf_put_finish(&buff_, output_size);
      data_ptr += output_size;
      bytes_remaining -= output_size;
    } while (bytes_remaining > 0);
    return pw::OkStatus();
  }

 private:
  static constexpr const char* kDefaultName = "RingReaderWriter";
  const char* name_;
  struct ring_buf buff_ = {};
  struct k_mutex mutex_;
  struct k_condvar condvar_;
};
