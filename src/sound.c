#include "sound.h"
#include "notes.h"

void set_note( int hz ) {
  g_note_h = note_h( hz ); 
  g_note_l = note_l( hz );
}
