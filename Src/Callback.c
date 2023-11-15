//
// Created by ytz20 on 2023/11/12.
//

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "RemoteControl.h"

extern volatile uint8_t sbus_rx_buffer[RC_FRAME_LENGTH];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==htim6.Instance)  //1000Hz
    {
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
        //MainControlLoop();
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
    if(htim->Instance==htim7.Instance)  //1000Hz
    {
        //Count();
        //Remote.Handle();
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance==USART3) //SBUS/DBUS RemoteControl
    {
        RemoteControlDataReceive();
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3,(uint8_t*) sbus_rx_buffer,RC_FRAME_LENGTH);
        RemoteControlDataUtilize();
    }
    if(huart->Instance==USART1) //UART2 1000000bps
    {
        ;
    }
    if(huart->Instance==USART6) //UART1 115200bps
    {
        ;
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&rx_header,rx_data);

    //motorsCanRxMsgHandle(hcan,rx_header,rx_data);
    //if(ultra_cap.canRxMsgCheck(hcan,rx_header))
    //{
    //    ultra_cap.canRxMsgCallback(hcan, rx_header, rx_data);
    //}
}