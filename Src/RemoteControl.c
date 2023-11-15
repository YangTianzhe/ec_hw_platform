//
// Created by ytz20 on 2023/11/15.
//

#include "main.h"
#include "RemoteControl.h"

volatile uint8_t sbus_rx_buffer[RC_FRAME_LENGTH];
static RC_Ctl_t RC_CtrlData;

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;

void RemoteControlInit(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3,(uint8_t*) sbus_rx_buffer,RC_FRAME_LENGTH);
}

void RemoteControlDataReceive(void)
{
    RC_CtrlData.rc.ch0=((uint16_t)sbus_rx_buffer[0]|(uint16_t)sbus_rx_buffer[1]<<8)&0x07FF;
    RC_CtrlData.rc.ch1=((uint16_t)sbus_rx_buffer[1]>>3|(uint16_t)sbus_rx_buffer[2]<<5)&0x07FF;
    RC_CtrlData.rc.ch2=((uint16_t)sbus_rx_buffer[2]>>6|(uint16_t)sbus_rx_buffer[3]<<2|(uint16_t)sbus_rx_buffer[4]<<10)&0x07FF;
    RC_CtrlData.rc.ch3=((uint16_t)sbus_rx_buffer[4]>>1|(uint16_t)sbus_rx_buffer[5]<<7)&0x07FF;
    RC_CtrlData.rc.s1=((uint8_t)sbus_rx_buffer[5]>>4)&0x03;
    RC_CtrlData.rc.s2=((uint8_t)sbus_rx_buffer[5]>>6)&0x03;

    RC_CtrlData.mouse.x=((uint16_t)sbus_rx_buffer[6]|(uint16_t)sbus_rx_buffer[7]<<8);
    RC_CtrlData.mouse.y=((uint16_t)sbus_rx_buffer[8]|(uint16_t)sbus_rx_buffer[9]<<8);
    RC_CtrlData.mouse.z=((uint16_t)sbus_rx_buffer[10]|(uint16_t)sbus_rx_buffer[11]<<8);
    RC_CtrlData.mouse.press_l=((uint8_t)sbus_rx_buffer[12]);
    RC_CtrlData.mouse.press_r=((uint8_t)sbus_rx_buffer[13]);

    RC_CtrlData.key.v=((uint16_t)sbus_rx_buffer[14]|(uint16_t)sbus_rx_buffer[15]<<8);
}

void RemoteControlDataUtilize(void)
{
    if(HAL_UART_GetState(&huart1)==HAL_UART_STATE_READY)
        HAL_UART_Transmit_IT(&huart1,(const uint8_t *)&RC_CtrlData,RC_FRAME_LENGTH);
}