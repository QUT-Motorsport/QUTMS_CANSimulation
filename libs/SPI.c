#include "SPI.h"
#include "macros.h"
void spiMasterINIT() {
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1<<PIN_MOSI)|(1<<PIN_SCK)|(1<<PIN_SS);
    DDRF = (1<<PIN_CS);
    PORT_SPI |= (1 << PIN_SS); 
    PORT_CS |= (1 << PIN_CS);    
        
    /* Enable SPI, Master, set clock rate fck/4, mode 0,0 */
    SPCR = (1<<SPE) | (1<<MSTR);
    SPSR = (1<<SPI2X);
}

unsigned char spiMasterTRANSMIT(unsigned char data) {
    /* Start transmission */
    SPDR = data;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));
    /* SPDR must be stored as quickly
        as possible (ref. ATMegaX ds) */
    return SPDR;
} 

void spiMasterChipSelect(unsigned char state) {
    /* What the user wants? (remember that the CS signal is inverted) */
    if(!state) {
        /* Upper the CS pin */
        SET_BIT(PORT_CS, PIN_CS);        
    } else {
        /* Lower the CS pin */
        CLEAR_BIT(PORT_CS, PIN_CS);        
    }
}

