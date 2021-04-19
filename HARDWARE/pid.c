/*******************************************************************************************
* PID
* 程序版本：V1.0
* 程序日期：2020-9-7
* 程序作者：徐杰
*******************************************************************************************/
#include "pid.h"
//#include "structconfig.h"

//角度差PID 
PID_TYPE   PID_Angle;

//位移差PID 
PID_TYPE   PID_Move;


/*****************************************************************************
* 函  数：void PID_Postion_Cal(PID_TYPE*PID,float target,float measure)
* 功  能：位置式PID算法
* 参  数：PID: 算法P I D参数的结构体
*         target: 目标值
*         measure: 测量值
* 返回值：无
* 备  注: 角度环和角速度环共用此函数
*****************************************************************************/
void PID_Postion_Cal(PID_TYPE*PID,float target,float measure)
{
	PID->Error  = target - measure;              //误差
	PID->Differ = PID->Error - PID->PreError;    //微分量
	PID->Pout = PID->P * PID->Error;                        //比例控制
	PID->Iout = PID->Ilimit_flag * PID->I * PID->Integral;  //积分控制
	PID->Dout = PID->D * PID->Differ;                       //微分控制
	PID->OutPut =  PID->Pout + PID->Iout + PID->Dout;       //比例 + 积分 + 微分总控制
	PID->Integral += PID->Error;                        //对误差进行积分
	if(PID->Integral > PID->Irang)                      //积分限幅
		PID->Integral = PID->Irang;
	if(PID->Integral < -PID->Irang)                     //积分限幅
		PID->Integral = -PID->Irang;                    
	PID->PreError = PID->Error ;                            //前一个误差值
}

/*****************************************************************************
* 函  数：void PidParameter_init(void)
* 功  能：初始化PID结构体里的一些成员值
* 参  数：无
* 返回值：无 
* 备  注: 由于PID参数我们都保存在FLASH中，所以此函数初始化时不用初始化这些参数，
*         但是Flash中的参数有可能因为误操作被擦除，如果Flash读取参数失败，则就
*         初始化为默认参数。
*****************************************************************************/
void PidParameter_init(void)
{
	//ROLL轴
	PID_ROL_Angle.Irang = 200;    //Roll轴角度积分限幅度（由于电机输出有限，所以积分输出也是有限的）

	PID_ROL_Angle.P=0.6;
	PID_ROL_Angle.I=0.05;
	PID_ROL_Angle.D=0;
	//PITCH轴
	PID_PIT_Angle.Irang = 200;    //Roll轴角度积分限幅度（由于电机输出有限，所以积分输出也是有限的）
	
	PID_PIT_Angle.P=0.3;
	PID_PIT_Angle.I=0.02;
	PID_PIT_Angle.D=0;
	
	
	//高度环
	PID_ALT_Rate.Irang = 200;
	
	PID_ALT_Rate.P = 2.5;
	PID_ALT_Rate.I = 0.5;
	PID_ALT_Rate.D = 4;
	
	
	//角度环
	PID_Angle.P = 0.06;
	PID_Angle.I = 0.0;
	PID_Angle.D = 0.5;
	//偏差环
	PID_Move.P = 80;
	PID_Move.I = 5;
	PID_Move.D = 10;

//	PID_Move.P = 0;
//	PID_Move.I = 0;
//	PID_Move.D = 0;

}


