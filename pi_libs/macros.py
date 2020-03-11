"""
    CAB202 Teensy Library (cab202_teensy)
    macros.h rewritten to Python code
 
    B.Talbot, September 2015
    L.Buckingham, September 2017
    M.Sadykov, November 2019
    Queensland University of Technology
"""

"""
    Setting data directions in a data direction register (DDR)
"""
def SET_INPUT(portddr, pin):            (portddr) &= ~(1 << (pin))
def SET_OUTPUT(portddr, pin):           (portddr) |= (1 << (pin))

"""
    Setting, clearing, and reading bits in registers.
    reg is the name of a register; pin is the index (0..7)
    of the bit to set, clear or read.
    (WRITE_BIT is a combination of CLEAR_BIT & SET_BIT)
"""
def SET_BIT(reg, pin):                  (reg) |= (1 << (pin))
def CLEAR_BIT(reg, pin):                (reg) &= ~(1 << (pin))
def WRITE_BIT(reg, pin, value):
    (reg) = (((reg) & ~(1 << (pin))) | ((value) << (pin)))
def BIT_VALUE(reg, pin):                (((reg) >> (pin)) & 1)
def BIT_IS_SET(reg, pin):               (BIT_VALUE((reg),(pin))==1)

"""
    Rudimentary math macros
"""
def ABS(x):
    if ((x) >= 0):
        return (x)
    else:
        return -(x)
def SIGN(x):
    if (((x) > 0) - ((x) < 0)):
        return True
    else:
        return False

"""
    Define definitions as in docs.
"""
HIGH    =       1
LOW     =       0

"""
    Bit Write
"""
def BV(bit):
    return (1 << (bit))


