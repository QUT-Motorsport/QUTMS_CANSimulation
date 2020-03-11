#include "SPI.h"
#include "macros.h"
void spiMasterINIT() {
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1<<PIN_MOSI)|(1<<PIN_SCK)|(1<<PIN_SS);
    DDR_SPI &= ~(1<<PIN_MISO);

    /* Enable SPI, Master, set clock rate fck/4, mode 0,0 */
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPR1);
    //SPSR = (1<<SPI2X);
    SPI_INT_DDR &= ~SPI_INT;

    spiMasterChipSelect(1);     //CS and SS pins are the same. Keep this way
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
    if(state) {
        /* Upper the CS pin */
        SET_BIT(PORT_SPI, PIN_SS);        
    } else {
        /* Lower the CS pin */
        CLEAR_BIT(PORT_SPI, PIN_SS);        
    }
}

// Pointer to function which handle change on INT pin handler 
void (*int_handler)(void);

// Initialization of hardware ext. interrupts \param *handler pointer to a function which handle occured interrupt. * \return nothing
void extInterruptINIT(void (*handler)(void)) {
    // Set function pointer 
    int_handler = handler;
    // TODO: Initialize external interrupt on pin INT0 on failing edge 
    //MCUCR |= (1 << ISC01);
    //GICR |= (1 << INT0); 
}
// System interrupt handler 
SIGNAL(INT0_vect) {
    int_handler();
}