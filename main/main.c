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

void deinitialize(void)
{
    gpio_config_t io_conf;

    mpu6050_deinit();

    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.pin_bit_mask = 0x7FFFFFFFFF;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);
}

void app_main(void)
{
    mpu6050_t imu;
    packet_t packet;

    /* gatt server and beacon */
    gatts_init();

    /* mpu6050 IMU Sensor */
    mpu6050_init();

    /* packet manager */
    packet_init(&packet);

    /* record IMU data */
    mpu6050_read(&imu);
    packet_push(imu.data);
    
    /* GATT SERVER Advertisement */
    vTaskDelay(500 / portTICK_PERIOD_MS);

    /* Stay awake while connected */
    while (gatts_isConnected()) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    /* Low Power Mode */
    deinitialize();
    deepsleep_start(29);
}
