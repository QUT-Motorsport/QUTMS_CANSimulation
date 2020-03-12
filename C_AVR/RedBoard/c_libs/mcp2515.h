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
 * Perform Software Reset. Controller will move to configuration mode.
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

/**     Normal mode
 * Normal mode is the standard operating mode of the
 * MCP2515. In this mode, the device actively monitors
 * all bus messages and generates Acknowledge bits,
 * error frames, etc. This is also the only mode in which
 * the MCP2515 will transmit messages over the CAN bus.
 *
 * @return - result of transmission
*/
uint8_t MCP2515_mode_normal(void);

/**     Configuration Mode
 * Configuration mode is automatically selected
 * after power-up, a Reset or can be entered from
 * any other mode. When Configuration mode is entered,
 * all error counters are cleared. Configuration
 * mode is the only mode where the following
 * registers are modifiable:
 *      CNF1, CNF2, CNF3 registers
 *      TXRTSCTRL registers
 *      Filter registers
 *      Mask registers
 *
 * @return - result of transmission
*/
uint8_t MCP2515_mode_config(void);

/**     Sleep mode
 * This mode minimises current consumption of the device.
 * SPI remains active, all registers can be accessed. When
 * in internal Sleep mode, the wake-up interrupt is still
 * active (if enabled). Stops internal oscilator. The TXCAN
 * pin will remain in the recessive state while the MCP2515
 * is in Sleep mode.
 *
 * @return - result of transmission
*/
uint8_t MCP2515_mode_sleep(void);

/**     Listen mode
 * Listen-Only mode provides a means for the MCP2515 to
 * receive all messages (including messages with errors).
 * Listen-Only mode is a silent mode, meaning no
 * messages will be transmitted while in this mode (including
 * error flags or Acknowledge signals). In Listen-Only mode,
 * both valid and invalid messages will be received, regardless
 * of filters and masks or the Receive Buffer Operating Mode
 * bits, RXMn. The error counters are reset and deactivated
 * in this state.
 *
 * @return - result of transmission
*/
uint8_t MCP2515_mode_listen(void);

/**     Loopback mode
 * Loopback mode will allow internal transmission of
 * messages from the transmit buffers to the receive buffers
 * without actually transmitting messages on the CAN bus.
 * This mode can be used in system development and testing.
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