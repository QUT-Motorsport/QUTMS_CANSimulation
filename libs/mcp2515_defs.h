#ifndef	_MCP2515_DEFS_H
#define	_MCP2515_DEFS_H

#include "macros.h"

#define CMD_RESET        0xC0
#define CMD_READ         0x03
#define CMD_WRITE        0x02
#define CMD_RTS          0x80
#define CMD_RD_STATUS    0xA0
#define CMD_BIT_MOD      0x05

/// Number of Filters
#define FILTR_CNT    6

/* Tx adress registers */
#define TXB_BASE  0x30
#define TXB_DELTA    0x10
#define TXB0CTRL    TXB_BASE
#define TXB1CTRL    (TXB0CTRL + TXB_DELTA)
#define TXB2CTRL    (TXB1CTRL + TXB_DELTA)
#define CTRL      0x00
#define SIDH        0x01
#define SIDL      0x02
#define EID8      0x03
#define EID0      0x04
#define DLC         0x05
#define D0          0x06

// TXBn bits
#define TXREQ    _BV(3)
#define TXERR    _BV(4)
#define MLOA    _BV(5)
#define ABTF    _BV(6)

#define EXIDE    _BV(3)
#define RTR        _BV(6)

/* Rx */
#define RXB0CTRL    0x60
#define RXB_BASE      RXB0CTRL
#define RXB_DELTA    0x10
#define RXB1CTRL    (RXB_BASE + RXB_DELTA)

#define RXM1    _BV(6)
#define RXM0    _BV(5)
#define BUKT    _BV(2)
#define RXRTR    _BV(3)
#define NOFILTR_RX    (RXM1 | RXM0)
#define EXT_FILTR_RX    RXM1
#define STD_FILTR_RX    RXM0
#define ANY_FILTR_RX    (0)

/* MCP2510 address registers */
#define CANCTRL   0x0F
#define CANSTAT   0x0E
#define CNF1      0x2A
#define CNF2      0x29
#define CNF3      0x28
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

/* Work Mods */
#define MODE_NORMAL   0x00
#define MODE_SLEEP    0x20
#define MODE_LOOP     0x40
#define MODE_LISTEN   0x60
#define MODE_CONFIG   0x80
#define MODE_MASK	  (MODE_NORMAL | MODE_SLEEP | MODE_LOOP | MODE_LISTEN | MODE_CONFIG)

#define ERRATA_REP_CNT	20

#endif
