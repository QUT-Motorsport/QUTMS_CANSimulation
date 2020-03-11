import time
import numpy as np

import pi_libs.MCP2515_defs as defs

class MCP2515:    
    
    def __init__(self, spi):
        """
        Initialises MCP2515 controller at
        the inputted SPI channels
        """
        self.SPI = spi        

    # -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
    # MCP2515 register manipulation
    # -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=

    def read_buf(self, addr, count):
        """
        Reads a buffer from MCP2515 register
        
        @param addr - Selecting address to read
        @param count - Number of times reading buffer
        """
        buffer = [defs.CMD_READ, addr]
        # buf.append(self.SPI.spi_transmit_buffer([defs.CMD_READ]))
        # buf.append(self.SPI.spi_transmit_buffer([addr]))
        
        for i in range(count):
            buffer.append(0x00)
        buf = self.SPI.spi_transmit_buffer([buffer])
        buf.pop(0)
        buf.pop(0)
        return buf
    
    def read_byte(self, addr):
        """
        Reads a byte from register
    
        @param addr Address to read from
        @return 8bit int resulted value
        TODO: seed if read_buf call more efficient
        """
        buffer = [defs.CMD_READ, addr, 0x00]
        buf = self.SPI.spi_transmit_buffer(buffer)
        print(buf)
        buf.pop(0)
        buf.pop(0)
        #print(tmp)
        #tmp = self.read_buf(addr, 2)        
        return buf[0]
        # return self.read_buf(addr, 1)[0][0]
    
    def write_buf(self, addr, buff, count):
        """
        Write to a buffer of MCP2515 register
        
        @param addr - Selecting address to write
        @param buf - Buffer[] array which will be written
        @param count - Number of time writing buffer
        """
        buffer = [defs.CMD_WRITE, addr]
        #self.SPI.spi_transmit_buffer([defs.CMD_WRITE])
        #self.SPI.spi_transmit_buffer([addr])
        # print("count: ")
        # print(count)
        # print(buff)
        for i in range(count):            
            #self.SPI.spi_transmit_buffer([buff[i]])
            buffer.append(buff[i])
            # self.SPI.spi_transmit_buffer(buff[i])
        self.SPI.spi_transmit_buffer(buffer)
        
    
    def write_byte(self, addr, data):        
        """        
        Write a byte to resgister
        
        @param addr - Address to write in
        @param data - 8bit data[] array to send        
        """
        #self.write_buf(addr,data,1)
        print([defs.CMD_WRITE, addr, data])
        self.SPI.spi_transmit_buffer([defs.CMD_WRITE, addr, data])

    # -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
    # MCP2515 chip control
    # -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
    
    def reset_software(self):
        """
        Perform Software Reset
        """
        #print(np.uint8((defs.CMD_RESET)))
        #print(type((defs.CMD_RESET)))
        self.SPI.spi_transmit_buffer([defs.CMD_RESET])        
        #self.SPI.spi_transmit_buffer([0xC0])
        #self.SPI.spi_transmit_byte(np.uint8(defs.CMD_RESET))

    def reset_hardware():        
        """
        Platform dependent hardware reset toggeling
        reset button.
        """
        #GPIO LOW
        time.sleep(0.3)
        #GPIO HIGH
    
    
    def status(self):
        """
        Read status of MCP2515 chip
        
        @return 8-bit status register
        """        
        return self.SPI.spi_transmit_buffer([defs.CMD_RD_STATUS, 0x00, 0x00])[1]
    
    def start_send(self, TXB_mask):
        """
        Send command for starting transmission
        
        @param TXB_mask 
        """
        self.SPI.spi_transmit_buffer([defs.CMD_RTS | (TXB_mask & 0x07)])
    
    def bit_modify(self, addr, mask, data):
        """
        Modefy bits in the MCP25** registers
        Function performs XOR for bits based on the masks.
        As funtion supported by not all registers MCP25**,
        function checks the valid addr and ignores others.
        
        @param addr address of register
        @param mask Mask for bit modefy
        @param data value of bits
        """
        if(addr == defs.CANCTRL):
            self.SPI.spi_transmit_buffer([defs.CMD_BIT_MOD, addr, mask, data])
            return 1
        else:
            return 0
    
    def set_mask(self, id, ext, mask):
        """
        Sets mask to RXM0SIDH registers
        param id 1 - RXM1SIDH, other - RMX0SIDH
        """
        mr = 0
        if(id):
            mr = defs.RXM1SIDH
        else:
            mr = defs.RXM0SIDH
        if(ext):
            mr += 1
            self.write_byte(mr, mask >> 16)
            mr += 1
            self.write_byte(mr, mask >> 8)
            mr += 1
            self.write_byte(mr, mask)
        else:
            self.write_byte(mr, mask >> 3)
            mr += 1
            self.write_byte(mr, mask << 5)
    
    def set_filtr(self, id, ext, filtr):
        """
        Sets filter for accepted messages.
        Works only in Config mode
        
        @param id - ID of filter
        @param ext - flag for 0 as notmal, 1 extended
        @param filtr - Mask of the filter itself        
        """
        fr = defs.RXF0SIDH + id*4
        if(id >= 3):
            fr += 4
        if(ext):		
            fr += 1
            self.write_byte(fr, defs.EXIDE | (filtr >> 16))
            fr += 1
            self.write_byte(fr, filtr >> 8)
            fr += 1
            self.write_byte(fr, filtr)
        else:
            self.write_byte(fr, filtr >> 3)
            fr += 1
            self.write_byte(fr, filtr << 5)

    # -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
    # MCP2515 MODE CHANGE
    # -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
    def set_mode(self, mode):
        """        
        Set mode
        @param mode - 8bit mode select
        @return Status of set. -1 if fail.
        """
        tmp = self.read_byte(defs.CANCTRL)         
        tmp &= ~defs.MODE_MASK
        # print(mode)
        # print(tmp)
        # print([tmp | mode])
        self.write_byte(defs.CANCTRL, tmp | mode)
        
        # TODO: Does nopt go back to Normal from config
        # print("If statement ---")
        # print(self.read_byte(defs.CANCTRL))
        # print(self.read_buf(defs.CANCTRL,1))
        # print(defs.MODE_MASK)
        # print(self.read_byte(defs.CANCTRL) & defs.MODE_MASK)
        # print("------")
        if((self.read_byte(defs.CANCTRL) & defs.MODE_MASK) == mode):
            return 0
        else:
            return -1
        # tmp = self.read_buf(defs.CANCTRL)[2]         
        # tmp = tmp[0]        
        # tmp &= ~defs.MODE_MASK
        # self.write_buf(defs.CANCTRL, [tmp | mode], 1)
        
        # # TODO: Does nopt go back to Normal from config
        # tmp2 = self.read_buf(defs.CANCTRL,1)[0][0]
        
        # if((tmp2 & defs.MODE_MASK) == mode):
        #     return 0
        # else:
        #     return -1
    
    def mode_normal(self):
        """
        Set normal mode
        @return - result of transmission        
        """
        for i in range(defs.ERRATA_REP_CNT):
            #self.set_mode(defs.MODE_NORMAL)
            #time.sleep(0.10)
            if(self.set_mode(defs.MODE_NORMAL) == 0):
                return 0
            else:
                self.set_mode(defs.MODE_CONFIG)
                print("Set Mode to Normal failed")
                time.sleep(0.001)
        return -1
    
    def mode_config(self):
        """
        Set Configuration mode
        @return - result of transmission
        """
        return self.set_mode(defs.MODE_CONFIG)
    
    def mode_sleep(self):
        """
        Set Sleep mode
        @return - result of transmission
        """
        return self.set_mode(defs.MODE_SLEEP)
    
    def mode_listen(self):
        """
        Set Listen mode
        @return - result of transmission
        """
        return self.set_mode(defs.MODE_LISTEN)
    
    def mode_loopback(self):
        """
        Set Loopback mode
        @return - result of transmission
        """
        return self.set_mode(defs.MODE_LOOP)
    
    def get_rt_pin(self):
        """
        Returns the levels on input pins
        @return RTS bits
        """
        return self.read_byte(defs.TXRTSCTRL) & defs.ANYRTS
    
    def set_bf_pin(self):
        """
        Set the levels on input pins
        @param data bit mask of levels
        """
        return self.write_byte(BFPCTRL, [data & ANYBFS])
    
    # def check_message(self):
    #     SPI_INT_DDR   DDRD
    #     SPI_INT_PORT  PORTD
    #     SPI_INT       (1 << (PIND1))

    def MCP2515_init(self):
        print("\r\nMCP2515: Software Reset\r\n")
        self.reset_software()

        # self.mode_config()
        # configurate speed - bitrate 125 kbps at 16MHz
        # activate interupt

        # [0x02 \ 0x02 0x90 0x07 0x06]
        self.SPI.spi_transmit_buffer([
            defs.CMD_WRITE,
            defs.CNF3,
            (1<<defs.PHSEG21),      # Bitrate 125 kbps at 16 MHz
            ((1<<defs.BTLMODE)|(1<<defs.PHSEG11)),
            ((1<<defs.BRP2)|(1<<defs.BRP1)|(1<<defs.BRP0)),
            # Activate interupt
            ((1<<defs.RX1IE)|(1<<defs.RX0IE))
        ])
        
        # self.SPI.spi_transmit_buffer(
        #     [defs.CMD_WRITE, defs.CNF3, (1<<defs.PHSEG21)])
        # self.SPI.spi_transmit_buffer(
        #     [((1<<defs.BTLMODE)|(1<<defs.PHSEG11)), ((1<<defs.BRP2)|(1<<defs.BRP1)|(1<<defs.BRP0)), ((1<<defs.RX1IE)|(1<<defs.RX0IE))])

        #time.sleep(0.1)
        # test if we could read back the value => is the chip accessible? 
        # if (self.read_byte(defs.CNF3) != ((1<<defs.BRP2)|(1<<defs.BRP1)|(1<<defs.BRP0))):
        #     print("\r\nMCP2515: CNF3 not passed\r\n")

        # print(self.read_buf(defs.CNF1,1))
        tmp = self.read_byte(defs.CNF1)
        
        print(tmp)
        print(((1<<defs.BRP2)|(1<<defs.BRP1)|(1<<defs.BRP0)))

        if (tmp != ((1<<defs.BRP2)|(1<<defs.BRP1)|(1<<defs.BRP0))):
        # if (self.read_buf(defs.CNF1,1)[2]  != ((1<<defs.BRP2)|(1<<defs.BRP1)|(1<<defs.BRP0))):
            print("\r\nMCP2515: CNF1 not passed\r\n")
            return -1
        
        print("\r\nMCP2515: Setting Mask\r\n")
        # deactivate the RXnBF Pins (High Impedance State)
        self.write_byte(defs.BFPCTRL, 0)

        # set TXnRTS as inputs
        self.write_byte(defs.TXRTSCTRL, 0)

        # turn off filters => receive any message
        self.write_byte(defs.RXB0CTRL, defs.RXM1 | defs.RXM0)
        self.write_byte(defs.RXB1CTRL, defs.RXM1 | defs.RXM0)

        self.set_mask(1, 0, 0x7ff)
        print("\r\nMCP2515: Deactivating remaining Filters\r\n")
        
        for i in range(2, defs.FILTR_CNT):
            self.set_filtr(i,0,0x7FF)
        
        print("\r\nMCP2515: Changing to Normal Mode\r\n")

        if(self.mode_normal() != 0):
            return -2
        
        return 0