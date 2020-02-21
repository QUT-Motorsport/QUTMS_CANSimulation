#include "usb_transmit.h"

uint8_t control_usb_is = 0;

void send_str(const char *s) {

	if (!control_usb_is) return;

	char c;
	while (1) {
		c = pgm_read_byte(s++);
		if (!c) break;
		usb_serial_putchar(c);
	}
}

uint8_t recv_str(char *buf, uint8_t size) {
	int16_t r;
	uint8_t count=0;

	while (count < size) {
		r = usb_serial_getchar();
		if (r != -1) {
			if (r == '\r' || r == '\n') return count;
			if (r >= ' ' && r <= '~') {
				*buf++ = r;
				usb_serial_putchar(r);
				count++;
			}
		} else {
			if (!usb_configured() ||
			  !(usb_serial_get_control() & USB_SERIAL_DTR)) {
				// user no longer connected
				return 255;
			}
			// just a normal timeout, keep waiting
		}
	}
	return count;
}

void send_buffer(char * buffer) {
	if (control_usb_is)
		usb_serial_write((uint8_t *) buffer, strlen(buffer));
}
void writeInHex(unsigned char number) {
    send_str(PSTR("\r\n writeInHex \r\n"));
    send_str(PSTR("\r\n writeInHex \r\n"));
}

void writeInDec(unsigned char code) {
    send_str(PSTR("\r\n writeInDec \r\n"));
    send_str(PSTR("\r\n writeInDec \r\n"));
    send_str(PSTR("\r\n writeInDec \r\n"));
}