/* Copyright 2023 The ChromiumOS Authors.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "deque_stream.h"

#include <mutex>

template<uint32_t buff_size>
::pw::StatusWithSize DequeReadWriter::DoRead(::pw::ByteSpan destination) {
  size_t count = 0;
  count = ring_buf_get(&buff_, destination, destination.size_bytes());
  auto status = (count == 0) ? ::pw::Status::OutOfRange() : ::pw::OkStatus();
  return ::pw::StatusWithSize(status, count);
}

template<uint32_t buff_size>
::pw::Status DequeReadWriter::DoWrite(::pw::ConstByteSpan data) {
  uint32_t ret = 0;

  do {
    if (ret > 0) {
      k_yield();
    }
    ret += ring_buf_put(&buff_, &data[ret], data.size_bytes() - ret);
  } while (ret < data.size_bytes());

  return ::pw::OkStatus();
}
