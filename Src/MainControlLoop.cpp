//
// Created by ytz20 on 2023/11/12.
//

#include "../Inc//MainControlLoop.h"
#include "../Inc/Motor.hpp"
#include "../Inc/RemoteControl.h"
#include "../Inc/BMI088.h"

extern Motor motor1;
extern RC_Ctl_t RC_CtrlData;

void MainControlLoop(void)
{
    if(RC_CtrlData.rc.s1!=RC_SW_MID)
        motor1.Reset();
    else
        motor1.target_speed_=RC_CtrlData.rc.ch0*500;
    motor1.Handle();
    MotorControlCANTx();

    BMI088_ReadData();
}