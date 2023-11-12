//
// Created by ytz20 on 2023/11/12.
//

#include "main.h"
#include "my_can.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

void canFilterInit(void)
{
    CAN_FilterTypeDef filter;
    filter.FilterIdHigh=0x0000;
    filter.FilterIdLow=0x0000;
    filter.FilterMaskIdHigh=0x0000;
    filter.FilterMaskIdLow=0x0000;
    filter.FilterFIFOAssignment=CAN_FILTER_FIFO0;
    //filter.FilterBank;
    filter.FilterMode=CAN_FILTERMODE_IDMASK;
    filter.FilterScale=CAN_FILTERSCALE_32BIT;
    filter.FilterActivation=CAN_FILTER_ENABLE;
    filter.SlaveStartFilterBank=14;

    filter.FilterBank=0;
    HAL_CAN_ConfigFilter(&hcan1,&filter);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);

    filter.FilterBank=14;
    HAL_CAN_ConfigFilter(&hcan2,&filter);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2,CAN_IT_RX_FIFO0_MSG_PENDING);
}