//
// Created by ytz20 on 2023/11/25.
//

#ifndef PROJECT_BMI088_H
#define PROJECT_BMI088_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void BMI088_ACCEL_NS_L(void);
void BMI088_ACCEL_NS_H(void);
void BMI088_GYRO_NS_L(void);
void BMI088_GYRO_NS_H(void);
static void BMI088_read_single_reg(uint8_t reg, uint8_t *return_data);
uint8_t BMI088_read_write_byte(uint8_t txdata);

#ifdef __cplusplus
}
#endif

#endif //PROJECT_BMI088_H
