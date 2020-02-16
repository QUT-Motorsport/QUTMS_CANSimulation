#include "mcp2515.h"

// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
// MCP2515 register manipulation
// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
void MCP2515_read_buf(uint8_t addr, uint8_t* buf, uint8_t count) {
    spiMasterChipSelect(1);             // CS=0
    spiMasterTRANSMIT(CMD_READ);		// COMMAND
	spiMasterTRANSMIT(addr);			// ADDRESS
	for(;count;count--)
		*buf++ = spiMasterTRANSMIT(0x00);// READING
	spiMasterChipSelect(0);				// CS=1
}

void MCP2515_write_buf(uint8_t addr, uint8_t* buf, uint8_t count) {
    spiMasterChipSelect(1);             // CS=0
    spiMasterTRANSMIT(CMD_WRITE);		// COMMAND
	spiMasterTRANSMIT(addr);			// ADDRESS
	for(;count;count--)
        spiMasterTRANSMIT(*buf++);		// WRITING
	spiMasterChipSelect(0);				// CS=1
}

uint8_t MCP2515_read_byte(uint8_t addr) {
    uint8_t tmp;
    MCP2515_read_buf(addr,&tmp,1);
    return tmp;
}

void MCP2515_write_byte(uint8_t addr, uint8_t data) {
    MCP2515_write_buf(addr,&data,1);
}

// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
// MCP2515 chip control
// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
void MCP2515_reset(void) {
	spiMasterTRANSMIT(1);
	spiMasterTRANSMIT(CMD_RESET);
	spiMasterTRANSMIT(0);
}

void MCP2515_hardware_reset(void) {    
    CLEAR_BIT(MCP2515_PORT_RESET, MCP2515_PIN_RESET);
    _delay_ms(300);
    SET_BIT(MCP2515_PORT_RESET, MCP2515_PIN_RESET);
}

uint8_t MCP2515_status(void) {
	uint8_t tmp;
	spiMasterChipSelect(1);
	spiMasterTRANSMIT(CMD_RD_STATUS);
	tmp = spiMasterTRANSMIT(0x00);
	spiMasterTRANSMIT(0x00);
	spiMasterChipSelect(0);
	return tmp;
}

void MCP2515_start_send(uint8_t TXB_mask) {
	spiMasterChipSelect(1);
	spiMasterTRANSMIT(CMD_RTS | (TXB_mask & 0x07));
	spiMasterChipSelect(0);
}

void MCP2515_bit_modify(uint8_t addr, uint8_t mask, uint8_t data) {
	switch(addr){
        case TXB_BASE:
        case TXB_BASE+0x10:
        case TXB_BASE+0x20:
        case RXB0CTRL:
        case RXB1CTRL:
        case CNF1:
        case CNF2:
        case CNF3:
        case EFLG:
        case BFPCTRL:
        case BFPCTRL+1:
        case CANCTRL: //
            spiMasterChipSelect(1);
            spiMasterTRANSMIT(CMD_BIT_MOD);
            spiMasterTRANSMIT(addr);
            spiMasterTRANSMIT(mask);
            spiMasterTRANSMIT(data);
            spiMasterChipSelect(0);
        default:
            return;
	}
}

void MCP2515_set_mask(uint8_t id, uint8_t ext, uint32_t mask) {
	uint8_t mr;
	if(id)
		mr = RXM1SIDH;
	else
		mr = RXM0SIDH;
	if(ext){		
		mr++;
		MCP2515_write_byte(mr++, mask >> 16);
		MCP2515_write_byte(mr++, mask >> 8);
		MCP2515_write_byte(mr, mask);
	} else {		
		MCP2515_write_byte(mr++, mask >> 3);
		MCP2515_write_byte(mr, mask << 5);
	}
}

void MCP2515_set_filtr(uint8_t id, uint8_t ext, uint32_t filtr) {
	uint8_t fr;	
	fr = RXF0SIDH + id*4;	
	if(id >= 3) fr += 4;
	if(ext){		
		fr++;
		MCP2515_write_byte(fr++, EXIDE | (filtr >> 16));
		MCP2515_write_byte(fr++, filtr >> 8);
		MCP2515_write_byte(fr, filtr);
	} else {		
		MCP2515_write_byte(fr++, filtr >> 3);
		MCP2515_write_byte(fr, filtr << 5);
	}
}

// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
// MCP2515 MODE CHANGE
// -=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=
uint8_t MCP2515_set_mode(uint8_t mode) {
	uint8_t tmp;
	tmp = MCP2515_read_byte(CANCTRL);
	tmp &= ~MODE_MASK;
	MCP2515_write_byte(CANCTRL, tmp | mode);
	if((MCP2515_read_byte(CANSTAT) & MODE_MASK) == mode)
		return 0;
	else
		return -1;
}

uint8_t MCP2515_mode_normal(void) {
    uint8_t i;
	
    // ERRATA    
	for ( i=0; i< ERRATA_REP_CNT; i++ ) {
        if(MCP2515_set_mode(MODE_NORMAL) == 0) {
			return 0;
		} else {
			MCP2515_set_mode(MODE_CONFIG);
			_delay_us(10);
		}
	}   
	return -1;
}

uint8_t MCP2515_mode_config(void) {
	return MCP2515_set_mode(MODE_CONFIG);
}

uint8_t MCP2515_mode_sleep(void) {
	return MCP2515_set_mode(MODE_SLEEP);
}

uint8_t MCP2515_mode_listen(void) {
	if(MCP2515_set_mode(MODE_LOOP) == 0)
		return MCP2515_set_mode(MODE_LISTEN);
	else
		return -1;
}

uint8_t MCP2515_mode_loopback(void) {
	return MCP2515_set_mode(MODE_LOOP);
}

uint8_t MCP2515_get_rt_pin(void) {
	return MCP2515_read_byte(TXRTSCTRL) & ANYRTS;
}

void MCP2515_set_bf_pin(uint8_t data) {
	MCP2515_write_byte(BFPCTRL, data & ANYBFS);
}

uint8_t MCP2515_init(void) {
    uint8_t i;
    DDRF = (1<<MCP2515_PIN_RESET);
    
    // Make reset HIGH to disable it    
    SET_BIT(MCP2515_PORT_RESET, MCP2515_PIN_RESET);
    _delay_ms(300);
    
    // Perform hard reset
	MCP2515_hardware_reset();

    // Perform soft reset
    MCP2515_reset();
	// TODO
    //SJW = 0(1),BRP = 4(5)--> number in brackets is actual value, as mcp2515 adds 1.
	MCP2515_write_byte(CNF1, 0x04);
    
    //BTL = 1, SAM = 1, PHSEG1 = 001(2), PRSEG = 010 (3)
	MCP2515_write_byte(CNF2, 0xCA);
    //SOF = 0, WAKFIL = 0, PHSEG2 = 001(2).
	MCP2515_write_byte(CNF3, 0x01);

	// 0x27
	// TODO 
	MCP2515_set_mask(0,0,0x27);
	MCP2515_set_filtr(0,0,0x27);
	MCP2515_set_filtr(1,0,0x7FF);
	MCP2515_write_byte(RXB0CTRL,NOFILTR_RX | BUKT);	
	MCP2515_write_byte(RXB1CTRL,STD_FILTR_RX);

	MCP2515_set_mask(1,0,0x7FF);
	for(i=2; i < FILTR_CNT; i++)
        MCP2515_set_filtr(i,0,0x7FF);

	return MCP2515_mode_normal();
}