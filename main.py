#!/usr/bin/python

import time
import sys

from pi_libs.SPI import *
from pi_libs.MCP2515 import *
from pi_libs.CAN import *

if __name__ == '__main__':    
    SPI_0 = SPI()
    print("SPI initialised as spi")
    CAN_bus = MCP2515(SPI_0)
    try:
        # Repeatedly switch a MCP4151 digital pot off then on
        #CAN.reset_hardware()
        if(CAN_bus.MCP2515_init() == -2):
            print("ERR: Chip Failed to set Normal mode")
            time.sleep(1)
        print("MCP passed initialisation")
        time.sleep(0.1)
        MSG = CAN(CAN_bus, 0x0A000000, 1, 0, 5, [0x01,0x00,0x01,0x00,0x02])
        MSG_result = CAN_RESULT()
        isLed = 0        
        j = 0
        k = 3
        # MSG.mcp_loopback(CAN_bus)
        while True:
            #SPI_0.spi_transmit_byte(BV(6))            
            # spi_write(spi, 0x00)
            # time.sleep(0.5)

            # CAN Message implementation
            j += 1

            if (j == k):
                j=0
                if(isLed == 0):                    
                    print("Try to send the message via CAN")
                    res = MSG.send_message()

                    if(res == MSG_result.CAN_OK):
                        print("Message was written to the buffer")
                    elif (res == MSG_result.CAN_MCP_ERROR):
                        print("Error (MCP_Error): could not send the message!")
                    elif (res == MSG_result.CAN_ALL_TX_BUSY):
                        print("Error ( ALL_TX_BUSY ): could not send the message!")
                    elif (res == MSG_result.CAN_ERROR):
                        print("Error ( ERROR ): could not send the message!")
                    elif (res == MSG_result.CAN_NOT_FOUND):
                        print("Error ( NOT_FOUND ): could not send the message!")
                    else:
                        print("Error (" + res + "could not send the message!")
            
            print("MCP2515: Loop")
            time.sleep(0.5)            
    except KeyboardInterrupt:
        SPI_0.spi_close()
        sys.exit(0)
