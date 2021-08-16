#ifndef __PACKET_H
#define __PACKET_H
#define MPU6050_DATA_LEN 14

#include "main.h"

#define DATA_LEN 14

typedef struct {
    uint16_t num;
    uint8_t data[DATA_LEN];
} packet_t;

void packet_push(uint8_t * buf);
int packet_pull(uint8_t * buf);
void packet_init(packet_t * packet);



// /**
//  * @brief Listing packets function from input_data, to list_data
//  *
//  * @note 
//  *
//  * @param num Order of input_data.
//  * @param LIST_LEN Length of lists.
//  * @param system_time Saving time.
//  * @param input_data Name got contents
//  * @param list_data Name got contents
//  *
//  */
// void packet_init(int num, int LIST_LEN, int system_time, mpu6050_t input_data, data_array_t * list_data);

#endif 