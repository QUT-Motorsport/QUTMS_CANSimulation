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
void control_usb_init(void);
void receive_can(void);

/* Main Routine*/
int main(void) {
    set_clock_speed(CPU_8MHz);

    //control_usb_init();
    
    spiMasterINIT();

    /////////////////////////////////////////////////////////////////////
    send_str(PSTR(
     "\r\nMCP2515-driver: logger & tracer\r\n"
    ));
    // snprintf(s_buffer, sizeof(s_buffer), "Current time \n");
    // usb_serial_send_here(s_buffer);
    // snprintf(score, sizeof(score), "Score \n");
    // usb_serial_send_here(score);

    /////////////////////////////////////////////////////////////////////

    /* Enable global interrupt flag */
    sei();
    _delay_ms(100);
    

    /* initialize CAN */
    char buf[48];
    uint8_t res = MCP2515_init();
    if (res == 0) {
        send_str(PSTR(
        "\r\nMCP2515 have initialised\r\n"
        ));
        if (mcp_loopback() == 0) {
            send_str(PSTR("\r\nMCP2515 test is successfull\r\n" ));
            _delay_ms(2000);
            //test_can();
            receive_can();
        }
    }
    else {
        snprintf(buf, sizeof(buf), "Error ( %d ): cannot address the MCP2515!\r\n", res);
        send_buffer( buf );		
	}    
    
    test_can();
    
}

void test_can() {
    CanMessage msg1;
	CAN_RESULT res;
    int isLed = 0;  
    int i, j, k;
        
    char buf[32];
    char s_buffer[15];

    init_msg(&msg1);
    msg1.id = 0x0A000000;
    msg1.ext_id = 1;
    msg1.rtr = 0;
    msg1.dlc = 5;
    msg1.dta[0] = 0x01;
    msg1.dta[2] = 0x01;
    msg1.dta[4] = 0x01;
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
                send_str(PSTR("Try to send the message via CAN\r\n" ));
                res = send_message(&msg1); 
                
                if (res == CAN_OK) {
                    send_str(PSTR("Message was written to the buffer\r\n" ));
                    
	            } else if (res == CAN_MCP_ERROR) {
                    send_str(PSTR("Error ( MCP_ERROR ): could not send the message!\r\n\r\n" ));
                } else if (res == CAN_ALL_TX_BUSY) {
                    send_str(PSTR("Error ( ALL_TX_BUSY ): could not send the message!\r\n\r\n" ));
                } else if (res == CAN_ERROR) {
                    send_str(PSTR("Error ( ERROR ): could not send the message!\r\n\r\n" ));
                } else if (res == CAN_NOT_FOUND) {
                    send_str(PSTR("Error ( NOT_FOUND ): could not send the message!\r\n\r\n" ));
                } else {
                    snprintf(buf, sizeof(buf), "Error ( %d ): could not send the message!\r\n\r\n", res);
                    send_buffer( buf );                    
	            } 
            }                         
            
            r = recv_str(buf, sizeof(int16_t));
            snprintf(s_buffer, sizeof(s_buffer), "%d", r);
            send_buffer( s_buffer );
            if ( r == 0 ) {
            } else if ( r == 'w' ) {
                isLed = 0;
                send_str(PSTR("isLed 0 again"));
                return;
            } else if ( r == 's' ) {
                send_str(PSTR("switch to sniffer"));
                return;
            }
             
            i++;              
            if (i>1000) i=0;
            
            send_str(PSTR("\r\nMCP2515: Loop\r\n"));
                        
        }   
    }
}

void control_usb_init(void){
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
}

void receive_can() 
{
	CanMessage message;

    send_str(PSTR("-Receive-\r\n" ));
    send_str(PSTR("Wait for messages to be received!\r\n\r\n" ));

    if (MCP2515_check_message()) {
        send_str(PSTR("Message received!\r\n" ));

        // read the message from the buffers
        if (read_message(&message) == CAN_OK) {
            send_str(PSTR("---- received message ---- \r\n" ));
            print_can_message(&message);
            send_str(PSTR("-successfull-\r\n" ));
        } else {
            send_str(PSTR("Can't read the message\r\n" ));
        }            			
    }    
}  
