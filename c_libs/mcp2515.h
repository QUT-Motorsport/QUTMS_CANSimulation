#ifndef	_MCP2515_H
#define	_MCP2515_H

//#include <mega16.h>
#include <avr/io.h>
// TempLibrary
#include <avr/pgmspace.h>

#include <util/delay.h>
#include <stdint.h>

#include "spi.h"
#include "mcp2515_defs.h"
#include "usb_transmit.h"

#define MCP2515_PORT_RESET     PORTF
#define MCP2515_PIN_RESET      PINF1

// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
// MCP2515 register manipulation
// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=

/**
 * Reads a buffer from MCP2515 register
 * 
 * @param addr - Selecting address to read
 * @param buf - Buffer pointer to save data
 * @param count - Number of times reading buffer
*/
void MCP2515_read_buf(uint8_t addr, uint8_t* buf, uint8_t count);

/**
 * Write to a buffer of MCP2515 register
 * 
 * @param addr - Selecting address to write
 * @param buf - Buffer which will be written
 * @param count - Number of time writing buffer
*/
void MCP2515_write_buf(uint8_t addr, uint8_t* buf, uint8_t count);

/**
 * Reads a byte from register
 * 
 * @param addr - Address to read from
 * @return 8bit int resulted value
*/
uint8_t MCP2515_read_byte(uint8_t addr);

/**
 * Write a byte to resgister
 * 
 * @param addr - Address to write in
 * @param data - 8bit data to send
*/
void MCP2515_write_byte(uint8_t addr, uint8_t data);

// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
// MCP2515 chip control
// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=

/**
 * Perform Software Reset
*/
void MCP2515_reset(void);

/**
 * Platform dependent hardware reset toggeling
 * reset button.
*/
void MCP2515_hardware_reset(void);

/**
 * Read status of MCP2515 chip
 * 
 * @return 8-bit status register
*/
uint8_t MCP2515_status(void);

/**
 * Send command for starting transmission
 * @param TXB_mask 
*/
void MCP2515_start_send(uint8_t TXB_mask);

/**
 * Modefy bits in the MCP25** registers
 * Function performs XOR for bits based on the masks.
 * As funtion supported by not all registers MCP25**,
 * function checks the valid addr and ignores others.
 * 
 * @param addr - address of register
 * @param mask - Mask for bit modefy
 * @param data - value of bits
 */
void MCP2515_bit_modify(uint8_t addr, uint8_t mask, uint8_t data);

/**
 * Sets mask to RXM0SIDH registers
 * param id 1 - RXM1SIDH, other - RMX0SIDH
 * 
 * @param id - ID for register
 * @param ext - flag for 0 as notmal, 1 extended
 * @param mask - Mask itself
*/
void MCP2515_set_mask(uint8_t id, uint8_t ext, uint32_t mask);

/**
 * Sets filter for accepted messages.
 * Works only in Config mode
 * @param id - ID of filter
 * @param ext - flag for 0 as notmal, 1 extended
 * @param filtr - Mask of the filter itself
*/
void MCP2515_set_filtr(uint8_t id, uint8_t ext, uint32_t filtr);

// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
// MCP2515 MODE CHANGE
// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=

/**
 * Set mode
 * @param mode - 8bit mode select
 * @return Status of set. -1 if fail.
*/
uint8_t MCP2515_set_mode(uint8_t mode);

/**
 * Set normal mode
 * @return - result of transmission
*/
uint8_t MCP2515_mode_normal(void);

/**
 * Set Configuration mode
 * @return - result of transmission
*/
uint8_t MCP2515_mode_config(void);

/**
 * Set Sleep mode
 * @return - result of transmission
*/
uint8_t MCP2515_mode_sleep(void);

/**
 * Set Listen mode
 * @return - result of transmission
*/
uint8_t MCP2515_mode_listen(void);

/**
 * Set Loopback mode
 * @return - result of transmission
*/
uint8_t MCP2515_mode_loopback(void);

/**
 * Returns the levels on input pins
 * @return RTS bits
*/
uint8_t MCP2515_get_rt_pin(void);

/**
 * Set the levels on input pins
 * @param data bit mask of levels
*/
void MCP2515_set_bf_pin(uint8_t data);

/**
 * check if there are any new messages waiting
 * @return - True then triggered
*/
uint8_t MCP2515_check_message(void);
/**
 * Initialise MCP2515 controller
*/
uint8_t MCP2515_init(void);
#endif