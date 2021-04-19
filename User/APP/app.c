#include <includes.h>

//
//��ע��Ϣ��X��ӦPITCH
//					Y��ӦROL
//


/**************************************************************
*                       ����������ƿ�
**************************************************************/
static  OS_TCB   AppTaskStartTCB;

static  OS_TCB   OLED_TASK_TCB;
static  OS_TCB   Control_TASK_TCB;
static  OS_TCB   Led_TASK_TCB;
static  OS_TCB   Bluetooth_TASK_TCB;
static  OS_TCB   MS5837_TASK_TCB;

/**************************************************************
*                      ��������ջ
**************************************************************/
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK  OLED_TASK_Stk[OLED_TASK_SIZE];
static  CPU_STK  Control_TASK_Stk[Control_TASK_SIZE];
static  CPU_STK  Led_TASK_Stk[Led_TASK_SIZE];
static  CPU_STK  Bluetooth_TASK_Stk[Bluetooth_TASK_SIZE];
static  CPU_STK  MS5837_TASK_Stk[MS5837_TASK_SIZE];

/*************************************************************
*                    ��������
*************************************************************/

static  void  AppTaskStart  (void *p_arg);

static  void  OLED_TASK(void *p_arg);
static  void  Control_TASK(void *p_arg);
static  void  Led_TASK(void *p_arg);
static  void  Bluetooth_TASK(void *p_arg);
static  void  MS5837_TASK(void *p_arg);
/*************************************************************
*                    ��������
*************************************************************/
OS_SEM 				ms5837_new;
s16 x,j;

u8 mark=0;
u8 balance_mark=0;

u8 run_mode=0,state_none=0;												//Ѳ��
u8 time_mark=0;
u8 time_cnt=0;
extern float angle_rol;
extern float angle_pit;




//ˮ��Ŀ���趨 +15
u16 target_first=1060,target_second=1070,target_third=1072;

int  main (void)


{
    OS_ERR  err;

    /* ��ʼ��ϵͳ*/
    OSInit(&err);                                               

    /* �������� */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
                 
    /* ��������*/
    OSStart(&err);                                              
		
		
}


/***********************************************************************
  * @ ������  �� AppTaskStart
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
	
   (void)p_arg;
    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();                           
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        
    OS_CPU_SysTickInit(cnts);                                   
     
    /* �ڴ��ʼ��*/
    Mem_Init();                                                
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               
#endif

    CPU_IntDisMeasMaxCurReset();

		BSP_Init();   
		
		OSSemCreate (&ms5837_new,  //��ֵ�ź������ƿ�ָ��
                  "ms5837 up date", //��ֵ�ź�������
                  0,    //��Դ��Ŀ���¼��Ƿ�����־
                  &err);  //���ش�������

		
			OSTaskCreate((OS_TCB     *)&Control_TASK_TCB,                
						 (CPU_CHAR   *)"Control Task Start",
						 (OS_TASK_PTR ) Control_TASK,
						 (void       *) 0,
						 (OS_PRIO     ) Control_TASK_PRIO,
						 (CPU_STK    *) &Control_TASK_Stk[0],
						 (CPU_STK_SIZE) Control_TASK_SIZE / 10,
						 (CPU_STK_SIZE) Control_TASK_SIZE,
						 (OS_MSG_QTY  ) 5u,
						 (OS_TICK     ) 0u,
						 (void       *) 0,
						 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
						 (OS_ERR     *)&err);
						 
				OSTaskCreate((OS_TCB     *)&OLED_TASK_TCB,                
						 (CPU_CHAR   *)"OLED Task Start",
						 (OS_TASK_PTR ) OLED_TASK,
						 (void       *) 0,
						 (OS_PRIO     ) OLED_TASK_PRIO,
						 (CPU_STK    *) &OLED_TASK_Stk[0],
						 (CPU_STK_SIZE) OLED_TASK_SIZE / 10,
						 (CPU_STK_SIZE) OLED_TASK_SIZE,
						 (OS_MSG_QTY  ) 5u,
						 (OS_TICK     ) 0u,
						 (void       *) 0,
						 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
						 (OS_ERR     *)&err);
						 
						 
				OSTaskCreate((OS_TCB     *)&Led_TASK_TCB,                
						 (CPU_CHAR   *)"LED Task Start",
						 (OS_TASK_PTR ) Led_TASK,
						 (void       *) 0,
						 (OS_PRIO     ) Led_TASK_PRIO,
						 (CPU_STK    *) &Led_TASK_Stk[0],
						 (CPU_STK_SIZE) Led_TASK_SIZE / 10,
						 (CPU_STK_SIZE) Led_TASK_SIZE,
						 (OS_MSG_QTY  ) 5u,
						 (OS_TICK     ) 0u,
						 (void       *) 0,
						 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
						 (OS_ERR     *)&err);

				OSTaskCreate((OS_TCB     *)&Bluetooth_TASK_TCB,                
						 (CPU_CHAR   *)"BlueTooth Task Start",
						 (OS_TASK_PTR ) Bluetooth_TASK,
						 (void       *) 0,
						 (OS_PRIO     ) Bluetooth_TASK_PRIO,
						 (CPU_STK    *) &Bluetooth_TASK_Stk[0],
						 (CPU_STK_SIZE) Bluetooth_TASK_SIZE / 10,
						 (CPU_STK_SIZE) Bluetooth_TASK_SIZE,
						 (OS_MSG_QTY  ) 5u,
						 (OS_TICK     ) 0u,
						 (void       *) 0,
						 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
						 (OS_ERR     *)&err);


				OSTaskCreate((OS_TCB     *)&MS5837_TASK_TCB,                
						 (CPU_CHAR   *)"MS5837 Task Start",
						 (OS_TASK_PTR ) MS5837_TASK,
						 (void       *) 0,
						 (OS_PRIO     ) MS5837_TASK_PRIO,
						 (CPU_STK    *) &MS5837_TASK_Stk[0],
						 (CPU_STK_SIZE) MS5837_TASK_SIZE / 10,
						 (CPU_STK_SIZE) MS5837_TASK_SIZE,
						 (OS_MSG_QTY  ) 5u,
						 (OS_TICK     ) 0u,
						 (void       *) 0,
						 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
						 (OS_ERR     *)&err);
						 
		OSTaskDel(&AppTaskStartTCB,&err);

		//������ѭ��
    while (DEF_TRUE) 
    {                                          
			macLED1_TOGGLE ();
			delay_ms(100);
			OLED_ShowString(40,0, "WARNING",16);
			OLED_ShowString(40,16,"WARNING",16);
			OLED_ShowString(40,32,"WARNING",16);
			OLED_Refresh_Gram();
    }
}

