#include "pid.h"
#include "attitude.h"
#include "motor.h"

#define k 0.01

void PID_init(pid* pid)
{
    pid->target = (Vec3d_t){0,0,0};
    pid->err = (Vec3d_t){0,0,0};
    pid->err_last = (Vec3d_t){0,0,0};
    pid->integral = (Vec3d_t){0,0,0};
    pid->kp = 0;
    pid->ki = 0;
    pid->kd = 0;
    pid->proportion = 0;
    pid->output = 0;
    pid->differ = 0;
}

void PID_setTarget(pid* pid, Vec4d_t target)
{
    pid->target = target;
}

void PID_setKP(pid* pid, float kp)
{
    pid->kp = kp;
}

void PID_setKI(pid* pid, float ki)
{
    pid->ki = ki;
}

void PID_setKD(pid* pid, float kd)
{
    pid->kd = kd;
}

void PID_positon_call(pid* pid, Vec3d_t target, Vec3d_t measure)
{
    pid->err    = vec3_add(target, vec3_scale(measure, -1));
    pid->differ = vec3_add(pid->err, vec3_scale(pid->err_last, -1));

    pid->output = vec3_add(vec3_scale(pid->err, pid->kp), vec3_add(vec3_scale((pid->integral, pid->ki)), vec3_scale(pid->differ, pid->kd)));

    pid->integral = vec3_add(pid->integral, pid->err);

    pid->err_last = pid->err;
}
