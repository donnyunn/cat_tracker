
#ifndef __DEEPSLEEP_H
#define __DEEPSLEEP_H

#include "main.h"

esp_sleep_source_t deepsleep_get_wakeup(void);
void deepsleep_start(int sleep_period, int gpio_num);

#endif /* __DEEPSLEEP_H */