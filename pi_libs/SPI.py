import spidev

class SPI:
    
    def __init__(self):
        """
        Initialization of the SPI hardware 
        interface - configure this
        device as master, set mode 0,0 and the communication speed
        250kHz    
        """
        self.spi = spidev.SpiDev()
        self.spi.open(0, 0)
        self.spi.max_speed_hz = 250000    

    
    def spi_transmit_byte(self, input):
        """
        Split an integer input into a two byte array to send via SPI
        TODO: rewritten as buffer due to no proper use
        """
        msb = input >> 8
        lsb = input & 0xFF
        return self.spi.xfer([msb, lsb])
        #return self.spi.xfer([input])
    
    
    def spi_transmit_buffer(self, input):
        """
        Send small sized buffers []
        """
        return self.spi.xfer(input)
    
    
    def spi_close(self):
        """
        Closes an SPI and relesing pins
        """
        self.spi.xfer([0x00])
        self.spi.close()