#ifndef	_CAN_H
#define	_CAN_H
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#include "mcp2515.h"
#include "mcp2515_defs.h"

/// Message structure
typedef struct {
	uint8_t	 	buf;		/// address of basic block MCP25XX 
                            ///     buffer(fieled within function)
	uint8_t		ext_id;		/// flag: normal (0) or extended (!0) id
	uint32_t	id;			/// ID
	uint8_t 	dlc;		/// number of data to send
	uint8_t 	dta[8];		/// array of dataa
	uint8_t		rtr;		/// flag: remote request
	uint8_t		idf;		/// Id of filter receiving message
} CanMessage;

/// Message status, send for transmission in a queue
typedef enum{
	TX_COMPLETE	= 0,		/// success
	TX_PENDED 	= 1,		/// waiting
	TX_ERROR	= 2,		/// error
	INVALID_MSG	= 3			/// message not set to queue
} CAN_TX_STATE;

/// Result of CAN-intefrace function
typedef enum{
	CAN_OK			= 0,	/// success complete
	CAN_MCP_ERROR	= 1,	/// error with MCP2515
	CAN_ALL_TX_BUSY	= 2,	/// during transmission:
                            ///     no free buffer
	CAN_ERROR		= 3,	/// end with error
	CAN_NOT_FOUND	= 4		/// during receive: no accepted messages
} CAN_RESULT;

/**
 * Initialise message structure from the 
 * structure.
 * 
 * @param msg point to structure
*/
void init_msg(CanMessage* msg);

/**
 * Check if all TXB occupied
 * 
 * @return if available TXB, returns 0, othervise !0
*/
uint8_t is_all_tx_busy(void);

/**
 * Returns status of the message, send for transmission
 * 
 * @param msg pointer to message
 * @return TX_PENDED - pending message, TX_ERROR - error of transmission, TX_COMPLETE - message send
*/
CAN_TX_STATE state_tx(CanMessage* msg);

/**
 * Set message in a queue.
 * 
 * @param msg pointer to a message
 * @return result of set: CAN_ALL_TX_BUSY if no free TXB or
 *          CAN_OK, if message was set to a queue.
*/
CAN_RESULT send_message(CanMessage* msg);

/**
 * Reading accepted messages.
 * As buffer 0 has a priority, first messages are read from it, and only if
 * there is no messages, the next one will be read.
 * 
 * @param msg pointer to message structure
 * @return CAN_OK - message accepted, CAN_NOT_FOUND - No accepted messages
*/
CAN_RESULT read_message(CanMessage* msg);

/**
 * Returning number of messages
 * Messages with error not counted
 * 
 * @param Number of accepted messages
*/
uint8_t resived_msg(void);

/**
 * Prints CAN message in the USB serial
*/
void print_can_message(CanMessage *message);

/**
 * Perform self test after initialising the controller
*/
uint8_t mcp_loopback(void);

#endif