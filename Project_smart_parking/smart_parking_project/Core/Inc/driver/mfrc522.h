/**
 * @file       rc522.h
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024-03-23
 * @author     Nguyen Minh
 *             
 * @brief      Driver for RC522
 *             
 */
/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __RC522_H
#define __RC522_H
/* Includes ----------------------------------------------------------- */
#include "main.h"
#include "stm32f4xx_hal.h"
/* Public defines ----------------------------------------------------- */
#define RC522_CS_GPIO_PORT GPIOA
#define RC522_CS_PIN	   GPIO_PIN_15

//Maximum length of the array
#define RC522_MAX_LEN           16

//MF522 Command word
#define RC522_COM_IDLE          0x00 //NO action; Cancel the current command
#define RC522_COM_AUTHENT       0x0E //Authentication Key
#define RC522_COM_RECEIVE       0x08 //Receive Data
#define RC522_COM_TRANSMIT      0x04 //Transmit data
#define RC522_COM_TRANSCEIVE    0x0C //Transmit and receive data,
#define RC522_COM_RESETPHASE    0x0F //Reset
#define RC522_COM_CALC_CRC      0x03 //CRC Calculate

// Mifare_One card command word
# define RC522_REQ_IDL          0x26 // find the antenna area does not enter hibernation
# define RC522_REQ_ALL          0x52 // find all the cards antenna area
# define RC522_ANTI_COLL        0x93 // anti-collision
# define RC522_SELECT_TAG       0x93 // election card
# define RC522_AUTHENT_1A       0x60 // authentication key A
# define RC522_AUTHENT_1B       0x61 // authentication key B
# define RC522_READ             0x30 // Read Block
# define RC522_WRITE            0xA0 // write block
# define RC522_DECREMENT        0xC0 // debit
# define RC522_INCREMENT        0xC1 // recharge
# define RC522_RESTORE          0xC2 // transfer block data to the buffer
# define RC522_TRANSFER         0xB0 // save the data in the buffer
# define RC522_HALT             0x50 // Sleep


//And MF522 The error code is returned when communication
#define STATUS_OK               0
#define STATUS_IDLE             1
#define STATUS_ERROR            2

