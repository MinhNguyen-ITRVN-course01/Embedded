/**
 * @file       bsp_uart.c
 * @copyright  Copyright (C) 2019 Fiot Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
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

/*use for buffer*/
uint8_t bsp_uart_rx_buffer[BSP_UART_RX_BUFFER_SIZE];

/*use for process data received in dma mode*/
uint8_t bsp_uart_rx_buffer_user[BSP_UART_RX_BUFFER_SIZE];
static uint8_t bsp_uart_rx_buffer_1[BSP_UART_RX_BUFFER_SIZE];
static uint8_t bsp_uart_rx_buffer_2[BSP_UART_RX_BUFFER_SIZE];
/*number of receive chars*/
__IO uint32_t num_received_char;

typedef void (*bootloader_handle_error_t)(void);
bootloader_handle_error_t bsp_uart_bootloader_error;
/*pointer for process data in dma mode*/
static uint8_t *ptr_buffer_reception;
static uint8_t *ptr_buffer_user;
/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void bsp_uart_init(void)
{
    cb_init(&cb, bsp_uart_rx_buffer, BSP_UART_RX_BUFFER_SIZE);
}

void bsp_uart_transmit(UART_HandleTypeDef *huart, unint8_t *data)
{
    HAL_UART_Transmit(huart, data, strlen((char *)data), TIME_OUT_TRANSMIT_UART);
}

void bsp_uart_receive(UART_HandleTypeDef *huart, unint8_t *data)
{
    HAL_UART_Receive(huart, data, strlen((char *)data), TIME_OUT_TRANSMIT_UART);
}


/* End of file -------------------------------------------------------- */
