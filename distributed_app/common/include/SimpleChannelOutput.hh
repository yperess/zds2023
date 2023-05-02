/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <pw_assert/assert.h>
#include <pw_bytes/span.h>
#include <pw_hdlc/rpc_packets.h>
#include <pw_rpc/channel.h>
#include <pw_span/span.h>
#include <pw_stream/stream.h>

#include "RingReaderWriter.hh"

class SimpleChannelOutput : public pw::rpc::ChannelOutput {
 public:
  constexpr SimpleChannelOutput(RingReaderWriter& writer) : pw::rpc::ChannelOutput(nullptr), writer_(writer) {}

  pw::Status Send(pw::ConstByteSpan output) override {
    uint32_t size = static_cast<uint32_t>(output.size());
//    printk("Writing %u bytes: ", size);
//    for (auto it = output.begin(); it != output.end(); it++) {
//      printk("%02x", *it);
//    }
//    printk("\n");
    writer_.BeginWrite();
    pw::stream::Writer& writer = writer_.as_writer();
    writer.Write(reinterpret_cast<const std::byte*>(&size), sizeof(size));
    writer.Write(output);
    writer_.EndWrite();
    return pw::OkStatus();
  }

 private:
//  pw::stream::Writer& writer_;
  RingReaderWriter& writer_;
};

class Frame {
 public:
  Frame(pw::ConstByteSpan data) : data_(data) {}
  constexpr uint64_t address() { return pw::hdlc::kDefaultRpcAddress; }
  constexpr pw::ConstByteSpan data() { return data_; }

 private:
  pw::ConstByteSpan data_;
};

class SimpleDecoder {
 public:
  constexpr SimpleDecoder(pw::ByteSpan buffer) : buffer_(buffer), current_frame_size_(0) {}
  SimpleDecoder(const SimpleDecoder&) = delete;
  SimpleDecoder& operator=(const SimpleDecoder&) = delete;

  pw::Result<Frame> Process(std::byte new_byte) {
    if (current_frame_size_ >= max_size()) {
      return pw::Status::Unavailable();
    }
    buffer_[current_frame_size_++] = new_byte;
    if (current_frame_size_ == current_frame_expected_size()) {
      return Frame(buffer_.subspan(sizeof(uint32_t), current_frame_size_ - sizeof(uint32_t)));
    }
    return pw::Status::Unavailable();
  }

  void Clear() { current_frame_size_ = 0; }

  size_t max_size() const { return buffer_.size(); }

 private:
  pw::ByteSpan buffer_;
  size_t current_frame_size_;

  inline uint32_t current_frame_expected_size() {
    if (current_frame_size_ < sizeof(uint32_t)) {
      return 0;
    }

    uint32_t expected_size = *reinterpret_cast<uint32_t*>(buffer_.data());
//    printk("Expected size is %u bytes\n", expected_size);
    return 4 + expected_size;
  }
};
