#ifndef PID_H
#define PID_H

// 头文件
#include "main.h"

// PID模式共用体
enum PID_MODE
{
    PID_POSITION = 0,
    PID_DELTA
};

// PID结构体
typedef struct
{
    uint8_t mode;
	
    fp32 Kp;
    fp32 Ki;
    fp32 Kd;

    fp32 max_out;  //最大输出
    fp32 max_iout; //最大积分输出

    fp32 set;
    fp32 fdb;

    fp32 out;
    fp32 Pout;
    fp32 Iout;
    fp32 Dout;
    fp32 Dbuf[3];  //微分项 0最新 1上一次 2上上次
    fp32 error[3]; //误差项 0最新 1上一次 2上上次

} PidTypeDef;

// 函数声明
void PID_Init(PidTypeDef *pid, uint8_t mode, const fp32 PID[3], fp32 max_out, fp32 max_iout);
fp32 PID_Calc(PidTypeDef *pid, fp32 ref, fp32 set);
void PID_clear(PidTypeDef *pid);

#endif
