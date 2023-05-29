# Copyright (c) 2023 Google LLC
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)
set(pw_third_party_nanopb_ADD_SUBDIRECTORY ON CACHE BOOL "" FORCE)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app)
enable_language(C CXX)

# Define the proto library
include(${ZEPHYR_PIGWEED_MODULE_DIR}/pw_protobuf_compiler/proto.cmake)
pw_proto_library(
    rpc_demo.protos
    SOURCES proto/demo.proto
)

zephyr_link_libraries(
    rpc_demo.protos.nanopb
    rpc_demo.protos.nanopb_rpc
)

target_include_directories(app
  PRIVATE
    include
)

target_sources(app
  PRIVATE
    src/main.cc
)

# Add custom logic to generate database.bin
# The application expects it to be in the binary dir.
add_custom_target(database_binary
DEPENDS
  ${CMAKE_BINARY_DIR}/database.csv
)
add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/database.csv
  COMMAND
  ${PYTHON_EXECUTABLE}
  $ENV{PW_ROOT}/pw_tokenizer/py/pw_tokenizer/database.py
  create
  --force
  --type csv
  -d ${CMAKE_BINARY_DIR}/database.csv
  ${CMAKE_BINARY_DIR}/zephyr/zephyr_pre0.elf
  DEPENDS zephyr_pre0
)
add_dependencies(${logical_target_for_zephyr_elf} database_binary)

#add_subdirectory(service)
#add_subdirectory_ifdef(CONFIG_LOCALITY_LPC local)
#add_subdirectory_ifdef(CONFIG_LOCALITY_RPC remote)
