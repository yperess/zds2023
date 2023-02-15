# Copyright (c) 2023 Google LLC
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)
set(pw_third_party_nanopb_ADD_SUBDIRECTORY ON CACHE BOOL "" FORCE)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app)
enable_language(C CXX)

target_sources(app
  PRIVATE
    src/main.cc
)
