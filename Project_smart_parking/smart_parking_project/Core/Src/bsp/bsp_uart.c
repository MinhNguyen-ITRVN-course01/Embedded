/**
 * @file       bsp_uart.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This file is released under Fiot License.
 * @version    1.0.0
 * @date       2024-18-03
 * @author     Nguyen Minh 
 
 *
 * @brief      bsp for uart
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_uart.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
extern UART_HandleTypeDef huart2;
/* Private variables -------------------------------------------------- */
cbuffer_t cb;

/*use for cbuffer*/
uint8_t bsp_uart_rx_buffer[BSP_UART_RX_BUFFER_SIZE];

/*use for process data received in dma mode*/
uint8_t bsp_uart_rx_buffer_user[BSP_UART_RX_BUFFER_SIZE];
static uint8_t bsp_uart_rx_buffer_recept[BSP_UART_RX_BUFFER_SIZE];

/*pointer for process data in dma mode*/
static uint8_t *ptr_buffer_reception;
static uint8_t *ptr_buffer_user;
/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void bsp_uart_init(void)
{
    cb_init(&cb, bsp_uart_rx_buffer, BSP_UART_RX_BUFFER_SIZE);
}

void bsp_uart_transmit(UART_HandleTypeDef *huart, uint8_t *data)
{
    HAL_UART_Transmit(huart, data, strlen((char *)data), TIME_OUT_TRANSMIT_UART);
}

void bsp_uart_receive(UART_HandleTypeDef *huart, uint8_t *data)
{
    HAL_UART_Receive(huart, data, strlen((char *)data), TIME_OUT_TRANSMIT_UART);
}

void bsp_uart_receive_to_idle_dma(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
	ptr_buffer_reception = bsp_uart_rx_buffer_recept;
	ptr_buffer_user      = bsp_uart_rx_buffer_user;

	if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(huart, data, size))
	{
		Error_Handler();
	}
}

void bsp_uart_process_data(UART_HandleTypeDef *huart, uint16_t size)
{
	static uint16_t old_position = 0;
	uint8_t *ptr_temp;
	uint8_t i;

	/* Check if the number of data received in buffer had change */
	if(size != old_position)
	{
		/* Copy data received to buffer  */
		if(size < old_position)
		{

		}
	}

}
/* End of file -------------------------------------------------------- */
