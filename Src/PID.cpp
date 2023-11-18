//
// Created by ytz20 on 2023/11/18.
//

#include "../Inc/PID.hpp"
#include "../Inc/my_Math.h"

PID::PID(float kp, float ki, float kd, float i_max, float out_max)
{
    kp_=kp;
    ki_=ki;
    kd_=kd;
    i_max_=i_max;
    out_max_=out_max;
}

float PID::Calculate(float ref, float fdb)
{
    ref_=ref;
    fdb_=fdb;

    last_err_=err_;
    err_=ref_-fdb_;
    err_sum_=Limit(err_sum_+err_,-i_max_,i_max_);

    pout_=kp_*err_;
    iout_=ki_*err_sum_;
    dout_=kd_*(last_err_-err_);

    output_=Limit(pout_+iout_+dout_,-out_max_,out_max_);
    return output_;
}