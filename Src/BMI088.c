//
// Created by ytz20 on 2023/11/25.
//

#include "../Inc/BMI088.h"

extern SPI_HandleTypeDef hspi1;

BMI088_DATA_TYPE bmi088;

void BMI088_ACCEL_NS_L(void)
{
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
}
void BMI088_ACCEL_NS_H(void)
{
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
}
void BMI088_GYRO_NS_L(void)
{
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);
}
void BMI088_GYRO_NS_H(void)
{
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
}

static void BMI088_ReadReg_ACCEL(uint8_t reg, uint8_t *return_data, uint8_t length)
{
    uint8_t data;

    data=0x80|reg; // 将Bit0置1，表示读取数据
    HAL_SPI_Transmit(&hspi1,&data,1,1000);
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX); // 等待SPI发送完成
    HAL_SPI_Receive(&hspi1,return_data,1,1000); // ACCEL读取数据时舍弃首个字节
    HAL_SPI_Receive(&hspi1,return_data,length,1000);
}

static void BMI088_ReadReg_GYRO(uint8_t reg, uint8_t *return_data, uint8_t length)
{
    uint8_t data;

    data=0x80|reg; // 将Bit0置1，表示读取数据
    HAL_SPI_Transmit(&hspi1,&data,1,1000);
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX); // 等待SPI发送完成
    HAL_SPI_Receive(&hspi1,return_data,length,1000);
}

static void BMI088_WriteReg(uint8_t reg, uint8_t write_data)
{
    uint8_t data[2];

    data[0]=0x7F&reg; //0x80将Bit0置0，表示写入数据
    data[1]=write_data;
    HAL_SPI_Transmit(&hspi1,data,2,1000);
    while(HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX); //等待SPI发送完成
}

void BMI088_Init(void)
{
    // Soft Reset ACCEL
    BMI088_ACCEL_NS_H();
    BMI088_WriteReg(0x7E,0xB6); // Write 0xB6 to ACC_SOFTRESET(0x7E)
    HAL_Delay(1);
    BMI088_ACCEL_NS_L();

    // Soft Reset GYRO
    BMI088_GYRO_NS_L();
    BMI088_WriteReg(0x14,0xB6); // Write 0xB6 to GYRO_SOFTRESET(0x14)
    HAL_Delay(30);
    BMI088_GYRO_NS_H();

    // Switch ACCEL to Normal Mode
    BMI088_ACCEL_NS_H();
    HAL_Delay(1);
    BMI088_WriteReg(0x7D,0x04); // Write 0x04 to ACC_PWR_CTRL(0x7D)
    HAL_Delay(1);
    BMI088_ACCEL_NS_L();
}

void BMI088_ReadData(void)
{
    //uint8_t bmi_range;
    uint8_t bmi_data[6];

    // Read ACCEL Data
    BMI088_ACCEL_NS_L();
    BMI088_ReadReg_ACCEL(0x12,bmi_data,6); // Read ACCEL data from 0x12
    BMI088_ACCEL_NS_H();

    bmi088.accel_x=(float)((int16_t)(((uint16_t)bmi_data[1])<<8|((uint16_t)bmi_data[0])));
    bmi088.accel_y=(float)((int16_t)(((uint16_t)bmi_data[3])<<8|((uint16_t)bmi_data[2])));
    bmi088.accel_z=(float)((int16_t)(((uint16_t)bmi_data[5])<<8|((uint16_t)bmi_data[4])));

    // Read GYRO Data
    BMI088_GYRO_NS_L();
    BMI088_ReadReg_GYRO(0x02,bmi_data,6); // Read GYRO data from 0x02
    BMI088_GYRO_NS_H();

    bmi088.gyro_x=(float)((int16_t)(((uint16_t)bmi_data[1])<<8|((uint16_t)bmi_data[0])));
    bmi088.gyro_y=(float)((int16_t)(((uint16_t)bmi_data[3])<<8|((uint16_t)bmi_data[2])));
    bmi088.gyro_z=(float)((int16_t)(((uint16_t)bmi_data[5])<<8|((uint16_t)bmi_data[4])));
}