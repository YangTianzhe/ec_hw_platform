//
// Created by ytz20 on 2023/11/18.
//

#include "../Inc/Motor.hpp"
#include <math.h>


Motor::Motor(const Type& type, const float& ratio, const ControlMethod& method,
             const PID& ppid, const PID& spid)
{
    info_={type,ratio};
    mode_=Mode::WORKING;
    method_=method;
    intensity_=0;
    target_angle_=0;target_speed_=0;
    motor_data_={0.0,0.0,0.0,0.0,0.0,0.0};
    if(method_==ControlMethod::POSITION_SPEED)
        ppid_=PID(ppid.kp_,(ratio>0)?ppid.ki_:(-ppid.ki_),(ratio>0)?ppid.kd_:(-ppid.kd_),ppid.i_max_,ppid.out_max_);
    spid_=PID(spid.kp_,(ratio>0)?spid.ki_:(-spid.ki_),(ratio>0)?spid.kd_:(-spid.kd_),spid.i_max_,spid.out_max_);
}
void Motor::Reset() // 重置电机所有状态
{
    motor_data_={0.0,0.0,0.0,0.0,0.0,0.0};
}
void Motor::Handle() // 根据当前 mode_ 计算控制量
{
    if(mode_==Mode::WORKING)// 电机正常工作
    {
        motor_data_.angle=motor_data_.ecd_angle/info_.ratio; //减速比为编码器角度/输出角度
        if(method_==ControlMethod::POSITION_SPEED) //双环控制，计算位置PID
            target_speed_=ppid_.Calculate(target_angle_,motor_data_.angle);
        intensity_=round(spid_.Calculate(target_speed_,motor_data_.rotate_speed)*1000);
          //电调接受控制量-10000~10000，对应电流-10A~10A
    }
    if(mode_==Mode::STOP)// 将目标速度置零，计算得出控制量，使得电机停转
    {
        SetSpeed(0.0);
        intensity_=round(spid_.Calculate(target_speed_,motor_data_.rotate_speed)*1000);
          //电调接受控制量-10000~10000，对应电流-10A~10A
    }
    if(mode_==Mode::POWER_OFF)// 断电，控制量置零
    {
        intensity_=0;
    }
}
void Motor::SetAngle(const float& target_angle) // 设置目标角度
{
    target_angle_=target_angle;
}
void Motor::SetSpeed(const float& target_speed) // 设置目标速度
{
    target_speed_=target_speed;
}