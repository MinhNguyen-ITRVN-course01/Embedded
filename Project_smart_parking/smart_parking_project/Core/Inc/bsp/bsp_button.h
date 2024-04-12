/**
 * @file       bsp_button.h
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024/12/03
 * @author     Minh Nguyen
   
 * @brief      BSP for button     
 */
/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BUTTON_H
#define __BUTTON_H

/* Includes ----------------------------------------------------------- */
#include "stm32f4xx_hal.h"

#include <stdbool.h>
#include <stdint.h>
/* Public defines ----------------------------------------------------- */
#define BUTTON_1_PORT       GPIOA
#define BUTTON_2_PORT       GPIOB
#define BUTTON_1_PIN        GPIO_PIN_0
#define BUTTON_2_PIN        GPIO_PIN_1
#define BUTTON_MAX          (2)
/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief   enum for button status
 */
typedef enum {
    BUTTON_STATE_IDLE = 0xA0u,
    BUTTON_STATE_PRESS,
    BUTTON_STATE_RELEASE,
    BUTTON_STATE_HOLD
} button_state_t;

/**
 * @brief   enum for button event
 */
typedef enum {
    PRESS = 0x10u,
    HOLD,
    RELEASE
} button_event_t;

/**
 * @brief   enum for button id
 */
typedef enum {
    BUTTON_1 = 0x01u,
    BUTTON_2
} button_id_t;

/**
 * @brief   struct for button information
 */
typedef struct 
{
    GPIO_TypeDef *port;
    uint16_t pin;
    button_state_t state;
    uint32_t timeout;
} button_t;


/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  Initialize button
 *
 * @param     button  Pointer to button struct
 * @param     port    button port
 * @param     pin     button pin
 * @return  
 *  - 0: init success
 *  - 1: init failure
 */
bool button_init(button_t *button);

/**
 * @brief  Update button status
 *
 * @param     button            Pointer to button struct
 * @return    button_state_t    Button status
 */
button_state_t button_update_state(button_t *button, button_id_t button_id, bool *interrupt_flag);

#endif // __BUTTON_H

/* End of file -------------------------------------------------------- */