/**
 * @file       bsp_uart.h
 * @copyright  Copyright (C) 2019 Fiot Co., Ltd. All rights reserved.
 * @license    This project is released under the QuyLe License.
 * @version    1.0.0
 * @date       2024-03-17
 * @author     Nguyen Minh 
 *
 * @brief      BSP for uart
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BSP_USART_H
#define BSP_USART_H
/* Includes ----------------------------------------------------------- */
#include "cbuffer.h"
#include "main.h"
#include <string.h>
/* Public defines ---------------------------------------------------- */
#define TIME_OUT_TRANSMIT_UART          1000
#define TRUE                            1
#define FALSE                           0
#define BSP_UART_RX_BUFFER_SIZE         2000
/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  Initialize uart
 *
 * @param[in]     
 * @param[out]    
 */
void bsp_uart_init(void);

/**
 * @brief  Transmit data to uart
 *
 * @param    *huart     Pointer to uart transmit data
 * @param    *data      Pointer to string data
 */
void bsp_uart_transmit(UART_HandleTypeDef *huart, unint8_t *data);

/**
 * @brief  Receive data from uart
 *
 * @param    *huart     Pointer to uart transmit data
 * @param    *data      Pointer to string data
 */
void bsp_uart_receive(UART_HandleTypeDef *huart, unint8_t *data);

/**
 * @brief  Receive data from uart DMA
 *
 * @param    *huart     Pointer to uart transmit data
 * @param    *data      Pointer to string data
 * @param    size       Size of data
 */
void bsp_uart_receive_to_idle_dma(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);

/**
 * @brief  Handle data
 *
 * @param    *huart     Pointer to uart transmit data
 * @param    *data      Pointer to string data
 */
void bsp_uart_process_data(UART_HandleTypeDef *huart, uint16_t size);

#endif
/* End of file -------------------------------------------------------- */
