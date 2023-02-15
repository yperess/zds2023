/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, CONFIG_APP_LOG_LEVEL);

int main(void) {
  while (true) {
    LOG_DBG("Hello Zephyr DBG");
    LOG_INF("Hello Zephyr INF");
    LOG_WRN("Hello Zephyr WRN");
    LOG_ERR("Hello Zephyr ERR");

    k_sleep(K_MSEC(5000));
  }
  return 0;
}
