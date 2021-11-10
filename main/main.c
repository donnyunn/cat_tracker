/*
   This code is written by Dulab Ltd. but it is in the Public Domain 
   (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/****************************************************************************
*
* This file is a main code for the cat tracker module.
*
****************************************************************************/

#include "main.h"

#define LED_IO GPIO_NUM_19

#define LED_ON() gpio_set_level(LED_IO, 0)
#define LED_OFF() gpio_set_level(LED_IO, 1)

#define BTN_IO GPIO_NUM_27

RTC_DATA_ATTR int cntBoot;
RTC_DATA_ATTR bool power;

void main_init(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL<<LED_IO);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
    LED_OFF();

    io_conf.pin_bit_mask = (1ULL<<BTN_IO);
    io_conf.mode = GPIO_MODE_INPUT | GPIO_MODE_DEF_OD;
    io_conf.pull_down_en = 1;
    gpio_config(&io_conf);
}

void main_deinit(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_DISABLE | GPIO_MODE_DEF_OD;
    io_conf.pin_bit_mask = (1ULL<<LED_IO) | (1ULL<<BTN_IO);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

void led_init(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL<<LED_IO);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

void led_deinit(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL<<LED_IO);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

void deinitialize(void)
{
    mpu6050_sleep();
    mpu6050_deinit();
    led_deinit();
}

void app_main(void)
{
    mpu6050_t imu;
    packet_t packet;
    int retry = 0;

    main_init();

    switch (deepsleep_get_wakeup()) {
        case ESP_SLEEP_WAKEUP_TIMER:
            LED_ON();


            /* gatt server and beacon */
            gatts_init();

            /* packet manager */
            packet_init(&packet);

            if ((cntBoot++ % 3) == 0) {
                /* mpu6050 IMU Sensor */
                mpu6050_init();

                /* record IMU data */
                mpu6050_read(&imu);
                packet_push(imu.data);

                deinitialize();
            }

            LED_OFF();
            
            /* GATT SERVER Advertisement */
            vTaskDelay(750 / portTICK_PERIOD_MS);

            /* Stay awake while connected */
            while (gatts_isConnected()) {
                LED_ON();
                vTaskDelay(50 / portTICK_PERIOD_MS);
                LED_OFF();
                vTaskDelay(50 / portTICK_PERIOD_MS);
            }
        break;
        case ESP_SLEEP_WAKEUP_EXT1:
            vTaskDelay(1500 / portTICK_PERIOD_MS);
            if (gpio_get_level(BTN_IO) != 0) {
                LED_ON();
                if (power) {
                    power = false;

                    vTaskDelay(100 / portTICK_PERIOD_MS);
                    LED_OFF();
                    vTaskDelay(100 / portTICK_PERIOD_MS);
                    LED_ON();
                    vTaskDelay(100 / portTICK_PERIOD_MS);
                } else {
                    power = true;

                    vTaskDelay(500 / portTICK_PERIOD_MS);
                    LED_OFF();
                    vTaskDelay(500 / portTICK_PERIOD_MS);
                    LED_ON();
                    vTaskDelay(2000 / portTICK_PERIOD_MS);
                }
                
                LED_OFF();
            }
        break;
        default:
            power = false;
            LED_OFF();
        break;
    }

    /* Low Power Mode */
    main_deinit();
    if (power) {
        deepsleep_start(10, BTN_IO);
    } else {
        deepsleep_start(0, BTN_IO);
    }
}
