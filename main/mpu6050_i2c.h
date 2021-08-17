
#ifndef __MPU6050_I2C_H
#define __MPU6050_I2C_H

#include "main.h"

#define MPU6050_DATA_LEN 14
typedef struct {
    union {
        uint8_t data[MPU6050_DATA_LEN];

        struct {
            short AX;
            short AY;
            short AZ;
            
            short Temp;

            short GyX;
            short GyY;
            short GyZ;
        } reg;
    };
} mpu6050_t;

void mpu6050_deinit(void);
void mpu6050_read(mpu6050_t * this);
void mpu6050_init(void);


#endif /* __MPU6050_H */