#include "control.h"
#include "pid.h"
#include "math.h"
#include "structconfig.h"
#include "stdio.h"
#include "motor.h"
#include "MS5837.h"
#include "includes.h"

//�ǶȻ�PID 
PID_TYPE   PID_ROL_Angle;
PID_TYPE   PID_PIT_Angle;

//�߶Ȼ�PID 
PID_TYPE   PID_ALT_Rate;

float angle_rol;
float angle_pit;

float Moto_PWM_1=0.0f,Moto_PWM_2=0.0f,Moto_PWM_3=0.0f,Moto_PWM_4=0.0f;

int alt_target;

void Control(FLOAT_ANGLE *att_in,FLOAT_XYZ *gyr_in)
{
	OS_ERR err;
	FLOAT_ANGLE Measure_Angle;
	Measure_Angle.rol = att_in->rol; 
	Measure_Angle.pit = att_in->pit;



	//�ǶȻ�
	PID_Postion_Cal(&PID_ROL_Angle,angle_rol,Measure_Angle.rol);//ROLL�ǶȻ�PID 
	PID_Postion_Cal(&PID_PIT_Angle,angle_pit,Measure_Angle.pit);//PITH�ǶȻ�PID 

	OSSemPend ((OS_SEM   *)&ms5837_new,               //�ȴ����ź��� SemOfKey
                 (OS_TICK   )0,                       //����ѡ�񲻵ȴ����ò���Ч
                 (OS_OPT    )OS_OPT_PEND_NON_BLOCKING,//���û�ź������ò��ȴ�
                 (CPU_TS   *)0,                       //����ȡʱ���
                 (OS_ERR   *)&err);                   //���ش�������

	if(err == OS_ERR_NONE)
	{
		//�߶Ȼ�
		PID_Postion_Cal(&PID_ALT_Rate, alt_target , Pressure);//PITH�ǶȻ�PID 
	}   
	Moto_PWM_1 = - PID_ROL_Angle.OutPut + PID_PIT_Angle.OutPut - PID_ALT_Rate.OutPut+150;  
	Moto_PWM_2 = - PID_ROL_Angle.OutPut - PID_PIT_Angle.OutPut - PID_ALT_Rate.OutPut+150; 
	Moto_PWM_3 = - PID_ROL_Angle.OutPut - PID_PIT_Angle.OutPut + PID_ALT_Rate.OutPut+150;   
	Moto_PWM_4 =   PID_ROL_Angle.OutPut - PID_PIT_Angle.OutPut - PID_ALT_Rate.OutPut+150;   
	
//	Moto_PWM_1 = - PID_ROL_Angle.OutPut + PID_PIT_Angle.OutPut +150;  
//	Moto_PWM_2 = - PID_ROL_Angle.OutPut - PID_PIT_Angle.OutPut +150; 
//	Moto_PWM_3 = - PID_ROL_Angle.OutPut - PID_PIT_Angle.OutPut +150;   
//	Moto_PWM_4 =   PID_ROL_Angle.OutPut - PID_PIT_Angle.OutPut +150;  
	
  Moto_Pwm(Moto_PWM_1,Moto_PWM_2,Moto_PWM_3,Moto_PWM_4); //������ֵ���䵽��ʱ���������Ӧռ�ձȵ�PWM��
	
}
