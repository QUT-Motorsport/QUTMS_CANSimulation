from pi_libs.macros import BV

# Define MCP commands registers
CMD_RESET       =   0xC0
CMD_READ        =   0x03
CMD_WRITE       =   0x02
CMD_RTS         =   0x80
CMD_RD_STATUS   =   0xA0
CMD_BIT_MOD     =   0x05

# Number of buffers for send
TXB_CNT         =   3
# Number of Filters
FILTR_CNT       =   6

# Tx adress registers
TXB_BASE        =   0x30
TXB_DELTA       =   0x10
TXB0CTRL        =   TXB_BASE
TXB1CTRL        =   (TXB0CTRL + TXB_DELTA)
TXB2CTRL        =   (TXB1CTRL + TXB_DELTA)
CTRL            =   0x00
SIDH            =   0x01
SIDL            =   0x02
EID8            =   0x03
EID0            =   0x04
DLC             =   0x05
D0              =   0x06

# TXBn bits
TXREQ           =   BV(3)
TXERR           =   BV(4)
MLOA            =   BV(5)
ABTF            =   BV(6)

EXIDE           =   BV(3)
RTR             =   BV(6)

# Rx
RXB0CTRL        =   0x60
RXB_BASE        =   RXB0CTRL
RXB_DELTA       =   0x10
RXB1CTRL        =   (RXB_BASE + RXB_DELTA)

RXM1            =   BV(6)
RXM0            =   BV(5)
BUKT            =   BV(2)
RXRTR           =   BV(3)
NOFILTR_RX      =   (RXM1 | RXM0)
EXT_FILTR_RX    =   RXM1
STD_FILTR_RX    =   RXM0
ANY_FILTR_RX    =   (0)

# MCP2510 address registers
CANCTRL         =   0x0F
CANSTAT         =   0x0E
CNF1            =   0x2A
CNF2            =   0x29
CNF3            =   0x28
BFPCTRL         =   0x0C
EFLG            =   0x2D
TEC             =   0x1C
REC             =   0x1D

TXRTSCTRL       =   0x0D
B2RTS           =   BV(5)
B1RTS           =   BV(4)
B0RTS           =   BV(3)
ANYRTS          =   (B2RTS | B1RTS | B0RTS)

B1BFS           =   BV(5)
B0BFS           =   BV(4)
ANYBFS          =   (B1BFS | B0BFS)

# mask
RXM0SIDH        =   0x20
RXM1SIDH        =   0x24
# filters
RXF0SIDH        =   0x00
RXF1SIDH        =   0x04
RXF2SIDH        =   0x08
RXF3SIDH        =   0x10
RXF4SIDH        =   0x14
RXF5SIDH        =   0x18

# Work Mods
MODE_NORMAL     =   0x00
MODE_SLEEP      =   0x20
MODE_LOOP       =   0x40
MODE_LISTEN     =   0x60
MODE_CONFIG     =   0x80
MODE_MASK       =   (MODE_NORMAL | MODE_SLEEP | MODE_LOOP | MODE_LISTEN | MODE_CONFIG)

ERRATA_REP_CNT  =   20

# interrupts
CANINTE         =   0x2B
CANINTF         =   0x2C

# Interupt flags
# error
MERRIE          =   BV(7)
MERRIF          =   BV(7)
MERRI           =   BV(7)
# Activity on a CAN bus
WAKIE           =   BV(6)
WAKIF           =   BV(6)
WAKI            =   BV(6)
# Global interrupt allowance upon error
ERRIE           =   BV(5)
ERRIF           =   BV(5)
ERRI            =   BV(5)
# Buffer transmission 2 empty
TX2IE           =   BV(4)
TX2IF           =   BV(4)
TX2I            =   BV(4)
# Buffer transmission 1 empty
TX1IE           =   BV(3)
TX1IF           =   BV(3)
TX1I            =   BV(3)
# Buffer transmission 0 empty
TX0IE           =   BV(2)
TX0IF           =   BV(2)
TX0I            =   BV(2)
# Buffer transmission 1 empty
RX1IE           =   BV(1)
RX1IF           =   BV(1)
RX1I            =   BV(1)
# Buffer transmission 0 empty
RX0IE           =   BV(0)
RX0IF           =   BV(0)
RX0I            =   BV(0)

# Bit definition for CANCTRL
REQOP2          =   7
REQOP1          =   6
REQOP0          =   5
ABAT            =   4
CLKEN           =   2
CLKPRE1         =   1
CLKPRE0         =   0

# Bit definition for CNF3
WAKFIL          =   6
PHSEG22         =   2
PHSEG21         =   1
PHSEG20         =   0

# Bit definition for CNF2
BTLMODE         =   7
SAM             =   6
PHSEG12         =   5
PHSEG11         =   4
PHSEG10         =   3
PHSEG2          =   2
PHSEG1          =   1
PHSEG0          =   0

# Bit definition for CNF1
SJW1            =   7
SJW0            =   6
BRP5            =   5
BRP4            =   4
BRP3            =   3
BRP2            =   2
BRP1            =   1
BRP0            =   0