static  void  OLED_TASK(void *p_arg)
{
	while(1)
	{
//		OLED_ShowString(12,0,"UCOS-3 Runing",16);
//		//���ݴ�ӡ
//		OLED_ShowString(0 ,16, "X:",16);
//		OLED_ShowString(40,16,"Y:",16);
//		
//		OLED_ShowString(0,32,"CPU_Using:  .  %",16);
//		
//		if((Att_Angle.pit-angle_pit)>0)
//		{
//			OLED_ShowChar(16,16,'+',16,1);
//			OLED_ShowNum(16+8,16,Att_Angle.pit-angle_pit,3,16);
//		}
//		else
//		{
//			OLED_ShowChar(16,16,'-',16,1);
//			OLED_ShowNum(16+8,16,-1*(Att_Angle.pit-angle_pit),3,16);
//		}
//		
//		if((Att_Angle.rol-angle_rol)>0)
//		{
//			OLED_ShowChar(56,16,'+',16,1);
//			OLED_ShowNum(56+8,16,Att_Angle.rol-angle_rol,3,16);
//		}
//		else
//		{
//			OLED_ShowChar(56,16,'-',16,1);
//			OLED_ShowNum(56+8,16,-1*(Att_Angle.rol-angle_rol),3,16);
//		}
		
		OLED_ShowNum(48,48,Pressure,5,16);
		OLED_ShowNum(0,48,USART2_RX_BUF[9]-48,5,16);
		
//		OLED_ShowNum(80,32,OSStatTaskCPUUsage/100,2,16);
//		OLED_ShowNum(104,32,OSStatTaskCPUUsage%100,2,16);
		OLED_Refresh_Gram();
		delay_ms(100);
	}
}

static  void  Control_TASK(void *p_arg)
{
	static u8 updat_target=0;
	
	//����ˮ��Ŀ��
	alt_target=target_first;
	
	while(1)
	{
		Prepare_Data(); //��ȡ��̬������������
		IMUupdate(&Gyr_rad,&Acc_filt,&Att_Angle); //��Ԫ����̬����
		if(updat_target==0)
		{
			angle_rol=Att_Angle.rol;
			angle_pit =Att_Angle.pit;
			updat_target=1;
		}
		if(balance_mark==1)
		{
			Control(&Att_Angle,&Gyr_rad);           //��̬����
		}else
		{
			Moto_Pwm(150,150,150,150);
		}
		delay_ms(10);
	}
}


