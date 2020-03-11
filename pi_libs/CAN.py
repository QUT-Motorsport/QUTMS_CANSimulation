from pi_libs.MCP2515 import *
import pi_libs.MCP2515_defs as defs
# TODO Use numpy for other data types
#import numpy as np

class CAN_TX_STATE:
    TX_COMPLETE	= 0 		# success
    TX_PENDED 	= 1 		# waiting
    TX_ERROR	= 2 		# error
    INVALID_MSG	= 3 		# message not set to queue

class CAN_RESULT:
    CAN_OK			= 0 	# success complete
    CAN_MCP_ERROR	= 1 	# error with MCP2515
    CAN_ALL_TX_BUSY	= 2 	# during transmission:
                            #     no free buffer
    CAN_ERROR		= 3 	# end with error
    CAN_NOT_FOUND	= 4		# during receive: no accepted messages

# class CanMessage:
#     buf:    uint8_t   		# address of basic block MCP25XX buffer(fieled within function)
#     ext_id: uint8_t 		# flag: normal (0) or extended (!0) id
#     id:     uint32_t    	# ID
#     dlc:    uint8_t   		# number of data to send
#     dta[8]: uint8_t 		# array of dataa
#     rtr:    uint8_t   		# flag: remote request
#     idf:    uint8_t		    # Id of filter receiving message

class CanMessage:
    buf:    int   		# address of basic block MCP25XX buffer(fieled within function)
    ext_id: int 		# flag: normal (0) or extended (!0) id
    id:     int    	# ID
    dlc:    int   		# number of data to send
    dta:    [int] 		# array of dataa
    rtr:    int   		# flag: remote request
    idf:    int		    # Id of filter receiving message

