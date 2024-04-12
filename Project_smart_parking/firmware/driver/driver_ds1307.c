/**
 * @file       driver_ds1307.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.1
 * @date       2024/03/16
 * @author     Nguyen Minh
 
 *             
 * @brief      Driver for RTC DS1307
 *             
 */

/* Includes ----------------------------------------------------------- */
#include "driver_ds1307.h"
/* Private defines ---------------------------------------------------- */
#define bool CHECK_LEAP_YEAR(x) \
    if (x % 4 == 0)             \             
    return true

#define bool CHECK_DATE_OF_FEB(x)   \ 
    if(x == 28 || x == 29)          \ 
    return true

#define bool CHECK_DATE_OF_MONTH_30(x)  \
    if(x <= 30)                          \
    return true                        \
/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
I2C_HandleTypeDef hi2c1;
/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */
/**
 * @brief  Function to check time range to be set for DS1307
 *
 * @param     *ptr_check  pointer to struct set_ds1307_t
 *
 * @return  
 *  - 0: Success
 *  - 1: Failure
 */
static bool check_time_range_ds1307(set_ds1307_t *ptr_check);
/* Function definitions ----------------------------------------------- */
static bool check_time_range_ds1307(set_ds1307_t *ptr_check)
{
    if(ptr_check->set_ds1307_sec < 0 || ptr_check->set_ds1307_sec > 59 || ptr_check->set_ds1307_min < 0 || ptr_check->set_ds1307_min > 59 || ptr_check->set_ds1307_hour < 0 || ptr_check->set_ds1307_hour > 24 || ptr_check->set_ds1307_date < 0 || ptr_check->set_ds1307_date > 31 || ptr_check->set_ds1307_month < 0 || ptr_check->set_ds1307_month > 12) 
        return false;
    
    if((ptr_check->set_ds1307_month == 4 || ptr_check->set_ds1307_month == 6 || ptr_check->set_ds1307_month == 9 || ptr_check->set_ds1307_month == 11) && CHECK_DATE_OF_MONTH_30(!ptr_check->set_ds1307_date))
        return false;

    if(CHECK_LEAP_YEAR(ptr_check->set_ds1307_year) == true && ptr_check->set_ds1307_month == 2 && CHECK_DATE_OF_FEB(!ptr_check->set_ds1307_date)) 
        return false;

void ds1307_gettime(ds1307_t *ptr_gettime)
{
    uint16_t ds1307_cen;
    ptr_gettime->ds1307_sec = ds1307_decode_bin(ds1307_get_reg_byte(DS1307_REG_SECOND) & 0x7f);
    ptr_gettime->ds1307_min = ds1307_decode_bin(ds1307_get_reg_byte(DS1307_REG_MINUTE));
    ptr_gettime->ds1307_hour = ds1307_decode_bin(ds1307_get_reg_byte(DS1307_REG_HOUR) & 0x3f);
    ptr_gettime->ds1307_dow = ds1307_decode_bin(ds1307_get_reg_byte(DS1307_REG_DOW));
    ptr_gettime->ds1307_date = ds1307_decode_bin(ds1307_get_reg_byte(DS1307_REG_DATE));
    ptr_gettime->ds1307_month = ds1307_decode_bin(ds1307_get_reg_byte(DS1307_REG_MONTH));
    ds1307_cen =_ds1307_get_reg_byte(DS1307_REG_CENT) * 100;
    ptr_gettime->ds1307_year = ds1307_decode_bin(ds1307_get_reg_byte(DS1307_REG_YEAR)) + ds1307_cen;
}

void ds1307_set_reg_byte(uint8_t ds1307_reg_addr, uint8_t ds1307_val)
{
    uint8_t ds1307_bytes[2] = { ds1307_reg_addr, ds1307_val };
    HAL_I2C_Master_Transmit(&hi2c1, DS1307_I2C_ADDR << 1, ds1307_bytes, 2, DS1307_TIMEOUT);
}

uint8_t ds1307_getclock(void)
{
    return (ds1307_get_reg_byte(DS1307_REG_SECOND) & 0x80) >> 7;
}

bool ds1307_settime(set_ds1307_t *ptr_settime)
{
    if(check_time_range_ds1307(set_ds1307_t *ptr_check) == true)
    {
    ds1307_set_reg_byte(DS1307_REG_SECOND, ds1307_decode_dec(ptr_settime->set_ds1307_sec | ds1307_getclock()));
    ds1307_set_reg_byte(DS1307_REG_MINUTE, ds1307_decode_dec(ptr_settime->set_ds1307_min));
    //hour_24mode Hour in 24h format, 0 to 23.
    ds1307_set_reg_byte(DS1307_REG_HOUR, ds1307_decode_dec(ptr_settime->set_ds1307_hour_24mode & 0x3f));
    //day_of_week Days since last Sunday, 0 to 6.
    ds1307_set_reg_byte(DS1307_REG_DOW, ds1307_decode_dec(ptr_settime->set_ds1307_day_of_week));
    //date Day of month, 1 to 31, 30, 28 or 29
    ds1307_set_reg_byte(DS1307_REG_DATE, ds1307_decode_dec(ptr_settime->set_ds1307_date)); 
    //set Month, 1 to 12.
    ds1307_set_reg_byte(DS1307_REG_MONTH, ds1307_decode_dec(ptr_settime->set_ds1307_month));
    ds1307_set_reg_byte(DS1307_REG_CENT, ptr_settime->set_ds1307_year / 100);
    //set year 2000 to 2099.
    ds1307_set_reg_byte(DS1307_REG_YEAR, ds1307_decode_dec(ptr_settime->set_ds1307_year % 100)); 
    }
    else return false;
}

uint8_t ds1307_get_reg_byte(uint8_t ds1307_reg_addr)
{
    uint8_t ds1307_val;
    HAL_I2C_Master_Transmit(&hi2c1, DS1307_I2C_ADDR << 1, &ds1307_reg_addr, 1, DS1307_TIMEOUT);
    HAL_I2C_Master_Receive(&hi2c1, DS1307_I2C_ADDR << 1, &ds1307_val, 1, DS1307_TIMEOUT);
    return ds1307_val;
}

void ds1307_setclock(uint8_t ds1307_halt)
{
    uint8_t ds1307_ch = (ds1307_halt ? 1 << 7 : 0);
    ds1307_set_reg_byte(DS1307_REG_SECOND, ds1307_ch | (ds1307_get_reg_byte(DS1307_REG_SECOND) & 0x7f));
}

void ds1307_init()
{
    ds1307_setclock(0);
}

uint8_t ds1307_decode_bin(uint8_t ds1307_bin)
{
    return (((ds1307_bin & 0xf0) >> 4) * 10) + (ds1307_bin & 0x0f);
}

uint8_t ds1307_decode_dec(uint8_t ds1307_dec)
{
    return (ds1307_dec % 10 + ((ds1307_dec / 10) << 4));
}

/* End of file -------------------------------------------------------- */