static  void  Led_TASK(void *p_arg)
{
	u8 m;
	u8 q;
	u8 n=0;
	while(1)
	{
		
		
		q++;
		
		if(mark==1)
		{
			time_cnt++;
		}
		if(time_cnt>=10)
			time_mark=1;
		
		
		
		
	n=USART2_RX_BUF[9]-48;
		
			//�ƹ�ʶ�� G -R B
		if(state_none == 0 && q>=1)
		{
			if((n==1) && (run_mode==0))
			{
				macLED2_ON();
			}if((n==1) && (run_mode==1))
			{
macLED2_ON();
			}if((n==1) && (run_mode==2))
			{
				macLED2_ON();
			}
			else
			{
				macLED2_OFF();
			}
		}
		
		q %= 1;
		delay_ms(500);
	}
}

static  void  Bluetooth_TASK(void *p_arg)
{
	float a,b;
	s16 output;
	int target_x_j;


	while(1)
	{
			//------------------------����1-------------------------
		if((USART_RX_STA[0]&0x8000)!=0)
		{
			//----------------------��̬����------------------------
			if(USART1_RX_BUF[0]=='m')
			{
				balance_mark = !balance_mark;
				Moto_Pwm(150,150,150,150);
			}
			if(USART1_RX_BUF[0]=='p')
			{
				angle_rol= Att_Angle.rol;
				angle_pit =Att_Angle.pit;
			}
			//--------------------Ѳ�߿���----------------------
			if(USART1_RX_BUF[0]=='g')
			{
				mark=!mark;
				time_mark=0;
				time_cnt=0;
				alt_target=target_first;
				TIM1->CCR1=150;
				TIM1->CCR4=150;
			}

			if(USART1_RX_BUF[0]=='a')
			{
				alt_target+=2;
			}
			if(USART1_RX_BUF[0]=='b')
			{
				alt_target-=2;
			}
			USART_RX_STA[0]=0;
		}
		//----------------------����2-------------------------
		if(USART_RX_STA[1]&0x8000)
		{
      x=(USART2_RX_BUF[1]-'0')*100+(USART2_RX_BUF[2]-'0')*10+(USART2_RX_BUF[3]-'0');//λ�Ʋ�
			j=(USART2_RX_BUF[5]-'0')*100+(USART2_RX_BUF[6]-'0')*10+(USART2_RX_BUF[7]-'0');//�ǶȲ�
			//USART2_RX_BUF[9]:0-�� 1-Բ�� 2-�����Σ�
			if(x==0 && j == 0)
			{
				state_none=1;
				j=180;
			}
			else
				state_none=0;
			
			SortAver_Filter(x,&a,6);
			SortAver_Filter1(j,&b,6);
			
			b -= 90;
		 	a -= 90;
			
			if(a>=20 || a<=-20)
				target_x_j=30;
			else
				target_x_j=a;
			
			if(a >= 20)
				j=b+target_x_j;		
			else if(a <= -20)
				j=b-target_x_j;		
			else
				j=b+target_x_j/5;		
			 

			if(mark==1)																//Ѳ��
			{
				PID_Postion_Cal(&PID_Angle,-25,j);
				PID_Postion_Cal(&PID_Move,PID_Angle.OutPut,Gyr_rad.Z);
				
				output=PID_Move.OutPut;
				if(output>30)
				{
					output=30;
				}
				else if(output<-30)
				{
					output=-30;
				}

				TIM1->CCR1=140-output;
				TIM1->CCR4=140+output;
			}
		   USART_RX_STA[1]=0;
		}
		
		delay_ms(10);
	}
}






static  void  MS5837_TASK(void *p_arg)
{
	OS_ERR err;
	static u8 count_dat=0;
	while(1)
	{
		MS5837_30BA_GetData();
		OSSemPost(&ms5837_new,    //��ֵ�ź������ƿ�ָ��
							OS_OPT_POST_ALL,      //ѡ��
							&err);   //���ش�������

		count_dat++;
	
		if(j>=35 && run_mode==0 && Pressure < ((target_first+target_second)/2) && mark==1 && state_none == 0 && time_mark==1)
		{
			run_mode=1;				//��һ�׶���Ǳ
		}
		if(j>=35 && run_mode==1 && Pressure > ((target_first+target_second)/2) && mark==1 && state_none == 0 && time_mark==1)
		{
			run_mode=2;				//�ڶ��׶���Ǳ
		}

		if(run_mode==1)
		{
			if(count_dat==10 && alt_target!= target_second)
				alt_target += (target_second-target_first)/5;	
		}

		if(run_mode==2 )
		{
			if(count_dat==10 && alt_target!= target_third)
				alt_target += (target_third-target_second)/5;	
		}
			
		count_dat %= 10;
		delay_ms(50);
	}
}

