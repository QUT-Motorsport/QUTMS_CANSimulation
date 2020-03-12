#include "can.h"

void init_msg(CanMessage* msg) {
	memset(msg,0,sizeof(CanMessage));
}

/**
 * Return first found TXB, available for send.
 * 
 * @return returns 0, if not TXB or address
 * from base register TXB
*/
static uint8_t get_free_txb(void) {
    uint8_t txb;
	// Go through all available TXB
	for(txb=TXB_BASE; txb < (TXB_BASE+(TXB_DELTA*TXB_CNT)); txb+=TXB_DELTA){
		// check transmission flag
		if(!(MCP2515_read_byte(txb) & TXREQ))
			return txb; // if no flag found - return this TXB
	}
	// if no found - return 0
	return 0;
}

uint8_t is_all_tx_busy(void) {
	return !get_free_txb();
}

CAN_TX_STATE state_tx(CanMessage* msg) {
	uint8_t i, tmp;
	for(i=TXB_BASE; i<(TXB_BASE+0x10*(TXB_CNT-1)); i+=0x10) {
		if(msg->buf == i) {
			tmp = MCP2515_read_byte(i);
			if(tmp & TXREQ) {
				if(!(tmp & (TXERR | MLOA)))
					return TX_PENDED;
				else
					return TX_ERROR;
			} else {
				return TX_COMPLETE;
			}
		}
	}
	return INVALID_MSG;
}

CAN_RESULT send_message(CanMessage* msg) {
    uint8_t rtr, txb;
	txb = get_free_txb();			// Getting free TXB
	if(!txb) return CAN_ALL_TX_BUSY;// If no message available - exit with error
	rtr = msg->rtr ? RTR : 0;		// preparing answer request
	
    // Filling ID
	if(msg->ext_id){
		// extended ID
		uint16_t t2 = msg->id >> 16;
		t2 <<= 3;
		MCP2515_write_byte(txb+SIDH, t2>>8);
		MCP2515_write_byte(txb+SIDL, EXIDE | (t2 & 0xE0) | ((t2 & 0x18)>>3));
		MCP2515_write_byte(txb+EID8, msg->id >> 8);
		MCP2515_write_byte(txb+EID0, msg->id);
	} else {
		// normal ID
		MCP2515_write_byte(txb+SIDL, msg->id << 5);
		MCP2515_write_byte(txb+SIDH, msg->id >> 3);
	}
	
    // Filling the exchange data buffer
	MCP2515_write_byte(txb+DLC, rtr | msg->dlc);	// number of data
	MCP2515_write_buf(txb+D0, msg->dta, msg->dlc);	// data itself
	
    // Start transmission
	MCP2515_write_byte(txb, TXREQ);
	msg->buf = txb;
	return CAN_OK;
}

/**
 * Extracts message from buffer.
 * 
 * Functions fils the structure with zeros
 * @param addr - address of the register
 * @param msg - pointer to a message
*/
static void get_msg(uint8_t addr, CanMessage* msg) {
	uint8_t tmp; 
    uint16_t t2;	
	init_msg(msg);                                  // Zero structure

	tmp = MCP2515_read_byte(addr);
	msg->rtr = tmp & RXRTR;			                // Field of remote request
	msg->buf = addr;				                // Remembering buffer address
	tmp = MCP2515_read_byte(addr+DLC);	            // Reading number of data
	msg->dlc = tmp & 0x07;			                // Fieling field the number of data
	MCP2515_read_buf(addr+D0, msg->dta, msg->dlc);  // Reading data
	tmp = MCP2515_read_byte(addr+SIDL);             // Reading lower byte of ID
	if(tmp & EXIDE) msg->ext_id = 1;                // Identefying extended ID flag
	if(msg->ext_id) {
		// Extended ID
		msg->id = (MCP2515_read_byte(addr+EID8) << 8) | (MCP2515_read_byte(addr+EID0));
		t2 = ((uint16_t)MCP2515_read_byte(addr+SIDH)<<8) + (tmp & 0xE0) + ((tmp & 3)<<3);
		msg->id = msg->id + ((uint32_t)t2<<13);
	} else {
		// Normal ID
		msg->id = ((uint16_t)tmp >> 5) | ((uint16_t)MCP2515_read_byte(addr+SIDH) << 3);
	}
}

