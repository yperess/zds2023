/*
 * Copyright (c) 2023 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */

#define PW_LOG_MODULE_NAME "APP"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <pw_log/log.h>
#include <pw_system/init.h>
#include <zephyr/logging/log_core.h>

namespace pw::system {

// This will run once after pw::system::Init() completes. This callback must
// return or it will block the work queue.
void UserAppInit() {
  Z_LOG_PRINTK(/*_is_raw=*/0, "UserAppInit Pigweed is fun\n");
  PW_LOG_INFO("Pigweed is fun!");
}

}  // namespace pw::system

int main(void) {
  int count = 0;
  pw::system::Init();

  while(true) {
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
