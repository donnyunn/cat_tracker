/*
   This code is written by Ironwalks Ltd. but it is in the Public Domain 
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

void app_main(void)
{
    /* gatt server and beacon */
    // lets check gatts_deme.c (342) : esp_log_buffer_hex function
    gatts_init();

    while (1) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}