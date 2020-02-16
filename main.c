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
#include "can.h"

void test_can(void);

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
    
    spiMasterINIT();

    send_str(PSTR("\r\n==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-"
     "\r\nMCP2515-driver: logger & tracer"
	 "\r\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n"
    ));

    /* initialize CAN */
    MCP2515_init();
    send_str(PSTR("\r\n==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-"
     "\r\nMCP2515 have initialised"
	 "\r\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n"
    ));

    /* Enable global interrupt flag */
    //sei();
    _delay_ms(100);
    
    test_can();
    
}

void test_can() {
    CanMessage msg1;
	//CAN_RESULT res;
    int isLed = 0;  
    int i, j, k;
    
    unsigned char cmd;
    char buf[32];

    init_msg(&msg1);
    msg1.id = 0x0125;
    msg1.rtr = 0;
    msg1.dlc = 2;
    msg1.dta[0] = 0x04;
    msg1.dta[1] = 0xf3;
    i = 0;
    j = 0;
    k = 5000;
    int16_t r;
    while (1) {
        _delay_us(500);
        j++;              
        
        if (j == k) {
            j=0;              
            
            if (isLed == 0) {                
                isLed = 1;    
                
    	        send_message(&msg1); 
                
                //snprintf(buf, sizeof(buf), "send can: %d", msg1.id);
                //send_str(PSTR(buf));
            }
            else {                    
                isLed = 0;
            }
                        
            snprintf(buf, sizeof(buf), "");        
            
            r = recv_str(buf, sizeof(int16_t));
            send_str((r));
            if ( r == 0 ) {
            }
            else if ( r == 'w' ) {
                send_str(PSTR("switch to sniffer"));
                return;
            }
            else if ( r == 's' ) {
                send_str(PSTR("switch to sniffer"));
                return;
            }
             
            i++;              
            if (i>1000) i=0;
            
            send_str(PSTR("\r\n==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-"
                        "\r\nMCP2515: Loop"
                        "\r\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n"));
                        
        }   
    }
}