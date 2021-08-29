#include "packet.h"

#define TAG "PACKET"

#define LIST_LEN 256

#define STORAGE_NAMESPACE "storage"

packet_t * this;

RTC_DATA_ATTR uint16_t g_num = 0;
RTC_DATA_ATTR uint16_t g_push_ptr = 0;
RTC_DATA_ATTR uint16_t g_pull_ptr = 0;

void packet_push(uint8_t * buf)
{
    esp_err_t err;
    nvs_handle_t handle;
    char key[10] = {0, };

    this->num = g_num++;
    memcpy(this->data, buf, DATA_LEN);

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) ESP_LOGE(TAG, "nvs_open failed %d", err);

    sprintf(key, "%d", g_push_ptr);
    err = nvs_set_blob(handle, key, this, sizeof(packet_t));
    if (err != ESP_OK) ESP_LOGE(TAG, "nvs_set_blob failed %d", err);

    err = nvs_commit(handle);
    if (err != ESP_OK) ESP_LOGE(TAG, "nvs_commit failed %d", err);

    nvs_close(handle);

    printf("pushed(%d): ", g_push_ptr);
    for (int i = 0; i < DATA_LEN; i++) {
        printf("%02x", buf[i]);
    }
    printf("\n");

    if (++g_push_ptr == LIST_LEN) {
        g_push_ptr = 0;
    }
}

int packet_pull(uint8_t * buf) 
{
    esp_err_t err;
    nvs_handle_t handle;
    char key[10] = {0, };
    packet_t temp;
    int ret = g_push_ptr >= g_pull_ptr ? 
                (g_push_ptr - g_pull_ptr - 1) : 
                (LIST_LEN + g_push_ptr - g_pull_ptr - 1);
                
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) ESP_LOGE(TAG, "nvs_open failed %d", err);

    size_t req_size = sizeof(packet_t);
    sprintf(key, "%d", g_pull_ptr);
    err = nvs_get_blob(handle, key, (uint8_t*)this, &req_size);
    if (err != ESP_OK) ESP_LOGE(TAG, "nvs_set_blob this failed %d", err);

    sprintf(key, "%d", g_push_ptr-1);
    err = nvs_get_blob(handle, key, (uint8_t*)&temp, &req_size);
    if (err != ESP_OK) ESP_LOGE(TAG, "nvs_set_blob temp failed %d", err);
    
    nvs_close(handle);

    this->num = temp.num - this->num;
    memcpy(buf, (uint8_t*)this, sizeof(packet_t));
    
    printf("pulled(%d)\n", g_pull_ptr);
    if (++g_pull_ptr == LIST_LEN) {
        g_pull_ptr = 0;
    }

    return ret;
}

void packet_init(packet_t * packet)
{
    this = packet;
}
