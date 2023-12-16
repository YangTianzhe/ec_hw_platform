//
// Created by ytz20 on 2023/11/25.
//

#ifndef PROJECT_BMI088_H
#define PROJECT_BMI088_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef  struct
{
    //ACCEL 加速度计 [g]
    float accel_x;
    float accel_y;
    float accel_z;

    //GYRO 陀螺仪 [deg/s]
    float gyro_x;
    float gyro_y;
    float gyro_z;

    float temperature;
}BMI088_DATA_TYPE;

void BMI088_ACCEL_NS_L(void);
void BMI088_ACCEL_NS_H(void);
void BMI088_GYRO_NS_L(void);
void BMI088_GYRO_NS_H(void);
static void BMI088_ReadReg_ACCEL(uint8_t reg, uint8_t *return_data, uint8_t length);
static void BMI088_ReadReg_GYRO(uint8_t reg, uint8_t *return_data, uint8_t length);
static void BMI088_WriteReg(uint8_t reg, uint8_t write_data);

void BMI088_Init(void);
void BMI088_ReadData(void);

#ifdef __cplusplus
}
#endif

#endif //PROJECT_BMI088_H
