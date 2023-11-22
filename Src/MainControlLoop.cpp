//
// Created by ytz20 on 2023/11/12.
//

#include "main.h"
#include "../Inc//MainControlLoop.h"
#include "../Inc/Motor.hpp"

extern Motor motor1;

void MainControlLoop(void)
{
    motor1.Handle();
    MotorControlCANTx();
}