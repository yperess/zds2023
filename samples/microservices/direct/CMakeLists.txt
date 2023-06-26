# Copyright (c) 2023 Google LLC
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)

zephyr_library_named(direct_demo)
zephyr_library_sources(
    include/DirectBenchmark.hh
    src/entrypoint.cc
)
zephyr_library_include_directories(include)
