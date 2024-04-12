/**
 * @file       driver_ds1307.h
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.1
 * @date       30/01/2024
 * @author     Nguyen Minh
 *             
 * @brief      Driver for RTC DS1307           
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DRIVER_DS1307_H
#define __DRIVER_DS1307_H

/* Includes ----------------------------------------------------------- */
#include "main.h"
#include <stdbool.h>
/* Public defines ----------------------------------------------------- */
#define DS1307_I2C_ADDR        0x68
#define DS1307_REG_SECOND      0x00
#define DS1307_REG_MINUTE      0x01
#define DS1307_REG_HOUR        0x02
#define DS1307_REG_DOW         0x03
#define DS1307_REG_DATE        0x04
#define DS1307_REG_MONTH       0x05
#define DS1307_REG_YEAR        0x06
#define DS1307_REG_CONTROL     0x07
#define DS1307_REG_UTC_HR      0x08
#define DS1307_REG_UTC_MIN     0x09
#define DS1307_REG_CENT        0x10
#define DS1307_TIMEOUT         1000

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief struct for time of DS1307
 */
typedef struct
{
    uint8_t     ds1307_sec;
    uint8_t     ds1307_min;
    uint8_t     ds1307_hour;
    uint8_t     ds1307_dow;
    uint8_t     ds1307_date;
    uint8_t     ds1307_month;
    uint16_t    ds1307_year;
} ds1307_t;

/**
 * @brief struct to set time of DS1307
 */
typedef struct
{
    uint8_t    set_ds1307_sec;
    uint8_t    set_ds1307_min;
    uint8_t    set_ds1307_hour;
    uint8_t    set_ds1307_dow;
    uint8_t    set_ds1307_date;
    uint8_t    set_ds1307_month;
    uint16_t   set_ds1307_year;
    uint8_t    set_ds1307_hour_24mode;
    uint8_t    set_ds1307_day_of_week;
} set_ds1307_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */
extern I2C_HandleTypeDef hi2c1;
/* Public function prototypes ----------------------------------------- */
/**
 * @brief  This function decodes binary values read from DS1307 registers
		   into decimal format for accessing time and date information.
 *
 * @param     ds1307_bin  <the binary value to be decoded into decimal format>
 * @return		decimal value
 */
uint8_t ds1307_decode_bin(uint8_t ds1307_bin);

/**
 * @brief  This function decodes decimal values into binary format for writing to DS1307 registers.
 *
 * @param       ds1307_dec  <the decimal value to be decoded into binary format>
 *
 * @return		binary value
 */
uint8_t ds1307_decode_dec(uint8_t ds1307_dec);

/**
 * @brief  This function is used to enable or disable the clock on the DS1307.
 *
 * @param       ds1307_halt  <Value of 0 to enable the clock, other values to disable>
 * @return		None
 */
void ds1307_setclock(uint8_t ds1307_halt);

/**
 * @brief  This function is used to set values into the registers of the DS1307.
 *
 * @param       ds1307_reg_addr  <Address of the register to set the value>
 * @param       ds1307_val       <Value to be set into the register>
 *
 * @return		None
 */
void ds1307_set_reg_byte(uint8_t ds1307_reg_addr, uint8_t ds1307_val);

/**
 * @brief  This function is used to get the clock status of DS1307.
 *
 * @return		Clock status
 */
uint8_t ds1307_getclock(void);

/**
 * @brief  This function is used to read values from the DS1307 register.
 *
 * @param         ds1307_reg_addr  <Address of the register to read>
 *
 * @return		  Value of the register
 */
uint8_t ds1307_get_reg_byte(uint8_t ds1307_reg_addr);

/**
 * @brief  This function is used to initialize DS1307.
 */
void ds1307_init();

/**
 * @brief  This function is used to get time from DS1307.
 
 */
void ds1307_gettime(ds1307_t *ptr_gettime);

/**
 * @brief  This function is used to set time into DS1307.
 *
 * @param[in]     sec        	<Second value>
 * @param[in]     min        	<Minute value>
 * @param[in]     hour_24mode	<Hour value in 24-hour mode>
 * @param[in]     day_of_week  	<Day of the week value>
 * @param[in]     date       	<Date value>
 * @param[in]     month      	<Month value>
 * @param[in]     year       	<Year value>
 * 
 * @return
 *        0: Failure
 *        1: Success
 */
bool ds1307_settime(set_ds1307_t *ptr_settime);

#endif // __DRIVER_DS1307_H

/* End of file -------------------------------------------------------- */
