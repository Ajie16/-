#include "bsp_led.h"   
#include "delay.h"
#include "os.h"


static void                         LED_GPIO_Config                  ( void );



 /**
  * @brief  配置 LED 的 GPIO 功能
  * @param  无
  * @retval 无
  */
static void LED_GPIO_Config ( void )
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 配置 LED1 引脚 */
	RCC_APB2PeriphClockCmd ( macLED1_GPIO_CLK, ENABLE ); 		
	
	GPIO_InitStructure.GPIO_Pin = macLED1_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init ( macLED1_GPIO_PORT, & GPIO_InitStructure );	

	/* 配置 LED2 引脚 */
	RCC_APB2PeriphClockCmd ( macLED2_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = macLED2_GPIO_PIN;	
	GPIO_Init ( macLED2_GPIO_PORT, & GPIO_InitStructure );	
	
	/* 配置 RGB 引脚 */
	RCC_APB2PeriphClockCmd ( RGB_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RGB_GPIO_PIN;	
	GPIO_Init ( RGB_GPIO_PORT, & GPIO_InitStructure );	
	
	RCC_APB2PeriphClockCmd ( RGB_GPIO_CLK2, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = RGB_GPIO_PIN2;	
	GPIO_Init ( RGB_GPIO_PORT2, & GPIO_InitStructure );	
	  		
}


 /**
  * @brief  LED 初始化函数
  * @param  无
  * @retval 无
  */
void LED_Init ( void )
{
  LED_GPIO_Config ();
	
	macLED1_OFF();
	macLED2_OFF();
	
}
/********************************************************/
//
/********************************************************/
void RGB_LED_Write0(void)
{
	RGB_LED_HIGH;
	__nop();__nop();__nop();__nop();__nop();__nop();
	RGB_LED_LOW;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

/********************************************************/
//
/********************************************************/

void RGB_LED_Write1(void)
{
	RGB_LED_HIGH;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
	RGB_LED_LOW;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

void RGB_LED_Reset(void)
{
	RGB_LED_LOW;
	delay_us(80);
}

void RGB_LED_Write_Byte(uint8_t byte)
{
	uint8_t i;

	for(i=0;i<8;i++)
		{
			if(byte&0x80)
			{
					RGB_LED_Write1();
			}
			else
			{
					RGB_LED_Write0();
			}
			byte <<= 1;
	}
}

/********************************************************/
//
/********************************************************/
void RGB_LED_Write0_2(void)
{
	RGB_LED_HIGH2;
	__nop();__nop();__nop();__nop();__nop();__nop();
	RGB_LED_LOW2;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

/********************************************************/
//
/********************************************************/

void RGB_LED_Write1_2(void)
{
	RGB_LED_HIGH2;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
	RGB_LED_LOW2;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

void RGB_LED_Reset_2(void)
{
	RGB_LED_LOW2;
	delay_us(80);
}

void RGB_LED_Write_Byte2(uint8_t byte)
{
	uint8_t i;

	for(i=0;i<8;i++)
		{
			if(byte&0x80)
			{
					RGB_LED_Write1_2();
			}
			else
			{
					RGB_LED_Write0_2();
			}
			byte <<= 1;
	}
}

void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue)
{
	RGB_LED_Write_Byte(green);
	RGB_LED_Write_Byte(red);
	RGB_LED_Write_Byte(blue);
}


void RGB_LED_Write_24Bits_2(uint8_t green,uint8_t red,uint8_t blue)
{
	OSIntEnter();
	RGB_LED_Write_Byte2(green);
	RGB_LED_Write_Byte2(red);
	RGB_LED_Write_Byte2(blue);
	OSIntExit();
}


/*********************************************END OF FILE**********************/



