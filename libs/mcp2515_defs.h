#ifndef	_MCP2515_DEFS_H
#define	_MCP2515_DEFS_H

#include "macros.h"


#define TXB_CNT         3       // Number of buffers for send
#define FILTR_CNT       6       // Number of Filters
#define ERRATA_REP_CNT	20      // Number of trials after failing to set mode
/**     SPI Interface commands
 * The MCP2515 is designed to interface directly with the
 * Serial Peripheral Interface (SPI) port  and supports
 * Mode 0,0 and Mode 1,1.
 * 
 * Reset interlal register/config mode:     1100 0000   =   0xC0
 * Read data from register at addr:         0000 0011   =   0x03
 * Reading a receive buffer:                1001 0nm0
 * Write data to register at addr:          0000 0010   =   0x02
 * RTS (Message Request to Send): TXBn      1000 0nnn   =   0x80
 * Quick poll to read status bits:          1010 0000   =   0xA0
 * Quick pooling filter match, message type 1011 0000
 * User to set/clear bits at registers:     0000 0101   =   0x05
 * 
*/
#define CMD_RESET        0xC0   // Reset interlal register/config mode: 1100 0000
#define CMD_READ         0x03   // Read data from register at addr: 0000 0011
#define CMD_WRITE        0x02   // Write data to register at addr: 0000 0010
#define CMD_RTS          0x80   // RTS (Message Request to Send): TXBn  1000 0nnn
#define CMD_RD_STATUS    0xA0   // Quick poll to read status bits: 1010 0000
#define CMD_BIT_MOD      0x05   // User to set/clear bits at registers: 0000 0101

/**     Work Mods
 * MCP25 operates in 5 modes. Each can be set by 
 * modefying CANCTRL registers. REQOP[0:2]
 * 
 * Normal Operation Mode:        000x xxxx   =   0x00
 * Sleep Mode:                   001x xxxx   =   0x20
 * Loopback Mode:                010x xxxx   =   0x40
 * Listen-Only Mode:             011x xxxx   =   0x60
 * Sets Configuration Mode:      100x xxxx   =   0x80
 * * All other values are invalid. At start and after rest
 * config mode set by default
 * 
 * The CLKOUT pin is provided to the system designer
 * for use as the main system clock or as a clock input
 * for other devices in the system. MAX 25MHz. Default SC/8.
 * F_CLCOUT     System Clock/1: 00
 * F_CLCOUT     System Clock/2: 01
 * F_CLCOUT     System Clock/4: 10
 * F_CLCOUT     System Clock/8: 11
*/
#define MODE_NORMAL   0x00      // Normal Operation Mode: 000x xxxx
#define MODE_SLEEP    0x20      // Sleep Mode: 001x xxxx
#define MODE_LOOP     0x40      // Loopback Mode: 010x xxxx
#define MODE_LISTEN   0x60      // Listen-Only Mode: 011x xxxx
#define MODE_CONFIG   0x80      // Sets Configuration Mode: 100x xxxx
#define MODE_MASK	  (MODE_NORMAL | MODE_SLEEP | MODE_LOOP | MODE_LISTEN | MODE_CONFIG)

/** Bit definition for CANCTRL. Page 60 */
#define REQOP2		7   // R/W-1: Operation Mode bits
#define REQOP1		6   // R/W-0: Operation Mode bits
#define REQOP0		5   // R/W-0: Operation Mode bits
#define ABAT		4   // R/W-0: Abort All Pending Transmissions
#define CLKEN		2   // R/W-1: One-Shot Mode - 1:OneTine 0:Reattempt
#define CLKPRE1		1   // R/W-1: CLKOUT Pin prescaler
#define CLKPRE0		0   // R/W-1: CLKOUT Pin prescaler

/**     Tx adress registers
 * TXBnCTRL - control register associated with the message buffer.
 * It determines the condition under  which the message will be send.
 * 
 * Addresses on Page 18: 30h,40h,50h.
 * TXB_BASE serves as TXB0CTRL, and TXB_Delta is a shift towards next one
 * 
 * TXP bits used to transmit message priority
 * Highest Message priority:            11
 * High intermediate message priority:  10
 * Low intermediate message priority:   01
 * Lowest Message Priority:             00
*/
#define TXB_BASE        0x30
#define TXB_DELTA       0x10
#define TXB0CTRL        TXB_BASE
#define TXB1CTRL        (TXB0CTRL + TXB_DELTA)
#define TXB2CTRL        (TXB1CTRL + TXB_DELTA)
// TXBn bits
#define ABTF    _BV(6)  // R-0: Message Aborted Flag bit - 1:Aborted 0:Succes
#define MLOA    _BV(5)  // R-0: Message Lost Arbitration bit - 1:Lost
#define TXERR   _BV(4)  // R-0: Transmission Error Detected bit - 1:Bus error during transmission
#define TXREQ   _BV(3)  // R/W-0: Message Transmit Request bit - 1: Buffer is pending in transmission
/*Currently unused*/
#define TXP1    _BV(1)  // R/W-0: Buffer Priority Bit
#define TXP0    _BV(0)  // R/W-0: Buffer Priority Bit

#define CTRL      0x00
#define SIDH      0x01
#define SIDL      0x02
#define EID8      0x03
#define EID0      0x04
#define DLC       0x05
#define D0        0x06