//MFRC522 Register
//Page 0:Command and Status
#define     RC522_RESERVED_00      0x00   //Reserved for future use
#define     RC522_COMMAND_REG      0x01   //Starts and stops command execution
#define     RC522_COM_IEN_REG      0x02   //Controls bits to enable and disable the passing of IT req
#define     RC522_DIVLEN_REG       0x03   //Controls bits to enable and disable the passing of IT req
#define     RC522_COM_IRQ_REG      0x04   //Contains Interrupt Request bits
#define     RC522_DIV_IRQ_REG      0x05   //Contains Interrupt Request bits
#define     RC522_ERROR_REG        0x06   //Error bits showing the error status of the last command executed
#define     RC522_STATUS1_REG      0x07   //Contains status bits for communication
#define     RC522_STATUS2_REG      0x08   //Contains status bits of the receiver and transmitter
#define     RC522_FIFO_DATA_REG    0x09   //In- and output of 64 byte FIFO buffer
#define     RC522_FIFO_LEVEL_REG   0x0A   //Indicates the number of bytes stored in the FIFO
#define     RC522_WATER_LEVEL_REG  0x0B   //Defines the level for FIFO under- and overflow warning
#define     RC522_CONTROL_REG      0x0C   //Contains miscellaneous Control Registers
#define     RC522_BIT_FRAMING_REG  0x0D   //Adjustments for bit oriented frames
#define     RC522_COLL_REG         0x0E   //Bit position of the first bit collision detected on the     RF-interface
#define     RC522_RESERVED_01      0x0F   //Reserved for future use
//Page 1:Command
#define     RC522_RESERVED_10            0x10 //Reserved for future use
#define     RC522_MODE_REG               0x11 //Defines general modes for transmitting and receiving
#define     RC522_TX_MODE_REG            0x12 //Defines the transmission data rate and framing
#define     RC522_RX_MODE_REG            0x13 //Defines the receive data rate and framing
#define     RC522_TX_CONTROL_REG         0x14 //Controls the logical behavior of the antenna driver pins TX1 and TX2
#define     RC522_TX_AUTO_REG            0x15 //Controls the setting of the TX modulation
#define     RC522_TX_SEL_REG             0x16 //Selects the internal sources for the antenna driver
#define     RC522_RX_SEL_REG             0x17 //Selects internal receiver settings
#define     RC522_RX_THRESHOLD_REG       0x18 //Selects thresholds for the bit decoder
#define     RC522_DEMOD_REG              0x19 // Defines demodulator settings
#define     RC522_RESERVED_11            0x1A //Reserved for future use
#define     RC522_RESERVED_12            0x1B //Reserved for future use
#define     RC522_MIFARE_REG             0x1C //Controls some MIFARE® communication transmit parameters
#define     RC522_RESERVED_13            0x1D //Reserved for future use
#define     RC522_RESERVED_14            0x1E //Reserved for future use
#define     RC522_SERIAL_SPEED_REG       0x1F //Selects the speed of the serial UART interface
//Page 2:CFG
#define     RC522_RESERVED_20            0x20 //Reserved for future use
#define     RC522_CRC_RESULT_REGM        0x21 //Shows the actual MSB values of the CRC calculation
#define     RC522_CRC_RESULT_REGL        0x22 //Shows the actual LSB values of the CRC calculation
#define     RC522_RESERVED_21            0x23 //Reserved for future use
#define     RC522_MOD_WIDTH_REG          0x24 //Controls the setting of the ModWidth
#define     RC522_RESERVED_22            0x25 //Reserved for future use
#define     RC522_RFCFG_REG              0x26 //Configures the receiver gain
#define     RC522_GSN_REG                0x27 //Selects the conductance of the antenna driver pins TX1 and TX2 for modulation
#define     RC522_CW_GSP_REG             0x28
#define     RC522_MOD_GSP_REG            0x29
#define     RC522_TMODE_REG              0x2A //Defines settings for the internal timer
#define     RC522_TPRESCALER_REG         0x2B 
#define     RC522_TRELOAD_REGH           0x2C //Describes the 16 bit timer reload value
#define     RC522_TRELOAD_REGL           0x2D 
#define     RC522_TCOUNTER_VALUE_REGH    0x2E //Shows the 16 bit actual timer value
#define     RC522_TCOUNTER_VALUE_REGL    0x2F
//Page 3:TestRegister
#define     RC522_RESERVED_30            0x30 //Reserved for future use
#define     RC522_TEST_SEL1_REG          0x31 //General test signal configuration
#define     RC522_TEST_SEL2_REG          0x32 //General test signal configuration and PRBS control
#define     RC522_TEST_PINEN_REG         0x33 //Enables pin output driver on D1-D7
#define     RC522_TEST_PIN_VALUE_REG     0x34 //Defines the values for D1 - D7 when it is used as I/O bus
#define     RC522_TEST_BUS_REG           0x35 //Shows the status of the internal testbus
#define     RC522_AUTO_TEST_REG          0x36 //Controls the digital selftest
#define     RC522_VERSION_REG            0x37 //Shows the version
#define     RC522_ANALOG_TEST_REG        0x38 //Controls the pins AUX1 and AUX2
#define     RC522_TEST_DAC1_REG          0x39 //Defines the test value for the TestDAC1
#define     RC522_TEST_DAC2_REG          0x3A //Defines the test value for the TestDAC2
#define     RC522_TEST_ADC_REG           0x3B //Shows the actual value of ADC I and Q 
#define     RC522_RESERVED_31            0x3C //Reserved for production tests
#define     RC522_RESERVED_32            0x3D
#define     RC522_RESERVED_33            0x3E
#define     RC522_RESERVED_34            0x3F
/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  write data to register of MFRC522
 *
 * @param   rc522_reg_addr  addr of register
 * @param   rc522_reg_val   data write to register
 *
 * @return  no
 */
void mfrc522_write_reg(unsigned char rc522_reg_addr, unsigned char rc522_reg_val);

/**
 * @brief  read data from register
 *
 * @param   rc522_reg_addr  <addr of register>
 *
 * @return  unsigned char          <data read from register>
 */
unsigned char mfrc522_read_reg(unsigned char rc522_reg_addr);

/**
 * @brief  set bit mask register
 *
 * @param    rc522_reg_addr  <addr of register>
 * @param    rc522_bit_mask  <set bit mask value>
 *
 */
