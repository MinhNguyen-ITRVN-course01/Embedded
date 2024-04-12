/**
 * @file       bsp_gpio.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This file is released under Fiot License.
 * @version    v1.0.0
 * @date       2024-22-03
 * @author     Nguyen Minh
 *
 * @brief      bsp for GPIO
 *
 * @note
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_gpio.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */

void bsp_gpio_set_pin(GPIO_TypeDef *bsp_gpio_port, uint16_t bsp_gpio_pin)
{
    HAL_GPIO_WritePin(bsp_gpio_port, bsp_gpio_pin, GPIO_PIN_SET);
}

void bsp_gpio_reset_pin(GPIO_TypeDef *bsp_gpio_port, uint16_t bsp_gpio_pin)
{
    HAL_GPIO_WritePin(bsp_gpio_port, bsp_gpio_pin, GPIO_PIN_RESET);
}

bool bsp_gpio_read_pin(GPIO_TypeDef *bsp_gpio_port, uint16_t bsp_gpio_pin)
{
    return HAL_GPIO_ReadPin(bsp_gpio_port, bsp_gpio_pin) == GPIO_PIN_SET ? true : false;
}

/* End of file -------------------------------------------------------- */