#define EXIDE    _BV(3)
#define RTR        _BV(6)

/**     Rx address registers
 * Two full receive buffers with multiple acceptance 
 * filters for each. There is also a separate Message
 * Assembly Buffer (MAB) that acts as a third receive
 * buffer.
 * 
 * Addresses on Page 27: 60h.
 * RXB_BASE serves as RXB0CTRL, and RXB_Delta is a shift towards next one
 * 
*/
#define RXB0CTRL    0x60
#define RXB_BASE    RXB0CTRL
#define RXB_DELTA   0x10
#define RXB1CTRL    (RXB_BASE + RXB_DELTA)

#define RXM1        _BV(6)  // R/W-0: Receive Buffer Operating bits
#define RXM0        _BV(5)  // R/W-0: Receive Buffer Operating bits
#define RXRTR       _BV(3)  // R-0: Received Remote Transfer Request
#define BUKT        _BV(2)  // R/W-0: Rollover Enable
/*Currently unused*/
#define BUKT1       _BV(1)  // R-0: Read-only copy of BUKT bit
#define FILHIT0     _BV(0)  // R-0: Filter Hit - indicates which acceptance filter enabled reception of message

#define NOFILTR_RX      (RXM1 | RXM0)
#define EXT_FILTR_RX    RXM1
#define STD_FILTR_RX    RXM0
#define ANY_FILTR_RX    (0)

/* CAN CONTROL REGISTER (ADDRESS: XFh) */
#define CANCTRL   0x0F
/* CAN STATUS REGISTER (ADDRESS: XEh) */
#define CANSTAT   0x0E

/* ADDRESS: 2Ah. SJW[1:0] - Synchronization Jump Width Length bits. BRP[5:0] Baud Rate Prescaler bits  T_Q = 2 x (BRP[5:0] + 1)/F_OSC */
#define CNF1      0x2A
#define SJW1		7
#define SJW0		6
#define BRP5		5
#define BRP4		4
#define BRP3		3
#define BRP2		2
#define BRP1		1
#define BRP0		0

/* ADDRESS 29h. BTLMODE: PS2 Bit Time Length bit. SAM (Sample Point Configuration). PHSEG1[2:0]: PS1 Length bits (PHSEG1[2:0] + 1) x T_Q. PRSEG[2:0]: Propagation Segment Length bits
(PRSEG[2:0] + 1) x T_Q.*/
#define CNF2      0x29
#define BTLMODE		7
#define SAM			6
#define PHSEG12		5
#define PHSEG11		4
#define PHSEG10		3
#define PHSEG2		2
#define PHSEG1		1
#define PHSEG0		0

/*SOF - State-of-Frame. WAKFIL: Wake-up Filter bit. PHSEG2[2:0] PS2 Length bits. (PHSEG2[2:0] + 1) x T_Q. Minimum valied for PS2 2T_Qs*/
#define CNF3      0x28
#define WAKFIL		6   // Wake-up Filter
#define PHSEG22		2   // PS2 Length bits
#define PHSEG21		1   // (PHSEG2[2:0] + 1) x T_Q
#define PHSEG20		0   // ???

/**/
#define BFPCTRL   0x0C
#define EFLG      0x2D
#define TEC       0x1C
#define REC       0x1D

#define TXRTSCTRL 0x0D
#define B2RTS        _BV(5)
#define B1RTS        _BV(4)
#define B0RTS        _BV(3)
#define ANYRTS        (B2RTS | B1RTS | B0RTS)

#define B1BFS        _BV(5)
#define B0BFS        _BV(4)
#define ANYBFS        (B1BFS | B0BFS)

/* mask */
#define RXM0SIDH  0x20
#define RXM1SIDH  0x24
/* filters */
#define RXF0SIDH  0x00
#define RXF1SIDH  0x04
#define RXF2SIDH  0x08
#define RXF3SIDH  0x10
#define RXF4SIDH  0x14
#define RXF5SIDH  0x18

/* interrupts */
#define CANINTE   0x2B
#define CANINTF   0x2C

// Interupt flags
/// error
#define MERRIE        _BV(7)
#define MERRIF        _BV(7)
#define MERRI        _BV(7)
/// Activity on a CAN bus
#define WAKIE        _BV(6)
#define WAKIF        _BV(6)
#define WAKI        _BV(6)
/// Global interrupt allowance upon error
#define ERRIE        _BV(5)
#define ERRIF        _BV(5)
#define ERRI        _BV(5)
/// Buffer transmission 2 empty
#define TX2IE        _BV(4)
#define TX2IF        _BV(4)
#define TX2I        _BV(4)
/// Buffer transmission 1 empty
#define TX1IE        _BV(3)
#define TX1IF        _BV(3)
#define TX1I        _BV(3)
/// Buffer transmission 0 empty
#define TX0IE        _BV(2)
#define TX0IF        _BV(2)
#define TX0I        _BV(2)
///Buffer transmission 1 empty
#define RX1IE        _BV(1)
#define RX1IF        _BV(1)
#define RX1I        _BV(1)
/// Buffer transmission 0 empty
#define RX0IE        _BV(0)
#define RX0IF        _BV(0)
#define RX0I        _BV(0)

#endif
