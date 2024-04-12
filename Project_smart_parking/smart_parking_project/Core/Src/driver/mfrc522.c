/*
 * @file       rc522.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024-27-03
 * @author     Nguyen Minh
 *             
 * @brief      Driver for MFRC522
*/
/* Includes ----------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "mfrc522.h"
#include "bsp_gpio.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
extern SPI_HandleTypeDef hspi1;
/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void mfrc522_write_reg(unsigned char rc522_reg_addr, unsigned char rc522_reg_val)
{
  unsigned char addr_bits = (((rc522_reg_addr<<1) & 0x7E));
  bsp_gpio_reset_pin(RC522_CS_GPIO_PORT, RC522_CS_PIN);
  HAL_SPI_Transmit(&hspi1, &addr_bits, 1, 500);
  HAL_SPI_Transmit(&hspi1, &rc522_reg_val, 1, 500);
  bsp_gpio_set_pin(RC522_CS_GPIO_PORT, RC522_CS_PIN);
}

unsigned char mfrc522_read_reg(unsigned char rc522_reg_addr)
{
  unsigned char rx_bits;
  unsigned char addr_bits = (((rc522_reg_addr<<1) & 0x7E) | 0x80);

  bsp_gpio_reset_pin(RC522_CS_GPIO_PORT, RC522_CS_PIN);
  HAL_SPI_Transmit(&hspi1, &addr_bits, 1, 500);

  HAL_SPI_Receive(&hspi1, &rx_bits, 1, 500);
  bsp_gpio_set_pin(RC522_CS_GPIO_PORT, RC522_CS_PIN);

	return rx_bits; // return the rx bits, casting to an 8 bit int and chopping off the upper 24 bits
}

void mfrc522_set_bit_mask(unsigned char rc522_reg_addr, unsigned char rc522_bit_mask)
{
    unsigned char temp;
    temp = mfrc522_read_reg(rc522_reg_addr);
    mfrc522_write_reg(rc522_reg_addr, temp | rc522_bit_mask);  // set bit mask
    }

void mfrc522_clear_bit_mask(unsigned char rc522_reg_addr, unsigned char rc522_bit_mask)
{
    unsigned char temp;
    temp = mfrc522_read_reg(rc522_reg_addr);
    mfrc522_write_reg(rc522_reg_addr, temp & (~rc522_bit_mask));  // clear bit mask
}

void mfrc522_antenna_on()
{
  mfrc522_set_bit_mask(RC522_TX_CONTROL_REG, 0x03);
}

void mfrc522_antenna_off()
{
  mfrc522_clear_bit_mask(RC522_TX_CONTROL_REG, 0x03);
}

void mfrc522_reset()
{
  mfrc522_write_reg(RC522_COMMAND_REG, RC522_COM_RESETPHASE);
}

void mfrc522_init()
{
  //MSS_GPIO_set_output( MSS_GPIO_1, 1 );
  mfrc522_reset();

  //Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
  mfrc522_write_reg(RC522_TMODE_REG, 0x80);       // 0x8D); Tauto=1; f(Timer) = 6.78MHz/TPreScaler
  mfrc522_write_reg(RC522_TPRESCALER_REG, 0xA9);  //0x34);  TModeReg[3..0] + TPrescalerReg
  mfrc522_write_reg(RC522_TRELOAD_REGL, 0x03);    //30);
  mfrc522_write_reg(RC522_TRELOAD_REGH, 0xE8);    //0);
  mfrc522_write_reg(RC522_TX_AUTO_REG, 0x40);     // force 100% ASK modulation
  mfrc522_write_reg(RC522_MODE_REG, 0x3D);        // CRC Initial value 0x6363

  //turn antenna on
  mfrc522_antenna_on();
}

unsigned char mfrc522_request(unsigned char rc522_req_mode, unsigned char *rc522_tag_type)
{
  unsigned char status;
  uint16_t back_bits; // The received data bits

  mfrc522_write_reg(RC522_BIT_FRAMING_REG, 0x07);   //TxLastBists = RC522_BIT_FRAMING_REG[2..0]

  rc522_tag_type[0] = rc522_req_mode;

  status = mfrc522_to_card(RC522_COM_TRANSCEIVE, rc522_tag_type, 1, rc522_tag_type, &back_bits);
  if ((status != STATUS_OK) || (back_bits != 0x10)) 
  {
    status = STATUS_ERROR;
  }
  return status;
}

unsigned char mfrc522_to_card(unsigned char rc522_command, unsigned char *rc522_send_data, unsigned char rc522_send_len, unsigned char *rc522_back_data, uint16_t *rc522_back_len)
{
  unsigned char status = STATUS_ERROR;
  unsigned char irq_enable = 0x00;
  unsigned char wait_irq = 0x00;
  unsigned char last_bits;
  unsigned char n;
  uint16_t i;

  switch (rc522_command)
  {
    case RC522_COM_AUTHENT:     // Certification cards close
      {
        irq_enable = 0x12;
        wait_irq = 0x10;
        break;
      }
    case RC522_COM_TRANSCEIVE:  // Transmit FIFO data
      {
        irq_enable = 0x77;
        wait_irq = 0x30;
        break;
      }
    default:
      break;
  }

  mfrc522_write_reg(RC522_COM_IEN_REG, irq_enable|0x80);   // Interrupt request
  mfrc522_clear_bit_mask(RC522_COM_IRQ_REG, 0x80);          // Clear all interrupt request bit
  mfrc522_set_bit_mask(RC522_FIFO_LEVEL_REG, 0x80);         // FlushBuffer=1, FIFO Initialization

  mfrc522_write_reg(RC522_COMMAND_REG, RC522_COM_IDLE);    // NO action; Cancel the current command

  // Writing data to the FIFO
  for (i = 0; i < rc522_send_len; i++)
  {
    mfrc522_write_reg(RC522_FIFO_DATA_REG, rc522_send_data[i]);
  }

  //Execute the command
  mfrc522_write_reg(RC522_COMMAND_REG, rc522_command);
  if (rc522_command == RC522_COM_TRANSCEIVE)
  {
    mfrc522_set_bit_mask(RC522_BIT_FRAMING_REG, 0x80);      // StartSend=1,transmission of data starts
  }

  // Waiting to receive data to complete
  i = 2000;	// i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms
  do
  {
    // CommIrqReg[7..0]
    // Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = mfrc522_read_reg(RC522_COM_IRQ_REG);
    i--;
  }
  while ((i != 0) && !(n & 0x01) && !(n & wait_irq));

  mfrc522_clear_bit_mask(RC522_BIT_FRAMING_REG, 0x80);      // StartSend=0

  if (i != 0)
  {
    if(!(mfrc522_read_reg(RC522_ERROR_REG) & 0x1B))  // BufferOvfl Collerr CRCErr ProtecolErr
    {
      status = STATUS_OK;
      if (n & irq_enable & 0x01)
      {
        status = STATUS_IDLE;             // ??
      }

      if (rc522_command == RC522_COM_TRANSCEIVE)
      {
        n = mfrc522_read_reg(RC522_FIFO_LEVEL_REG);
        last_bits = mfrc522_read_reg(RC522_CONTROL_REG) & 0x07;
        if (last_bits)
        {
          *rc522_back_len = (n - 1)*8 + last_bits;
        }
        else
        {
          *rc522_back_len = n * 8;
        }

        if (n == 0)
        {
          n = 1;
        }
        if (n > RC522_MAX_LEN)
        {
          n = RC522_MAX_LEN;
        }

        // Reading the received data in FIFO
        for (i = 0; i < n; i++)
        {
          rc522_back_data[i] = mfrc522_read_reg(RC522_FIFO_DATA_REG);
        }
      }
    }
    else {
      //printf("~~~ buffer overflow, collerr, crcerr, or protecolerr\r\n");
      status = STATUS_ERROR;
    }
  }
  else {
    //printf("~~~ request timed out\r\n");
  }

  return status;
}

unsigned char mfrc522_anti_coll(unsigned char *rc522_serial_num)
{
  unsigned char status;
  unsigned char i;
  unsigned char ser_num_check=0;
  uint16_t unlen;


  //mfrc522_clear_bit_mask(RC522_STATUS2_REG, 0x08);		//TempSensclear
  //mfrc522_clear_bit_mask(CollReg,0x80);			//ValuesAfterColl
  mfrc522_write_reg(RC522_BIT_FRAMING_REG, 0x00);		//TxLastBists = RC522_BIT_FRAMING_REG[2..0]

  rc522_serial_num[0] = RC522_ANTI_COLL;
  rc522_serial_num[1] = 0x20;
  status = mfrc522_to_card(RC522_COM_TRANSCEIVE, rc522_serial_num, 2, rc522_serial_num, &unlen);

  if (status == STATUS_OK)
  {
    //Check card serial number
    for (i = 0; i < 4; i++)
    {
      ser_num_check ^= rc522_serial_num[i];
    }
    if (ser_num_check != rc522_serial_num[i])
    {
      status = STATUS_ERROR;
    }
  }

  //mfrc522_set_bit_mask(CollReg, 0x80);		//ValuesAfterColl=1

  return status;
}

unsigned char mfrc522_read_block(unsigned char rc522_block_addr, unsigned char *rc522_rec_data)
{
  unsigned char status;
  uint16_t unlen;

  rc522_rec_data[0] = RC522_READ;
  rc522_rec_data[1] = rc522_block_addr;
  calulate_crc(rc522_rec_data, 2, &rc522_rec_data[2]);
  status = mfrc522_to_card(RC522_COM_TRANSCEIVE, rc522_rec_data, 4, rc522_rec_data, &unlen);

  if ((status != STATUS_OK) || (unlen != 0x90))
  {
    status = STATUS_ERROR;
  }

  return status;
}

unsigned char mfrc522_write_block(unsigned char rc522_block_addr, unsigned char *rc522_write_data)
{
  unsigned char status;
  uint16_t recv_bits;
  unsigned char i;
  unsigned char buffer[18];

  buffer[0] = RC522_WRITE;
  buffer[1] = rc522_block_addr;
  calulate_crc(buffer, 2, &buffer[2]);
  status = mfrc522_to_card(RC522_COM_TRANSCEIVE, buffer, 4, buffer, &recv_bits);

  if ((status != STATUS_OK))// || (recv_bits != 4) || ((buffer[0] & 0x0F) != 0x0A))
  {
    status = STATUS_ERROR;
  }

  if (status == STATUS_OK)
  {
    for (i = 0; i < 16; i++)		//Data to the FIFO write 16Byte
    {
      buffer[i] = *(rc522_write_data + i);
    }
    calulate_crc(buffer, 16, &buffer[16]);
    status = mfrc522_to_card(RC522_COM_TRANSCEIVE, buffer, 18, buffer, &recv_bits);

    if ((status != STATUS_OK))// || (recv_bits != 4) || ((buffer[0] & 0x0F) != 0x0A))
    {
      status = STATUS_ERROR;
    }
  }

  return status;
}

void calulate_crc(unsigned char *rc_522_ptr_indata, unsigned char rc522_len, unsigned char *rc522_ptr_outdata)
{
  unsigned char i, n;

  mfrc522_clear_bit_mask(RC522_DIV_IRQ_REG, 0x04);			//CRCIrq = 0
  mfrc522_set_bit_mask(RC522_FIFO_LEVEL_REG, 0x80);			//Clear the FIFO pointer
  //mfrc522_write_reg(RC522_COMMAND_REG, RC522_COM_IDLE);

  //Writing data to the FIFO
  for (i = 0; i < rc522_len; i++)
  {
    mfrc522_write_reg(RC522_FIFO_DATA_REG, *(rc_522_ptr_indata+i));
  }
  mfrc522_write_reg(RC522_COMMAND_REG, RC522_COM_CALC_CRC);

  //Wait CRC calculation is complete
  i = 0xFF;
  do
  {
    n = mfrc522_read_reg(RC522_DIV_IRQ_REG);
    i--;
  }
  while ((i != 0) && !(n & 0x04));			//CRCIrq = 1

  //Read CRC calculation result
  rc522_ptr_outdata[0] = mfrc522_read_reg(RC522_CRC_RESULT_REGL);
  rc522_ptr_outdata[1] = mfrc522_read_reg(RC522_CRC_RESULT_REGM);
}

unsigned char mfrc522_auth(unsigned char rc522_auth_mode, unsigned char rc522_block_addr, unsigned char *rc522_sector_key, unsigned char *rc522_serial_num)
{
  unsigned char status;
  uint16_t recv_bits;
  unsigned char i;
  unsigned char buffer[12];

  //Verify the command block address + sector + password + card serial number
  buffer[0] = rc522_auth_mode;
  buffer[1] = rc522_block_addr;
  for (i = 0; i < 6; i++)
  {
    buffer[i + 2] = *(rc522_sector_key+i);
  }
  for (i = 0; i < 4; i++)
  {
    buffer[i + 8] = *(rc522_serial_num+i);
  }
  status = mfrc522_to_card(RC522_COM_AUTHENT, buffer, 12, buffer, &recv_bits);

  if ((status != STATUS_OK) || (!(mfrc522_read_reg(RC522_STATUS2_REG) & 0x08)))
  {
    status = STATUS_ERROR;
  }

  return status;
}

unsigned char mfrc522_select_tag(unsigned char *rc522_serial_num)
{
  unsigned char i;
  unsigned char status;
  unsigned char size;
  uint16_t recv_bits;
  unsigned char buffer[9];

  //mfrc522_clear_bit_mask(RC522_STATUS2_REG, 0x08);			//MFCrypto1On=0

  buffer[0] = RC522_SELECT_TAG;
  buffer[1] = 0x70;
  for (i = 0; i < 5; i++)
  {
    buffer[i + 2] = *(rc522_serial_num+i);
  }
  calulate_crc(buffer, 7, &buffer[7]);		//??
  status = mfrc522_to_card(RC522_COM_TRANSCEIVE, buffer, 9, buffer, &recv_bits);

  if ((status == STATUS_OK) && (recv_bits == 0x18))
  {
    size = buffer[0];
  }
  else
  {
    size = 0;
  }
  return size;
}

void mfrc522_stop_crypto()
{
	// Clear MFCrypto1On bit
	mfrc522_clear_bit_mask(RC522_STATUS2_REG, 0x08); // RC522_STATUS2_REG[7..0] bits are: TempSensClear I2CForceHS reserved reserved   MFCrypto1On ModemState[2:0]
}
/* End of file -------------------------------------------------------- */