class CAN:

    def __init__(self, chip, id, ext_id, rtr, dlc, data):
        """
        Initialise message structure from the 
        structure.
        """
        self.chip = chip
        
        self.CanResult = CAN_RESULT()
        self.message = CanMessage()
        
        self.message.id = id
        self.message.ext_id = ext_id
        self.message.rtr = rtr
        self.message.dlc = dlc        
        self.message.dta = data
    
    
    
    def get_free_txb(self):
        """
        Return first found TXB, available for send.
        
        @return returns 0, if not TXB or address
        from base register TXB
        """
        # Go through all available TXB
        limit = (defs.TXB_BASE+(defs.TXB_DELTA*defs.TXB_CNT))
        for txb in range(defs.TXB_BASE, limit, defs.TXB_DELTA):
            # check transmission flag
            if(not (self.chip.read_byte(txb) & defs.TXREQ)):
                return txb # if no flag found - return this TXB
            else:
                print("")
        # if no found - return 0
        return 0
    
    def is_all_tx_busy(self):
        """
        Check if all TXB occupied
        
        @return if available TXB, returns 0, othervise !0
        """
        return not self.get_free_txb
    
    def state_tx(self):
        """
        Returns status of the message, send for transmission
        
        @param msg pointer to message
        @return TX_PENDED - pending message, TX_ERROR - error of transmission, TX_COMPLETE - message send
        """
        tmp = 0
        limit = (defs.TXB_BASE+0x10*(defs.TXB_CNT-1))
        for i in range(TXB_BASE, limit, 0x10):
            if(self.message.buf == i):
                tmp = self.chip.read_byte(i)
                if(tmp & TXREQ):
                    if( not (tmp & (defs.TXERR | defs/MLOA))):
                        return self.CanResult.TX_PENDED
                    else:
                        return self.CanResult.TX_ERROR
                else:
                    return self.CanResult.TX_COMPLETE
        return self.CanResult.INVALID_MSG
    
    def send_message(self):
        """
        Reading accepted messages.
        As buffer 0 has a priority, first messages are read from it, and only if
        there is no messages, the next one will be read.
        
        @param msg pointer to message structure
        @return CAN_OK - message accepted, CAN_NOT_FOUND - No accepted messages
        """
        # Get free TXB
        txb = self.get_free_txb()
        
        # If no message available - exit with error
        if not txb:
            return self.CanResult.CAN_ALL_TX_BUSY
        
        # Preparing answer request        
        rtr = 0
        if (self.message.rtr):
            rtr = defs.RTR
        else:
            rtr = 0
        
        # Filling ID
        if(self.message.ext_id):
            # Extended ID
            t2 = self.message.id >> 16
            t2 <<= 3
            self.chip.write_byte(txb+defs.SIDH, t2>>8)
            self.chip.write_byte(txb+defs.SIDL, defs.EXIDE | (t2 & 0xE0) | ((t2 & 0x18)>>3))
            self.chip.write_byte(txb+defs.EID8, self.message.id >> 8)
            self.chip.write_byte(txb+defs.SIDH, self.message.id)
        else:
            # Normal ID
            self.chip.write_byte(txb+defs.SIDH, self.message.id << 5)
            self.chip.write_byte(txb+defs.SIDH, self.message.id >> 3)
        
        # Filling the exchange data buffer
        self.chip.write_byte(txb+defs.DLC, rtr | self.message.dlc)
        self.chip.write_buf(txb+defs.D0, self.message.dta, self.message.dlc)

        # Start transmission
        self.chip.write_byte(txb, defs.TXREQ)
        self.message.buf = txb
        return self.CanResult.CAN_OK
    
    def get_msg(addr):
        """
        Extracts message from buffer.
        
        Functions fils the structure with zeros
        @param addr - address of the register
        @param msg - pointer to a message
        """
        tmp = 0
        t2 = 0
        msg = CanMessage()  # Zero message

        tmp = self.chip.read_byte(addr)
        msg.rtr = tmp & defs.RXRTR  # Field of remote request
        msg.buf = addr  # Remembering buffer address
        tmp = self.chip.read_byte(addr+defs.DLC)    # Reading number of data
        msg.dlc = tmp & 0x07    # Fieling field the number of data
        msg.dta = self.chip.read_buf(addr+defs.D0, msg.dlc) # Reading data
        tmp = self.chip.read_byte(addr+defs.SIDL)   # Reading lower byte of ID

        if(tmp & defs.EXIDE):   # Identefy extended ID flag
            msg.ext_id = 1
        
        if(msg.ext_id):
            # Extended ID
            # TODO Consider that size of those variable may be small
            msg.id = ((self.chip.read_byte(addr+defs.EID8) << 8) | self.chip.read_byte(addr+defs.EID8))
            t2 = (self.chip.read_byte(addr+defs.EID8)<<8) + (tmp & 0xE0) + ((tmp & 3) << 3)
            msg.id = msg.id + (t2<<13)
        else:
            # Normal ID
            msg.id = ((tmp >> 5) | (self.chip.read_byte(addr+SIDH) << 3))
        return msg
    
    def read_message(self):
        """
        Reading accepted messages.
        As buffer 0 has a priority, first messages are read from it, and only if
        there is no messages, the next one will be read.
        
        @param msg pointer to message structure
        @return CAN_OK - message accepted, CAN_NOT_FOUND - No accepted messages
        """
        tmp = self.chip.status()
        msg = CanMessage()
        if (tmp & defs.RX0IF):
            msg = self.get_msg(defs.RXB0CTRL)
            self.chip.bit_modify(defs.CANINTF, RX0IF, 0)
            return self.CanResult.CAN_OK
        if (tmp & defs.RX1IF):
            msg = self.get_msg(defs.RXB1CTRL+defs.RXB_DELTA)
            self.chip.bit_modify(defs.CANINTF, RX1IF, 0)
            return self.CanResult.CAN_OK
        return self.CanResult.CAN_NOT_FOUND

    def resived_msg(self):
        tmp = self.chip.read_byte(defs.CANINTF) & (defs.RX1IF | defs.RX0IF)

        if(tmp == 0):
            return 0
        elif (tmp == RX0IF):
            return tmp
        elif (tmp == RX1IF):
            return 1
        else:
            return 2

    def print_can_message(self, message):
        length = message.dlc
        print("id   ")
        print(message.id)
        print("length:  ") 
        print(length)        

        print("rtr, ext_id: ")
        print(message.rtr)
        print(message.ext_id)
            
        if ( not message.rtr):
            print("---data---" )            
            for i in range (length):
                print("0x ")
                print(message.dta[i])            
            print("\r\n------" )
    
    def mcp_loopback(self, CAN_bus):
        #message = CanMessage(CAN_bus, 0x123, 0, 0, 2, [0x0f,0xF3])
        message = CanMessage()
        result =  CAN_RESULT()
        print("-- Start testing loopback --")

        message.id = 0x123
        message.ext_id = 1
        message.rtr = 0
        message.dlc = 2
        message.dta = [0x04, 0xF3]        

        err = 0
        
        print("switch to loopback mode")
        if(self.chip.mode_loopback() != 0):
            print()
            err |= 0x01
        
        # Send message
        res = self.send_message()
        if(res == result.CAN_OK):
            print("Message was written to the buffer")
            self.print_can_message(message)
        else:
            print("Error: could not send the message")
            err |=0x02
        time.sleep(10)

        # TODO: Check message left unimplemented
        #if(self.chip.check_message()):
        print("Back to normal mode")
        if(self.chip.mode_normal() != 0):
            print("Error: no switch to normal mode")
            err |= 0x10
        
        print("-- Finish testing loopback --")
        
        return err