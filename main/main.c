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

#define TAG "MAIN"

int get_time(void)
{
    int result;
    struct timeval rawtime;
    gettimeofday(&rawtime, NULL);
    result = rawtime.tv_sec * 1000 + rawtime.tv_usec/1000;
    return result;
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

    /* first data */
    mpu6050_read(&imu);
    packet_push(imu.data);

    do {
        vTaskDelay(500 / portTICK_PERIOD_MS);
    } while (gatts_isConnected());

    deepsleep_init(9);

    // while (1) {
    //     if (!gatts_isConnected()) {
    //         vTaskDelay(10 / portTICK_PERIOD_MS);
    //         mpu6050_read(&imu);
    //         packet_push(imu.data);
    //         vTaskDelay(1000 / portTICK_PERIOD_MS);
    //         deepsleep_init(9);
    //     } else {
    //         while (!gatts_isConnected()) {
    //             printf(".");
    //             vTaskDelay(500 / portTICK_PERIOD_MS);
    //         }
    //     }
        
    //     vTaskDelay(10 / portTICK_PERIOD_MS);
    // }
}