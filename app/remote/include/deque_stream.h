/* Copyright 2023 The ChromiumOS Authors.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <pw_stream/stream.h>
#include <zephyr/sys/ring_buffer.h>

template<uint32_t buffer_size>
class DequeueReadWriter : public ::pw::stream::NonSeekableReaderWriter {
 public:
  DequeueReadWriter() {
    ring_buf_init(&buff_, buffer_size, buffer_bytes_);
  }

 protected:
  ::pw::StatusWithSize DoRead(::pw::ByteSpan destination) override;
  ::pw::Status DoWrite(::pw::ConstByteSpan data) override;

 private:
  struct ring_buf buff_;
  uint32_t buffer_bytes_[buffer_size];
};
