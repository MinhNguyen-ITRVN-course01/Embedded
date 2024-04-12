/**
 * @file       bsp_gpio.h
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

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

/* Includes ----------------------------------------------------------- */
#include "main.h"
#include "stdbool.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief This function set pin gpio
 *
 * @param bsp_gpio_port     gpio port
 * @param bsp_gpio_pin      gpio pin
 */
void bsp_gpio_set_pin(GPIO_TypeDef *bsp_gpio_port, uint16_t bsp_gpio_pin);

/**
 * @brief This function reset pin gpio
 *
 * @param bsp_gpio_port     gpio port
 * @param bsp_gpio_pin      gpio pin
 */
void bsp_gpio_reset_pin(GPIO_TypeDef *bsp_gpio_port, uint16_t bsp_gpio_pin);

/**
 * @brief This function read pin GPIO
 * 
 * @param bsp_gpio_port  gpio_port
 * @param bsp_gpio_pin   gpio_pin
 * @return true 
 * @return false 
 */
bool bsp_gpio_read_pin(GPIO_TypeDef *bsp_gpio_port, uint16_t bsp_gpio_pin);

#endif  // __BSP_GPIO_H

/* End of file -------------------------------------------------------- */
