/* Deep sleep wake up example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp32/ulp.h"
#include "driver/touch_pad.h"
#include "driver/adc.h"
#include "driver/rtc_io.h"
#include "soc/sens_periph.h"
#include "soc/rtc.h"
#include "deepsleep.h"
static RTC_DATA_ATTR struct timeval sleep_enter_time;

esp_sleep_source_t deepsleep_get_wakeup(void)
{
    esp_sleep_source_t ret = ESP_SLEEP_WAKEUP_UNDEFINED;
    struct timeval now;
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;
    switch (esp_sleep_get_wakeup_cause()) {
        case ESP_SLEEP_WAKEUP_EXT1: {
            uint64_t wakeup_pin_mask = esp_sleep_get_ext1_wakeup_status();
            if (wakeup_pin_mask != 0) {
                int pin = __builtin_ffsll(wakeup_pin_mask) - 1;
                printf("Wake up from GPIO %d\n", pin);
            } else {
                printf("Wake up from GPIO\n");
            }
            ret = ESP_SLEEP_WAKEUP_EXT1;
            break;
        }
        case ESP_SLEEP_WAKEUP_TIMER: {
            printf("Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
            ret = ESP_SLEEP_WAKEUP_TIMER;
            break;
        }
        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            printf("Not a deep sleep reset\n");
            ret = ESP_SLEEP_WAKEUP_UNDEFINED;
    }

    return ret;
}

void deepsleep_start(int sleep_period, int gpio_num)
{
    const int wakeup_time_sec = sleep_period;
    
    if (sleep_period != 0) {
        // printf("Enabling timer wakeup, %ds\n", wakeup_time_sec);
        esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);
    }

// #ifdef CONFIG_EXAMPLE_EXT1_WAKEUP
    const int ext_wakeup_pin_1 = gpio_num;
    const uint64_t ext_wakeup_pin_1_mask = 1ULL << ext_wakeup_pin_1;

    // printf("Enabling EXT1 wakeup on pins GPIO%d\n", ext_wakeup_pin_1);
    esp_sleep_enable_ext1_wakeup(ext_wakeup_pin_1_mask , ESP_EXT1_WAKEUP_ANY_HIGH);
// #endif // CONFIG_EXAMPLE_EXT1_WAKEUP

#if CONFIG_IDF_TARGET_ESP32
    // Isolate GPIO12 pin from external circuits. This is needed for modules
    // which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
    // to minimize current consumption.
    rtc_gpio_isolate(GPIO_NUM_12);
#endif

    // printf("Entering deep sleep\n");
    gettimeofday(&sleep_enter_time, NULL);
    esp_deep_sleep_start();
}
