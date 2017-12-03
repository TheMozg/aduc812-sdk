#include "aduc812.h"

#define MAXBASE 0x8

void write_max( unsigned char __xdata *regnum, unsigned char val ) {
    unsigned char oldDPP = DPP;
    DPP     = MAXBASE;
    *regnum = val;
    DPP     = oldDPP;
}

unsigned char read_max( unsigned char __xdata *regnum ) {
    unsigned char oldDPP=DPP;
    unsigned char val;
    DPP = MAXBASE;
    val = *regnum;
    DPP = oldDPP;
    return val;
}

