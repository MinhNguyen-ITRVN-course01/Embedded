/**
 * @file       bsp_button.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024/12/03
 * @author     Nguyen Minh
 *             
 * @brief      BSP for button
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_button.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */
#define CHECK_ID_BUTTON_INIT(x) if (x > BUTTON_MAX || x < 1) return false;
/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */
/*
* @brief    Function check init button

* @param     button_id  ID button want to init
* @param     button     Pointer to button struct
* @param     port       Button port want to init
* @param     pin        Button pin want to init
* @return    true       Button init success
             false      Button init failure
*/
bool check_init_button(button_t *button, button_id_t button_id, GPIO_TypeDef *button_port, uint16_t button_pin);
/* Function definitions ----------------------------------------------- */
bool check_init_button(button_t *button, button_id_t button_id, GPIO_TypeDef *button_port, uint16_t button_pin)
{
    CHECK_ID_BUTTON_INIT(button_id);

    button->port = button_port;
    button->pin = button_pin;

    return true;
}
bool button_init(button_t *button) 
{
    if(check_init_button(&button[BUTTON_1 - 1u], BUTTON_1, BUTTON_1_PORT, BUTTON_1_PIN)
     &&check_init_button(&button[BUTTON_2 - 1u], BUTTON_2, BUTTON_2_PORT, BUTTON_2_PIN) == true)
     return true;
     else return false;
}

button_state_t button_update_state(button_t *button, button_id_t button_id, bool *interrupt_flag) 
{
    button_event_t event = RELEASE;
    switch (button->state) 
    {
        case BUTTON_STATE_IDLE:
            if (*interrupt_flag == true && (HAL_GPIO_ReadPin(button->port, button->pin) == GPIO_PIN_RESET)) 
            {
                button->state = BUTTON_STATE_PRESS;
                button->timeout = HAL_GetTick() + 50; //set debounce time is 50ms to detect press event
            }
            break;

        case BUTTON_STATE_PRESS:
            if ((HAL_GPIO_ReadPin(button->port, button->pin) == GPIO_PIN_RESET) && (HAL_GetTick() > button->timeout)) 
            {
                button->state = BUTTON_STATE_HOLD;
                
                event = PRESS; //return to PRESS event
                button->timeout = HAL_GetTick() + 1950; //set time is 2000 to detect hold event
            } 
            else if ((HAL_GPIO_ReadPin(button->port, button->pin) == GPIO_PIN_SET) && (HAL_GetTick() <= button->timeout)) 
            {
                button->state = BUTTON_STATE_IDLE;
            }
            break;

        case BUTTON_STATE_HOLD:
            if ((HAL_GPIO_ReadPin(button->port, button->pin) == GPIO_PIN_RESET) && (HAL_GetTick() > button->timeout)) 
            {
                button->state = BUTTON_STATE_IDLE;
                event = HOLD; //return to HOLD event
            }

            if ((HAL_GPIO_ReadPin(button->port, button->pin) == GPIO_PIN_RESET) && (HAL_GetTick() <= button->timeout)) 
            {
                button->state = BUTTON_STATE_PRESS;
            }

            if ((HAL_GPIO_ReadPin(button->port, button->pin) == GPIO_PIN_SET) && (HAL_GetTick() <= button->timeout)) 
            {
                button->state = BUTTON_STATE_IDLE;
            }

            break;

        default: button->state = BUTTON_STATE_IDLE
        break;
    }
    *interrupt_flag = false;
    return event;
}

/* End of file -------------------------------------------------------- */