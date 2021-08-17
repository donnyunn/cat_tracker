#include "packet.h"

#define TAG "PACKET"

// #define DEBUG
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

    if (++g_push_ptr == LIST_LEN) {
        g_push_ptr = 0;
    }
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

    if (++g_pull_ptr == LIST_LEN) {
        g_pull_ptr = 0;
    }

    return ret;
}

void packet_init(packet_t * packet)
{
    this = packet;
}
