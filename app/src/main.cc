/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */

#define PW_LOG_MODULE_NAME "APP"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <pw_log/log.h>

int main(void) {
  while (true) {
    PW_LOG_DEBUG("Hello Pigweed DEBUG");
    PW_LOG_INFO("Hello Pigweed INFO");
    PW_LOG_WARN("Hello Pigweed WARN");
    PW_LOG_ERROR("Hello Pigweed ERROR");

    LOG_DBG("Hello Zephyr DBG");
    LOG_INF("Hello Zephyr INF");
    LOG_WRN("Hello Zephyr WRN");
    LOG_ERR("Hello Zephyr ERR");

    k_sleep(K_MSEC(5000));
  }
  return 0;
}
