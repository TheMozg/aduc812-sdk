#include "isr_utils.h"

void set_vector( unsigned char __xdata * Address, void * Vector ) {
  unsigned char __xdata * TmpVector;

  *Address = 0x02;

  TmpVector = (unsigned char __xdata *) (Address + 1);
  *TmpVector = (unsigned char) ((unsigned short)Vector >> 8);
  ++TmpVector;
  *TmpVector = (unsigned char) Vector;

}
