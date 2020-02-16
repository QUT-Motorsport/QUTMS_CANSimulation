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

/**
 * Reads a buffer from MCP2515 register
 * 
 * @param addr Selecting address to read
 * @param buf Buffer pointer to save data
 * @param count Number of times reading buffer
*/
void MCP2515_read_buf(uint8_t addr, uint8_t* buf, uint8_t count);

/**
 * Write to a buffer of MCP2515 register
 * 
 * @param addr Selecting address to write
 * @param buf Buffer which will be written
 * @param count Number of time writing buffer
*/
void MCP2515_write_buf(uint8_t addr, uint8_t* buf, uint8_t count);

/**
 * Reads a byte from register
 * 
 * @param addr Address to read from
 * @return 8bit int resulted value
*/
uint8_t MCP2515_read_byte(uint8_t addr);

/**
 * Write a byte to resgister
 * 
 * @param addr Address to write in
 * @param 8bit data to send
*/
void MCP2515_write_byte(uint8_t addr, uint8_t data);

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
 * TODO: Requires completion
*/
void MCP2515_bit_modify(uint8_t addr, uint8_t mask, uint8_t data);

/**
 * Sets mask to RXM0SIDH registers
 * param id 1 - RXM1SIDH, other - RMX0SIDH
*/
void MCP2515_set_mask(uint8_t id, uint8_t ext, uint32_t mask);

/**
 * 
*/
void MCP2515_set_filtr(uint8_t id, uint8_t ext, uint32_t filtr);

/**
 * Set mode
 * @param mode 80bit mode select
 * @return Status of set. -1 if fail.
*/
uint8_t MCP2515_set_mode(uint8_t mode);

/**
 * Set normal mode
*/
uint8_t MCP2515_mode_normal(void);

/**
 * Set Configuration mode
*/
uint8_t MCP2515_mode_config(void);

/**
 * Set Sleep mode
*/
uint8_t MCP2515_mode_sleep(void);

/**
 * Set Listen mode
*/
uint8_t MCP2515_mode_listen(void);

/**
 * Set Loopback mode
*/
uint8_t MCP2515_mode_loopback(void);

/**
 * 
*/
uint8_t MCP2515_get_rt_pin(void);

/**
 * 
*/
void MCP2515_set_bf_pin(uint8_t data);

/**
 * Initialise MCP2515 controller
*/
uint8_t MCP2515_init(void);
#endif