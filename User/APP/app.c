#include <includes.h>

/**************************************************************
*                       定义任务控制块
**************************************************************/
static  OS_TCB   AppTaskStartTCB;

static  OS_TCB   OLED_TASK_TCB;
static  OS_TCB   Control_TASK_TCB;
/**************************************************************
*                      定义任务栈
**************************************************************/
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK  OLED_TASK_Stk[OLED_TASK_SIZE];
static  CPU_STK  Control_TASK_Stk[Control_TASK_SIZE];
/*************************************************************
*                    函数声明
*************************************************************/
static  void  AppTaskStart  (void *p_arg);

static  void  OLED_TASK(void *p_arg);
static  void  Control_TASK(void *p_arg);

int  main (void)
{
    OS_ERR  err;

    /* 初始化系统*/
    OSInit(&err);                                               

    /* 创建任务 */
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
                 
    /* 开启调度*/
    OSStart(&err);                                              
		
		
}


/***********************************************************************
  * @ 函数名  ： AppTaskStart
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
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
     
    /* 内存初始化*/
    Mem_Init();                                                
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               
#endif

    CPU_IntDisMeasMaxCurReset();

		BSP_Init();   
		
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

		OSTaskDel(&AppTaskStartTCB,&err);

		//报错死循环
    while (DEF_TRUE) 
    {                                          
			macLED1_TOGGLE ();
			delay_ms(100);
			OLED_ShowString(40,0,"WARNING",16);
			OLED_ShowString(40,16,"WARNING",16);
			OLED_ShowString(40,32,"WARNING",16);
			OLED_Refresh_Gram();
    }
}

static  void  OLED_TASK(void *p_arg)
{
	while(1)
	{
		OLED_ShowString(12,0,"UCOS-3 Runing",16);
		//数据打印
		OLED_ShowString(0,16,"X:",16);
		OLED_ShowString(42,16,"Y:",16);
		OLED_ShowString(82,16,"Z:",16);
		OLED_ShowString(0,32,"CPU_Using:  .  %",16);
		
		OLED_ShowNum(16,16,Att_Angle.pit,3,16);
		OLED_ShowNum(56,16,Att_Angle.rol,3,16);
		OLED_ShowNum(96,16,Att_Angle.yaw,3,16);
		OLED_ShowNum(80,32,OSStatTaskCPUUsage/100,2,16);
		OLED_ShowNum(104,32,OSStatTaskCPUUsage%100,2,16);
		
		OLED_Refresh_Gram();
		delay_ms(100);
	}
}

static  void  Control_TASK(void *p_arg)
{
	while(1)
	{
		Prepare_Data(); //获取姿态解算所需数据
		IMUupdate(&Gyr_rad,&Acc_filt,&Att_Angle); //四元数姿态解算
		delay_ms(50);
	}
}