CAN_RESULT read_message(CanMessage* msg) {
	uint8_t tmp = MCP2515_status();
	if(tmp & RX0IF){
		// if there is a flag in buffer 0 - reading it
		get_msg(RXB0CTRL, msg);
		MCP2515_bit_modify(CANINTF, RX0IF,0);
		return CAN_OK;
	}
	if(tmp & RX1IF){
		// if a flag in buffer 1 - reading from this instead
		get_msg(RXB1CTRL+RXB_DELTA, msg);
		MCP2515_bit_modify(CANINTF, RX1IF,0);
		return CAN_OK;
	}
	return CAN_NOT_FOUND;
}

uint8_t resived_msg(void) {
	uint8_t tmp;
	tmp = MCP2515_read_byte(CANINTF) & (RX1IF | RX0IF);
	switch(tmp){
	case 0:
	case RX0IF:
		return tmp;
	case RX1IF:
		return 1;
	default:
		return 2;
	}
}

void print_can_message(CanMessage *message) {
	uint8_t length = message->dlc;
    char buf[24];

    snprintf(buf, sizeof(buf), "id:     0x%lu\r\n", message->id);
    send_buffer( buf );

    snprintf(buf, sizeof(buf), "length: %d\r\n", length);
    send_buffer( buf );

    snprintf(buf, sizeof(buf), "rtr:    %d,  ext_id:    %d  \r\n", message->rtr, message->ext_id);
    send_buffer( buf );
    	
	if (!message->rtr) {
        send_str(PSTR("---data---\r\n" ));
		
		for (uint8_t i = 0; i < length; i++) {
            snprintf(buf, sizeof(buf), "0x%02x ", message->dta[i]);
            send_buffer( buf );
		}
        send_str(PSTR("\r\n------\r\n" ));
	}
}

uint8_t mcp_loopback(void){
	
	CanMessage message;
    CAN_RESULT res;
	
    send_str(PSTR("\r\n\r\n-- Start testing loopback -- \r\n" ));
 
 	// einige Testwerte
	message.id = 0x123;
	message.rtr = 0;
	message.dlc = 2;
	message.dta[0] = 0x04;
	message.dta[1] = 0xf3;
	
    uint8_t err = 0;
	
    send_str(PSTR("switch to loopback mode\r\n" ));
    if (MCP2515_mode_loopback() != 0) {
        send_str(PSTR("Error: no switch to loopback mode\r\n" ));
        err |= 0x01;
    }
	
	// Sende eine Nachricht
    res = send_message(&message); 
	if (res == CAN_OK) {
        send_str(PSTR("Message was written to the buffer\r\n" ));        
    	print_can_message(&message);        
	}
	else {
        send_str(PSTR("Error: could not send the message\r\n" ));
        err |= 0x02;
	}
	
	// warte ein bisschen
	_delay_ms(10);
	
	if (MCP2515_check_message()) {
        send_str(PSTR("Message received!\r\n" ));
		
		// read the message from the buffers
		if (read_message(&message) == CAN_OK) {
            send_str(PSTR("---- received message ---- \r\n" ));
			print_can_message(&message);
            send_str(PSTR("-successfull-\r\n" ));
		}
		else {
            send_str(PSTR("Error: could not read the message\r\n" ));
            err |= 0x04;
		}
	}
	else {
        send_str(PSTR("Error: no message received\r\n" ));
        err |= 0x08;
	}
	
    send_str(PSTR("back to normal mode\r\n" ));
    if (MCP2515_mode_normal() != 0) {
        send_str(PSTR("Error: no switch to normal mode\r\n" ));
        err |= 0x10;
    }

    send_str(PSTR("-- Finish testing loopback -- \r\n\r\n" ));
    	
    return err;
}
