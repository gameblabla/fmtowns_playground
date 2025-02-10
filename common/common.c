#include "common.h"
#include <stdlib.h>

uint16_t crtc_in16(crtc_reg_t reg) {
   outb(reg,CRTC_ADDR);
   return inw(CRTC_DATA);
}

void crtc_out8_lo(crtc_reg_t reg, uint16_t val) {
   outb( reg, CRTC_ADDR);
   outb( val & 0xff, CRTC_DATA_LO);
}

void crtc_out8_hi(crtc_reg_t reg, uint16_t val) {
   outb(reg, CRTC_ADDR);
   outb(val >> 8, CRTC_DATA_HI);
}

void crtc_out16(crtc_reg_t reg, uint16_t val) {
   outb( reg, CRTC_ADDR);
   outw( val, CRTC_DATA);
}

uint8_t video_in(video_reg_t reg) {
   outb( reg, VIDEO_ADDR);
   return inb(VIDEO_DATA);
}

void video_out(video_reg_t reg, uint8_t val) {
   outb( reg, VIDEO_ADDR);
   outb( val, VIDEO_DATA);
}

void start_display(void) {
   uint16_t v = crtc_in16(CR0);
   v |= 0x8000;
   crtc_out16(CR0, v);
}

void stop_display(void) {
   uint16_t v = crtc_in16(CR0);
   v &= 0x7fff;
   crtc_out16(CR0, v);
}

void set_crtc(const crtc_set_t set) {
   for (int i = 0; i < sizeof(crtc_set_t) / sizeof(set[0]); ++i) {
      if (i == RSV0 || i == RSV1) {
         continue;
      }
      crtc_out16(i, set[i]);
   }
}

void set_video(const video_set_t set) {
   for (int i = 0; i < sizeof(video_set_t) / sizeof(set[0]); ++i) {
      video_out(i, set[i]);
   }
}
