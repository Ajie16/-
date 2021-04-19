/*******************************************************************************************
* PID
* ����汾��V1.0
* �������ڣ�2020-9-7
* �������ߣ����
*******************************************************************************************/
#include "pid.h"
//#include "structconfig.h"

//�ǶȲ�PID 
PID_TYPE   PID_Angle;

//λ�Ʋ�PID 
PID_TYPE   PID_Move;


/*****************************************************************************
* ��  ����void PID_Postion_Cal(PID_TYPE*PID,float target,float measure)
* ��  �ܣ�λ��ʽPID�㷨
* ��  ����PID: �㷨P I D�����Ľṹ��
*         target: Ŀ��ֵ
*         measure: ����ֵ
* ����ֵ����
* ��  ע: �ǶȻ��ͽ��ٶȻ����ô˺���
*****************************************************************************/
void PID_Postion_Cal(PID_TYPE*PID,float target,float measure)
{
	PID->Error  = target - measure;              //���
	PID->Differ = PID->Error - PID->PreError;    //΢����
	PID->Pout = PID->P * PID->Error;                        //��������
	PID->Iout = PID->Ilimit_flag * PID->I * PID->Integral;  //���ֿ���
	PID->Dout = PID->D * PID->Differ;                       //΢�ֿ���
	PID->OutPut =  PID->Pout + PID->Iout + PID->Dout;       //���� + ���� + ΢���ܿ���
	PID->Integral += PID->Error;                        //�������л���
	if(PID->Integral > PID->Irang)                      //�����޷�
		PID->Integral = PID->Irang;
	if(PID->Integral < -PID->Irang)                     //�����޷�
		PID->Integral = -PID->Irang;                    
	PID->PreError = PID->Error ;                            //ǰһ�����ֵ
}

/*****************************************************************************
* ��  ����void PidParameter_init(void)
* ��  �ܣ���ʼ��PID�ṹ�����һЩ��Աֵ
* ��  ������
* ����ֵ���� 
* ��  ע: ����PID�������Ƕ�������FLASH�У����Դ˺�����ʼ��ʱ���ó�ʼ����Щ������
*         ����Flash�еĲ����п�����Ϊ����������������Flash��ȡ����ʧ�ܣ����
*         ��ʼ��ΪĬ�ϲ�����
*****************************************************************************/
void PidParameter_init(void)
{
	//ROLL��
	PID_ROL_Angle.Irang = 200;    //Roll��ǶȻ����޷��ȣ����ڵ��������ޣ����Ի������Ҳ�����޵ģ�

	PID_ROL_Angle.P=0.6;
	PID_ROL_Angle.I=0.05;
	PID_ROL_Angle.D=0;
	//PITCH��
	PID_PIT_Angle.Irang = 200;    //Roll��ǶȻ����޷��ȣ����ڵ��������ޣ����Ի������Ҳ�����޵ģ�
	
	PID_PIT_Angle.P=0.3;
	PID_PIT_Angle.I=0.02;
	PID_PIT_Angle.D=0;
	
	
	//�߶Ȼ�
	PID_ALT_Rate.Irang = 200;
	
	PID_ALT_Rate.P = 2.5;
	PID_ALT_Rate.I = 0.5;
	PID_ALT_Rate.D = 4;
	
	
	//�ǶȻ�
	PID_Angle.P = 0.06;
	PID_Angle.I = 0.0;
	PID_Angle.D = 0.5;
	//ƫ�
	PID_Move.P = 80;
	PID_Move.I = 5;
	PID_Move.D = 10;

//	PID_Move.P = 0;
//	PID_Move.I = 0;
//	PID_Move.D = 0;

}


