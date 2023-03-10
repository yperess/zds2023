#define PW_LOG_MODULE_NAME "APP"

#include <zephyr/logging/log.h>
#include <pw_log/log.h>

int main(void) {
  PW_LOG_INFO("Hello Pigweed logging");
  LOG_INF("Hello Zephyr logging");
  return 0;
}
