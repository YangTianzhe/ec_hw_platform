//
// Created by ytz20 on 2023/11/18.
//

#include "../Inc/Motor.hpp"
#include <cmath>
#include "../Inc/my_Math.h"


Motor::Motor(const Type& type, const float& ratio, const ControlMethod& method,
             const PID& ppid, const PID& spid)
{
    info_.type=type;
    info_.ratio=ratio;
    mode_=Motor::WORKING;
    method_=method;
    intensity_=0;
    target_angle_=0;
    target_speed_=0;
    motor_data_.angle=0.0;
    motor_data_.ecd_angle=0.0;
    motor_data_.last_ecd_angle=0.0;
    motor_data_.rotate_speed=0.0;
    motor_data_.current=0.0;
    motor_data_.temp=0.0;
    if(method_==Motor::POSITION_SPEED)
        ppid_=PID(ppid.kp_,(ratio>0)?ppid.ki_:(-ppid.ki_),(ratio>0)?ppid.kd_:(-ppid.kd_),ppid.i_max_,ppid.out_max_);
    spid_=PID(spid.kp_,(ratio>0)?spid.ki_:(-spid.ki_),(ratio>0)?spid.kd_:(-spid.kd_),spid.i_max_,spid.out_max_);
}
void Motor::Reset() // 重置电机所有状态
{
    motor_data_.angle=0.0;
    motor_data_.ecd_angle=0.0;
    motor_data_.last_ecd_angle=0.0;
    motor_data_.rotate_speed=0.0;
    motor_data_.current=0.0;
    motor_data_.temp=0.0;
}
void Motor::Handle() // 根据当前 mode_ 计算控制量
{
    if(mode_==Motor::POWER_OFF)// 断电，控制量置零
    {
        intensity_=0;
        return;
    }
    if(mode_==Motor::WORKING)// 电机正常工作
    {
        motor_data_.angle = motor_data_.ecd_angle / info_.ratio; //减速比为编码器角度/输出角度
        if (method_ == Motor::POSITION_SPEED) //双环控制，计算位置PID
            target_speed_ = ppid_.Calculate(target_angle_, motor_data_.angle);
    }
    else if(mode_==Motor::STOP)// 将目标速度置零，计算得出控制量，使得电机停转
    {
        target_speed_=0.0;
    }
    if(info_.type==Motor::M3508) //C620电调
        intensity_=(int16_t)round(spid_.Calculate(target_speed_,motor_data_.rotate_speed)/10*10000);
        //电调接受控制量-10000~10000，对应电流-10A~10A
    else if(info_.type==Motor::M2006) //C610电调
        intensity_=(int16_t)round(spid_.Calculate(target_speed_,motor_data_.rotate_speed)/20*16384);
        //电调接受控制量-16384~16384，对应电流-20A~20A
    else if(info_.type==Motor::GM6020)
        intensity_=(int16_t)round(spid_.Calculate(target_speed_,motor_data_.rotate_speed)/24*30000);
        //电调接受控制量-30000~30000，对应电压-24V~24V
}
void Motor::SetAngle(const float& target_angle) // 设置目标角度
{
    target_angle_=target_angle;
}
void Motor::SetSpeed(const float& target_speed) // 设置目标速度
{
    target_speed_=target_speed;
}



extern Motor motor1;
static uint16_t montor_Tx_message[4];
static CAN_TxHeaderTypeDef montor_Tx_header;
static uint32_t TxMailbox0=CAN_TX_MAILBOX0;
static uint32_t TxMailbox1=CAN_TX_MAILBOX1;

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;


void MotorControlCANTx(void)
{
    montor_Tx_message[0]=(uint16_t)(motor1.intensity_);
    montor_Tx_message[1]=0;
    montor_Tx_message[2]=0;
    montor_Tx_message[3]=0;

    montor_Tx_header.StdId=0x200;
    montor_Tx_header.ExtId=0;
    montor_Tx_header.IDE=CAN_ID_STD;
    montor_Tx_header.RTR=CAN_RTR_DATA;
    montor_Tx_header.DLC=8;
    montor_Tx_header.TransmitGlobalTime=DISABLE;
    HAL_CAN_AddTxMessage(&hcan1,&montor_Tx_header,(uint8_t*)montor_Tx_message,&TxMailbox0);

/*
    montor_Tx_message[0]=(uint16_t)(motor[4].intensity_);
    montor_Tx_message[1]=(uint16_t)(motor[5].intensity_);
    montor_Tx_message[2]=(uint16_t)(motor[6].intensity_);
    montor_Tx_message[3]=(uint16_t)(motor[7].intensity_);

    montor_Tx_header={0x1FF,0,CAN_ID_STD,CAN_RTR_DATA,8,DISABLE};
    HAL_CAN_AddTxMessage(&hcan1,&montor_Tx_header,(uint8_t*)montor_Tx_message,&TxMailbox1);
*/
}

void MotorControlCANRx(CAN_HandleTypeDef *hcan,const CAN_RxHeaderTypeDef *rx_header,const uint8_t *rx_data)
{
    if(rx_header->StdId < 0x201 || rx_header->StdId > 0x20B) //不是DJI电机数据包
        return;
    //uint16_t motor_index=rx_header->StdId - 0x201;
    if(motor1.info_.type==Motor::M3508)
        //M3508最大空载转速为589rpm，在一个CAN周期中最多转动589rpm*1ms=3.534度
    {
        motor1.motor_data_.last_ecd_angle=motor1.motor_data_.ecd_angle;
        motor1.motor_data_.ecd_angle=Encoder2Degree((float)((uint16_t)rx_data[0]<<8|(uint16_t)rx_data[1]),8192);
        motor1.motor_data_.angle=motor1.motor_data_.ecd_angle/motor1.info_.ratio;
        motor1.motor_data_.rotate_speed=(float)((uint16_t)rx_data[2]<<8|(uint16_t)rx_data[3])/motor1.info_.ratio;
    }
    else if(motor1.info_.type==Motor::M2006)
        //M2006最大空载转速为777rpm，在一个CAN周期中最多转动777rpm*1ms=4.662度
    {
        motor1.motor_data_.last_ecd_angle=motor1.motor_data_.ecd_angle;
        motor1.motor_data_.ecd_angle=Encoder2Degree((float)((uint16_t)rx_data[0]<<8|(uint16_t)rx_data[1]),8192);
        motor1.motor_data_.angle=motor1.motor_data_.ecd_angle/motor1.info_.ratio;
        motor1.motor_data_.rotate_speed=(float)((uint16_t)rx_data[2]<<8|(uint16_t)rx_data[3])/motor1.info_.ratio;
        motor1.motor_data_.temp=(float)rx_data[6];
    }
    else if(motor1.info_.type==Motor::GM6020)
        //GM6020最大空载转速为320rpm，在一个CAN周期中最多转动320rpm*1ms=1.92度
    {
        motor1.motor_data_.last_ecd_angle=motor1.motor_data_.ecd_angle;
        motor1.motor_data_.ecd_angle=Encoder2Degree((float)((uint16_t)rx_data[0]<<8|(uint16_t)rx_data[1]),8192);
        motor1.motor_data_.angle=motor1.motor_data_.ecd_angle/motor1.info_.ratio;
        motor1.motor_data_.rotate_speed=(float)((uint16_t)rx_data[2]<<8|(uint16_t)rx_data[3])/motor1.info_.ratio;
        motor1.motor_data_.temp=(float)rx_data[6];
    }
}