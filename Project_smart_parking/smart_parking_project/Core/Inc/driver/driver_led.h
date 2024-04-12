/**
 * @file       driver_led.h
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024/03/13
 * @author     Minh Nguyen
 *             
 * @brief      Driver for led
 *             
 * @note          
 * @example    
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __LED_H
#define __LED_H

/* Includes ----------------------------------------------------------- */
#include"main.h"
#include <stdio.h>
#include <stdint.h>
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief status of led
 */
typedef enum 
{
  LED_STATUS_OK = 0, 
  LED_STATUS_ERROR, 
  LED_STATUS_NULL
} led_status_t;

/**
* @brief value of led
 */
typedef enum 
{
  LED_VALUE_OFF, 
  LED_VALUE_ON,
  LED_VALUE_TOGGLE
} led_value_t;

/**
* @brief mode enable led
 */
typedef enum 
{
  LED_ENABLE_LOW, 
  LED_ENABLE_HIGH
} led_enbale_mode_t;

/**
 * @brief struct to store information
 */
typedef struct 
{
  GPIO_TypeDef *port;     // Port LED
  uint16_t pin;           // Pin LED
  led_enbale_mode_t mode; // Mode enable LED
} led_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  Initialize LED
 *
 * @param  led: Pointer to struct led_t
 * @retval None
 */
void led_init(led_t *led);

/**
 * @brief  Set status for led
 *
 * @param  led: pointer to struct led_t
 * @param[in]  value: set value led (LED_VALUE_ON, LED_VALUE_OFF or LED_VALUE_TOGGLE)
 * @retval None
 */
void led_set(led_t *led, led_value_t value);

/**
 * @brief  get status of LED
 *
 * @param[in]  led: pointer to struct led_t
 * @retval status of LED (LED_STATUS_OK, LED_STATUS_ERROR or LED_STATUS_NULL)
 */
led_status_t led_get(led_t *led);

#endif // __CODE_TEMPLATE_H

/* End of file -------------------------------------------------------- */
