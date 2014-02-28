#include "bootloader.h"
#case
#include <GOGO40.H>

#define bip PIN_C2

void main() {

   set_tris_a(PIC_TRIS_A);
   set_tris_b(PIC_TRIS_B);
   set_tris_c(PIC_TRIS_C);
   set_tris_d(PIC_TRIS_D);
   set_tris_e(PIC_TRIS_E);

   while(1) {

      output_high(bip);
   }
}
