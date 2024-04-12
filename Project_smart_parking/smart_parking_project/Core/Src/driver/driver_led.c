/*
 * @file       driver_led.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024-03-13
 * @author     Nguyen Minh
 *             
 * @brief      driver for led
*/
/* Includes ----------------------------------------------------------- */
#include "driver_led.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */

void led_init(led_t *led) 
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(led == NULL) 
    {
        return; // returns if the led pointer is invalid
    }

    // Initialize GPIO configuration
    GPIO_InitStruct.Pin = led->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(led->port, &GPIO_InitStruct);

    // Set LED enbale mode
    if(led->mode == LED_ENABLE_LOW) 
    {
        led_set(led, LED_VALUE_OFF);
    } 
    else 
    {
        led_set(led, LED_VALUE_ON);
    }
}

void led_set(led_t *led, led_value_t value) 
{
    if(led == NULL) 
    {
        return; // returns if the led pointer is invalid
    }

    if(value == LED_VALUE_ON) 
    {
        HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET); // set pin LED high
    } 
    else if(value == LED_VALUE_OFF) 
    {
        HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET); // set pin LED low
    } 
    else if(value == LED_VALUE_TOGGLE) 
    {
        HAL_GPIO_TogglePin(led->port, led->pin); // toggle LED
    }
}

led_status_t led_get(led_t *led) 
{
    if(led == NULL) 
    {
        return LED_STATUS_NULL; // return LED_STATUS_NULL if LED pointer invalid
    }

    GPIO_PinState pin_state = HAL_GPIO_ReadPin(led->port, led->pin);

    // check pin LED and return status
    if(pin_state == GPIO_PIN_SET) 
    {
        return LED_STATUS_OK;
    } 
    else 
    {
        return LED_STATUS_ERROR;
    }
}

/* End of file -------------------------------------------------------- */
