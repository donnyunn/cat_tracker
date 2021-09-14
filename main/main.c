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
#define CNT_RETRY 15

RTC_DATA_ATTR int cntBoot;

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

    /* gatt server and beacon */
    gatts_init();

    /* packet manager */
    packet_init(&packet);

    if ((cntBoot++ % 2) == 0) {
        led_init();
        gpio_set_level(LED_IO, 1);

        /* mpu6050 IMU Sensor */
        mpu6050_init();

        /* record IMU data */
        mpu6050_read(&imu);
        packet_push(imu.data);

        deinitialize();
        
        gpio_set_level(LED_IO, 0);
    }
    
    /* GATT SERVER Advertisement */
    while (!gatts_isConnected()) {
        if (retry++ > CNT_RETRY) {
            break;
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    // vTaskDelay(600 / portTICK_PERIOD_MS);

    /* Stay awake while connected */
    while (gatts_isConnected()) {
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }

    /* Low Power Mode */
    deepsleep_start(13);
}
