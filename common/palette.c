#include "palette.h"
#include "common.h"

void set_palette(uint8_t c, uint8_t r, uint8_t g, uint8_t b) {
   outb( c, PALETTE_CODE);
   outb( b, PALETTE_BLUE);
   outb( r, PALETTE_RED);
   outb( g, PALETTE_GREEN);
}
