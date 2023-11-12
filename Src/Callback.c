//
// Created by ytz20 on 2023/11/12.
//

#include "main.h"
#include "tim.h"

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
        ;
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