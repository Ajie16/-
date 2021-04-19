#ifndef __LED_H_
#define	__LED_H_



#include "stm32f10x.h"
 
 #include "os.h"
 
/****************************** LED 引脚配置参数定义***************************************/
#define             macLED1_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macLED1_GPIO_CLK                        RCC_APB2Periph_GPIOC
#define             macLED1_GPIO_PORT                       GPIOB
#define             macLED1_GPIO_PIN                        GPIO_Pin_8

#define             macLED2_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macLED2_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             macLED2_GPIO_PORT                       GPIOB
#define             macLED2_GPIO_PIN                        GPIO_Pin_9

#define             macLED3_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macLED3_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             macLED3_GPIO_PORT                       GPIOB
#define             macLED3_GPIO_PIN                        GPIO_Pin_15

#define             RGB_GPIO_APBxClock_FUN             			RCC_APB2PeriphClockCmd
#define             RGB_GPIO_CLK                        		RCC_APB2Periph_GPIOA
#define             RGB_GPIO_PORT                       		GPIOA
#define             RGB_GPIO_PIN                        		GPIO_Pin_12

#define             RGB_GPIO_APBxClock_FUN            			RCC_APB2PeriphClockCmd
#define             RGB_GPIO_CLK2                        		RCC_APB2Periph_GPIOA
#define             RGB_GPIO_PORT2                       		GPIOA
#define             RGB_GPIO_PIN2                        		GPIO_Pin_5


/****************************** LED 函数宏定义***************************************/
#define             macLED1_ON()                            GPIO_ResetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN )
#define             macLED1_OFF()                           GPIO_SetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN )
#define             macLED1_TOGGLE()                        GPIO_ReadOutputDataBit ( macLED1_GPIO_PORT, macLED1_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN ) : GPIO_SetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN )

#define             macLED2_ON()                            GPIO_ResetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN )
#define             macLED2_OFF()                           GPIO_SetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN )
#define             macLED2_TOGGLE()                        GPIO_ReadOutputDataBit ( macLED2_GPIO_PORT, macLED2_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN ) : GPIO_SetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN )

#define             macLED3_ON()                            GPIO_ResetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN )
#define             macLED3_OFF()                           GPIO_SetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN )
#define             macLED3_TOGGLE()                        GPIO_ReadOutputDataBit ( macLED3_GPIO_PORT, macLED3_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN ) : GPIO_SetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN )

#define 						RGB_LED 				GPIO_Pin_12
#define							RGB_LED_HIGH		(GPIO_SetBits(GPIOA,RGB_LED))
#define 						RGB_LED_LOW			(GPIO_ResetBits(GPIOA,RGB_LED))

#define 						RGB_LED2 				GPIO_Pin_5
#define							RGB_LED_HIGH2		(GPIO_SetBits(GPIOA,RGB_LED2))
#define 						RGB_LED_LOW2		(GPIO_ResetBits(GPIOA,RGB_LED2))

/************************** PAD 函数声明********************************/
void                               LED_Init                         ( void );
void 															 WS0_delay2us(u8 i);														//当i=4时，即2us
void 															 RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue);

void 															 RGB_LED_Write_24Bits_2(uint8_t green,uint8_t red,uint8_t blue);

void 												delay_ns(u32 nTime);
void 												delayns_init(void);
#endif /* __LED_H_ */

