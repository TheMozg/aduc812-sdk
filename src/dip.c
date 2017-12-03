#include "aduc812.h"
#include "max.h"
#include "dip.h"

unsigned char read_dip() {
    return read_max( EXT_LO );
}
