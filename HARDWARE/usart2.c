/*******************************************************************************************
* 串口初始化
* 程序版本：V1.0
* 程序日期：2020-8-18
* 程序作者：徐杰
*******************************************************************************************/
#include "stm32f10x.h"
#include "usart.h"
#include "os.h"
extern u16 USART_RX_STA[2];       //接收状态标记	  
u8 USART2_RX_BUF[USART_REC_LEN];

/*****************************************************************************
* 函  数：void usart2_init(uint32_t baudrate)
* 功  能: 串口2的初始化
* 参  数: bud 波特率
* 返回值：无
* 备  注: 无
*****************************************************************************/
void usart2_init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;

	// 外设使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2); //复位串口2 -> 可以没有

	// 初始化 串口对应IO口 TX-PA2 RX-PA3
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);

	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStrue);

	// 初始化 串口模式状态
	USART_InitStrue.USART_BaudRate=bound; // 波特率
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // 硬件流控制
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // 发送 接收 模式都使用
	USART_InitStrue.USART_Parity=USART_Parity_No; // 没有奇偶校验
	USART_InitStrue.USART_StopBits=USART_StopBits_1; // 一位停止位
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; // 每次发送数据宽度为8位
	USART_Init(USART2,&USART_InitStrue);

	USART_Cmd(USART2,ENABLE);//使能串口
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启接收中断

	// 初始化 中断优先级
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);

}
/*****************************************************************************
* 函  数：void usart_send(uint8_t *data,uint8_t len)
* 功  能: 串口发送函数
* 参  数: data-发送数据，len-字长
* 返回值：无
* 备  注: 无
*****************************************************************************/

void usart2_send(uint8_t *data,uint8_t len)
{
		 
}

void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res;
	
	#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
		OSIntEnter();    
	#endif
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART2);	//读取接收到的数据
		
		if((USART_RX_STA[1]&0x8000)==0)//接收未完成
		{

			if(Res=='E')USART_RX_STA[1]|=0x8000;
			else
			{
				USART2_RX_BUF[USART_RX_STA[1]&0X3FFF]=Res ;
				USART_RX_STA[1]++;
				if(USART_RX_STA[1]>(200-1))USART_RX_STA[1]=0;//接收数据错误,重新开始接收	  
			}		 
			
		}
   } 	
	
	#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
		OSIntExit();  											 
	#endif	
	 
} 