void mfrc522_set_bit_mask(unsigned char rc522_reg_addr, unsigned char rc522_bit_mask);

/**
 * @brief  clear bit mask from register
 *
 * @param   rc522_reg_addr  <addr of register>
 * @param   rc522_bit_mask  <clear bit value>
 */
void mfrc522_clear_bit_mask(unsigned char rc522_reg_addr, unsigned char rc522_bit_mask);

/**
 * @brief  open antennas
 *
 */
void mfrc522_antenna_on();

/**
 * @brief  off antennas
 *
 */
void mfrc522_antenna_off();

/**
 * @brief  reset MFRC522
 *
 */
void mfrc522_reset();

/**
 * @brief  Initialize MFRC522
 *
 */
void mfrc522_init();

/**
 * @brief  find cards, read the card type number
 *
 * @param   rc522_req_mode <find cards way>
 * @param   rc522_tag_type <card type>
 
 * @return  unsigned char: the successful return STATUS_OK
 */
unsigned char mfrc522_request(unsigned char rc522_req_mode, unsigned char *rc522_tag_type);

/**
 * @brief  RC522 and ISO14443 card communication
 *
 * @param   rc522_command       <MF522 command word>
 * @param   *rc522_send_data    <RC522 sent to the card by the data>
 * @param   rc522_send_len      <Length of data sent>
 * @param   rc522_back_data     <Received the card returns data>
 * @param   *rc522_back_len     <Return data bit length>
 *
 * @return  unsigned char: the successful return STATUS_OK
 */
unsigned char mfrc522_to_card(unsigned char rc522_command, unsigned char *rc522_send_data, unsigned char rc522_send_len, unsigned char *rc522_back_data, uint16_t *rc522_back_len);

/**
 * @brief   Anti-collision detection, reading selected card serial number card
 *
 * @param   *rc522_serial_num <returns 4 bytes card serial number, the first 5 bytes for the checksum byte>
 *
 * @return  the successful return STATUS_OK
 */
unsigned char mfrc522_anti_coll(unsigned char *rc522_serial_num);

/**
 * @brief  CRC calculation with MF522
 *
 * @param   *rc_522_ptr_indata <pointer to read the CRC data>
 * @param   rc522_len          <data length>
 * @param   *rc522_ptr_outdata <pointer to CRC calculation results>
 */
void calulate_crc(unsigned char *rc_522_ptr_indata, unsigned char rc522_len, unsigned char *rc522_ptr_outdata);

/**
 * @brief  read the card memory capacity
 *
 * @param  *rc522_serial_num <incoming card serial number>
 *
 * @return  the successful return of card capacity
 */
unsigned char mfrc522_select_tag(unsigned char *rc522_serial_num);

/**
 * @brief  verify card password
 *
 * @param   rc522_auth_mode     <Password Authentication Mode>
 * @param   rc522_block_addr    <Block address>
 * @param   *rc522_sector_key   <Sector password>
 * @param   *rc522_serial_num   <Card serial number, 4-byte>
 
 * @return  the successful return STATUS_OK
 */
unsigned char mfrc522_auth(unsigned char rc522_auth_mode, unsigned char rc522_block_addr, unsigned char *rc522_sector_key, unsigned char *rc522_serial_num);

/**
 * @brief  Read block data
 *
 * @param   rc522_block_addr <block address want to read>
 * @param   *rc522_rec_data  <receive data read from block>

 * @return  the successful return STATUS_OK
 */
unsigned char mfrc522_read_block(unsigned char rc522_block_addr, unsigned char *rc522_rec_data);

/**
 * @brief  Write block dat
 *
 * @param   rc522_block_addr    <block address>
 * @param   *rc522_write_data   <data write to block>
 *
 * @return  the successful return STATUS_OK
 */
unsigned char mfrc522_write_block(unsigned char rc522_block_addr, unsigned char *rc522_write_data);

/**
 * @brief  Stop Crypto1 encryption function on MFRC522.
 *
 * @attention  This function will disable the Crypto1 encryption function on MFRC522, causing data transmitted wirelessly between the card and the reader to be unencrypted.
 *
 */
void mfrc522_stop_crypto();

#endif // __RC522_H

/* End of file -------------------------------------------------------- */
