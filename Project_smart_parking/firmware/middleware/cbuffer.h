/**
 * @file       cbuffer.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date
 * @author     Triet Luu
 * @brief      Circular Buffer
 *             This Circular Buffer is safe to use in IRQ with single reader,
 *             single writer. No need to disable any IRQ.
 *
 *             Capacity = <size> - 1
 * @note       None
 * @example
 * 
 */
/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __CBUFFER_H
#define __CBUFFER_H

/* Includes ----------------------------------------------------------- */
#include "cmsis_compiler.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Public defines ----------------------------------------------------- */
#define CB_MAX_SIZE (0x00800000)

/* Public enumerate/structure ----------------------------------------- */
typedef struct
{
  uint8_t *data;
  uint32_t size;
  uint32_t writer;
  uint32_t reader;
  uint32_t overflow;
  bool     active;
} cbuffer_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief Initialize the Circular buffer.
 *
 * @param cb A pointer to cbuffer_t structure to store information about the Circular buffer.
 * @param buf A pointer to the buffer memory to store the data.
 * @param size The size of the Circular buffer in bytes.
 */
void cb_init(cbuffer_t *cb, void *buf, uint32_t size);

/**
 * @brief Clear all data currently stored in the Circular buffer, making it empty.
 *
 * @param cb A pointer to cbuffer_t to work with the Circular buffer.
 */
void cb_clear(cbuffer_t *cb);

/**
 * @brief Read data from the Circular buffer into a buffer.
 *
 * @param cb A pointer to cbuffer_t to work with the Circular buffer.
 * @param buf A pointer to the buffer to store the read data.
 * @param nbytes The number of bytes to read from the Circular buffer.
 * @return The actual number of bytes read from the Circular buffer.
 */
uint32_t cb_read(cbuffer_t *cb, void *buf, uint32_t nbytes);

/**
 * @brief Write data from a buffer into the Circular buffer.
 *
 * @param cb A pointer to cbuffer_t to work with the Circular buffer.
 * @param buf A pointer to the buffer containing the data to be written into the Circular buffer.
 * @param nbytes The number of bytes to write into the Circular buffer.
 * @return The actual number of bytes written into the Circular buffer.
 */
uint32_t cb_write(cbuffer_t *cb, void *buf, uint32_t nbytes);

/**
 * @brief Get the number of bytes currently stored in the Circular buffer.
 *
 * @param cb A pointer to cbuffer_t to work with the Circular buffer.
 * @return The number of bytes currently stored in the Circular buffer.
 */
uint32_t cb_data_count(cbuffer_t *cb);

/**
 * @brief Get the number of empty bytes remaining in the Circular buffer.
 *
 * @param cb A pointer to cbuffer_t to work with the Circular buffer.
 * @return The number of empty bytes remaining in the Circular buffer.
 */
uint32_t cb_space_count(cbuffer_t *cb);

#endif  // __CBUFFER_H

/* End of file -------------------------------------------------------- */
