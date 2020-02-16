#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <usb_serial.h>
#include <stdint.h>
#include <stdio.h>

#include "macros.h"
#include "SPI.h"
#include "usb_transmit.h"
#include "mcp2515.h"

/* Main Routine*/
int main(void) {
    set_clock_speed(CPU_8MHz);

    /* Initialise USB debugger session, waiting for terminal to connect*/
    usb_init();
    while (!usb_configured()) /* wait */ ;
	_delay_ms(1000);
    
    // wait for the user to run their terminal emulator program
    // which sets DTR to indicate it is ready to receive.
    while (!(usb_serial_get_control() & USB_SERIAL_DTR)) /* wait */ ;

    // discard anything that was received prior.  Sometimes the
    // operating system or other software will send a modem
    // "AT command", which can still be buffered.
    usb_serial_flush_input();
    
    send_str(PSTR("\r\n==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-"
     "\r\nMCP2515-driver: logger & tracer"
	 "\r\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n"
    ));

    /* initialize CAN */
    MCP2515_init();
}