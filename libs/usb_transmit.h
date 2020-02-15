#include <usb_serial.h>
#include <avr/pgmspace.h>

// Send a string to the USB serial port.  The string must be in
// flash memory, using PSTR
void send_str(const char *s);

// Receive a string from the USB serial port.  The string is stored
// in the buffer and this function will not exceed the buffer size.
// A carriage return or newline completes the string, and is not
// stored into the buffer.
// The return value is the number of characters received, or 255 if
// the virtual serial connection was closed while waiting.
//
uint8_t recv_str(char *buf, uint8_t size);

/* Send the number in the hexadecimal format */
void writeInHex(unsigned char number);

/* Send the number in the decimal format */
void writeInDec(unsigned char code);