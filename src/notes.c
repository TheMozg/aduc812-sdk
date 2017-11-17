#include "notes.h"
#include "sys_timer.h"

short note( int hz ) {
  // hz is multiplied by 2 cause we handle a note in two half periods
  return 0xFFFF - MCLKIN / ( ( hz * 2 )  ) / 12;
}

char note_h( int hz ) {
  return ( note( hz ) >> 8 ) & 0xFF;
}

char note_l( int hz ) {
  return ( note( hz ) & 0xFF );
}

