#ifndef SPI_H
#define SPI_H

#include "avr/io.h"
#include "avr/interrupt.h" 

/* Platform dependent Registers, Ports and Pins*/
#define DDR_SPI     DDRB    /* Data dir. register for port with SPI */
#define PORT_SPI    PORTB   /* Port with SPI */
#define PIN_MOSI    PINB2     /* MOSI pin on the PORTB_SPI */
#define PIN_MISO    PINB3     /* MISO pin on the PORTB_SPI */
#define PIN_SCK     PINB1     /* SCK pin on the PORTB_SPI */
#define PIN_SS      PINB0     /* SS pin on the PORTB_SPI */

#define PORT_CS     PORTB
#define PIN_CS      PINB0

#define SPI_INT_DDR   DDRD
#define SPI_INT_PORT  PORTD
#define SPI_INT       (1 << (PIND1))

/** \brief Initialization of the SPI interface on the MCU
 * Initialization of the SPI hardware interface - configure this
 * device as master, set mode 0,0 and the communication speed (there
 * is limitation - 10Mhz, nominal speed should be >8Mhz, for this
 * purpose.
 * 
 * \warning This is platform-dependent method!  
 */
void spiMasterINIT();

/** \brief Transmiting databytes via the SPI
 * 
 * This function is transmitting data via the SPI interface. Input
 * parameter is uns. char array. Data are transmited from the zero
 * index 
 * 
 * \warning This is platform-dependent method!
 * \param data[] Source data array
 * \param length Array length
 * 
 */
unsigned char spiMasterTRANSMIT(unsigned char data);

/** \brief Settings of the CS pin
 * 
 * This function is used for setting of the CS pin. CS signal
 * is inverted, so input 1 (true) means zero on the output.
 * Otherwise is analogically the same.
 * 
 * \warning This is platform-dependent method!
 * \param state Wished state
 */
void spiMasterChipSelect(unsigned char state);

/** Initialization of hardware ext. interrupts
 * \param *handler pointer to a function which handle occured interrupt.
 * \return nothing
 */
//void extInterruptINIT(void (*handler)(void));

#endif