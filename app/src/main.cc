#define PW_LOG_MODULE_NAME "APP"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <pw_log/log.h>

int main(void) {
  while(1) {

    PW_LOG_DEBUG("Hello Pigweed DEBUG");
    PW_LOG_INFO("Hello Pigweed INFO");
    PW_LOG_WARN("Hellow Pigweed WARN");
    PW_LOG_ERROR("Hello Pigweed ERROR");

    LOG_ERR("Hello Zephyr ERR");
    LOG_WRN("Hello Zephyr WRN");
    LOG_INF("Hello Zephyr INF");
    LOG_DBG("Hello Zephyr DBG");

    k_sleep(K_MSEC(1000));
  }
  return 0;
}
