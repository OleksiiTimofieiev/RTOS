/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include <stdio.h>
#include <string.h>

#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

TaskHandle_t * xTaskHandle1 = NULL;
TaskHandle_t * xTaskHandle2 = NULL;

#ifdef USE_SEMIHOSTING
// to be used: -specs=rdimon.specs -lc -lrdimon in lincer/misc
	extern void initialise_monitor_handles();
#endif

static void	prvSetupHardware(void);
void		printMsg(char *msg);

// macros
#define TRUE 1
#define FALSE 2
#define AVAILABLE TRUE
#define NON_AVAILABLE FALSE

#define PRESSED TRUE
#define NOT_PRESSED FALSE

// Global variable section
char usr_msg[250] = {0};
uint8_t UART_ACCESS_KEY = AVAILABLE;

void led_task_handler(void *params);
void button_task_handler(void *params);

uint8_t button_status_flag = NOT_PRESSED;

int main(void)
{
#ifdef USE_SEMIHOSTING
	initialise_monitor_handles();
#endif

	// 1,  RCC set to the default state ==> SCC - 16 MHz;
	RCC_DeInit();

	// 2. Update system SystemCoreClock variable
	SystemCoreClockUpdate();

	prvSetupHardware();

	sprintf(usr_msg, "Hello World\r\n");
	printMsg(usr_msg);
//	screen /dev/tty.usbmodem143103 115200

	// 3. Create tasks;
	xTaskCreate(led_task_handler, "LED-TASK", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(button_task_handler, "BUTTON-TASK", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	// 4, start the scheduler
	vTaskStartScheduler();

	// will never be called if scheduler works fine
	for(;;);
}

void led_task_handler(void *params)
{
	while (1)
	{
		if (button_status_flag == PRESSED)
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		}
		else
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
		}
	}
}

void button_task_handler(void *params)
{
	while (1)
	{
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
		{
			// !pressed
			printMsg("NOT PRESSED\r\n");
			button_status_flag = NOT_PRESSED;
		}
		else
		{
			// pressed
			printMsg("PRESSED\r\n");
			button_status_flag = PRESSED;
		}
	}
}

static void	prvSetupUART(void)
{
	// 1. Enable UART2 peripheral clock and GPIOA - rcc.c - APB == communication bus
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// PA2 is UART2_TX, PA3 is UART2_RX
	// 2. Alternate function configuration of of MCU PIN to behave uart tx/rx
	GPIO_InitTypeDef gpio_uart_pins;

	memset(&gpio_uart_pins, 0, sizeof(gpio_uart_pins));

	gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF; // alternate function mode
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP; // idle -> logical up

	GPIO_Init(GPIOA, &gpio_uart_pins);

	// 3. AF mode settings for the pins;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // PA2 as Tx
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // PA3 as Rx

	// 4. UART parameters initialisation
	USART_InitTypeDef uart2_init;

	memset(&uart2_init, 0, sizeof(uart2_init));

	uart2_init.USART_BaudRate = 115200;
	uart2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart2_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	uart2_init.USART_Parity = USART_Parity_No;
	uart2_init.USART_StopBits = USART_StopBits_1;
	uart2_init.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2, &uart2_init);

	// 5. Enable UART 2 peripheral;
	USART_Cmd(USART2, ENABLE);

	// listen on Mac
	//minicom -D /dev/tty.usbmodem141103 -b 115200
}

void prvSetupGpio(void)
{
	// this function is board specific
	//Peripheral clock enable for GPIOA and GPIOC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

	GPIO_InitTypeDef led_init, button_init;

	led_init.GPIO_Mode = GPIO_Mode_OUT;
	led_init.GPIO_OType = GPIO_OType_PP;
	led_init.GPIO_Pin = GPIO_Pin_5;
	led_init.GPIO_Speed = GPIO_Low_Speed;
	led_init.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA, &led_init);

	button_init.GPIO_Mode = GPIO_Mode_IN;
	button_init.GPIO_OType = GPIO_OType_PP;
	button_init.GPIO_Pin = GPIO_Pin_13;
	button_init.GPIO_Speed = GPIO_Low_Speed;
	button_init.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOC, &button_init);
}


static void	prvSetupHardware(void)
{
	// UART2
	prvSetupUART();

	// GPIO - LED
	prvSetupGpio();
}

void		printMsg(char *msg)
{
	for (uint32_t i = 0; i < strlen(msg); i++)
	{
		// wait finish of writing to the bus;
		while( USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET );
		USART_SendData(USART2, msg[i]);
	}
	// wait till data register is empty another byte can be loaded
}
