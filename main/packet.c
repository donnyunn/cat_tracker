#include "packet.h"

#define TAG "PACKET"

#define DEBUG 1
#ifdef DEBUG
#define LIST_LEN 25
#else
#define LIST_LEN 256
#endif

packet_t * this;

RTC_DATA_ATTR uint16_t g_num = 0;
RTC_DATA_ATTR uint8_t g_push_ptr = 0;
RTC_DATA_ATTR uint8_t g_pull_ptr = 0;
RTC_DATA_ATTR packet_t packet_list[LIST_LEN];

void packet_push(uint8_t * buf)
{
    packet_list[g_push_ptr].num = g_num++;
    memcpy(packet_list[g_push_ptr].data, buf, DATA_LEN);

    printf("pushed(%d): ", g_push_ptr);
    printf("%04x", packet_list[g_push_ptr].num);
    for (int i = 0; i < DATA_LEN; i++) {
        printf("%02x", packet_list[g_push_ptr].data[i]);
    }
    printf("\n");

#ifndef DEBUG
    g_push_ptr++;
#else
    if (++g_push_ptr == LIST_LEN) {
        g_push_ptr = 0;
    }
#endif
}

int packet_pull(uint8_t * buf) 
{
    uint16_t t_stamp;
    int ret = g_push_ptr >= g_pull_ptr ? 
                (g_push_ptr - g_pull_ptr - 1) : 
                (LIST_LEN + g_push_ptr - g_pull_ptr - 1);
    
    t_stamp = (packet_list[g_push_ptr-1].num - packet_list[g_pull_ptr].num);
    packet_list[g_pull_ptr].num = t_stamp;
    memcpy(buf, (uint8_t*)(&packet_list[g_pull_ptr]), sizeof(packet_list[g_pull_ptr]));

    printf("pulled(%d): ", g_pull_ptr);
    for (int i = 0; i < sizeof(packet_list[g_pull_ptr]); i++) {
        printf("%02x", buf[i]);
    }
    printf("\n");

#ifndef DEBUG
    g_pull_ptr++;
#else
    if (++g_pull_ptr == LIST_LEN) {
        g_pull_ptr = 0;
    }
#endif

    return ret;
}

void packet_init(packet_t * packet)
{
    this = packet;
}

// int end_i;
// int num_imsi;
// void packet_init(int num, int LIST_LEN, int system_time, mpu6050_t input_data, data_array_t * list_data)
// {
//     if (num>=LIST_LEN)
//     {
//         end_i = LIST_LEN-1;
//     }
//     else
//     {
//         end_i = num;
//     }


//     list_data[num% LIST_LEN].record_time = system_time;
//     // int mydata.data_num = num;
//     list_data[num% LIST_LEN].turn = num;
//     list_data[num% LIST_LEN].AX = (short)(input_data.data[0]<<8 | input_data.data[1]);
//     list_data[num% LIST_LEN].AY = (short)(input_data.data[2]<<8 | input_data.data[3]);
//     list_data[num% LIST_LEN].AZ = (short)(input_data.data[4]<<8 | input_data.data[5]);
    
//     list_data[num% LIST_LEN].Temp = (short)(input_data.data[6]<<8 | input_data.data[7]);
    
//     list_data[num% LIST_LEN].GyX = (short)(input_data.data[8]<<8 | input_data.data[9]);
//     list_data[num% LIST_LEN].GyY = (short)(input_data.data[10]<<8 | input_data.data[11]);
//     list_data[num% LIST_LEN].GyZ = (short)(input_data.data[12]<<8 | input_data.data[13]);
    
//     //printf("\n%d, %d, %d, %d\n", num, mydata[num].AX, mydata[num].AY, mydata[num].AZ);

//     for(int i=0; i<=(end_i); i++){
//         num_imsi = i % LIST_LEN;


//         printf("  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", list_data[num_imsi].turn, list_data[num_imsi].record_time , list_data[num_imsi].AX , list_data[num_imsi].AY , list_data[num_imsi].AZ , list_data[num_imsi].Temp , list_data[num_imsi].GyX , list_data[num_imsi].GyY , list_data[num_imsi].GyZ );

//     }
//     if (num % LIST_LEN ==0)
//     {
//     ESP_LOGI("MAIN", "Loop end!");
//     }
// }
