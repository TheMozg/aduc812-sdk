#include "aduc812.h"
#include "max.h"
#include "dip.h"

unsigned char get_dips( void ) {
  return read_max(EXT_LO);
}
