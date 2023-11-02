#ifndef __PID_H
#define __PID_H

#include "mathkit.h"
#include "stm32f4xx.h"

typedef struct
{
    uint32_t motor1;
    uint32_t motor2;
    uint32_t motor3;
    uint32_t motor4;
}pwm;


typedef struct
{
    Vec3d_t target;     //目标值
    Vec3d_t actual;     //实际值
    Vec3d_t err;        //误差
    Vec3d_t err_last;   //上一次误差
    Vec3d_t integral;   //误差积分
    Vec3d_t output;     //比例
    Vec3d_t differ;     //微分
    float ki,kp,kd;     //比例、积分、微分系数
}pid;

void PID_init(pid* pid);
void PID_setTarget(pid* pid, Vec4d_t target);
void PID_setKP(pid* pid, float kp);
void PID_setKI(pid* pid, float ki);
void PID_setKD(pid* pid, float kd);
void PID_positon_call(pid* pid, Vec3d_t target, Vec3d_t measure);

#endif
