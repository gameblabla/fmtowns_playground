#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include "io.h"


typedef enum {
   HSW1, HSW2, RSV0, RSV1, HST,  VST1, VST2, EET,
   VST,  HDS0, HDE0, HDS1, HDE1, VDS0, VDE0, VDS1,
   VDE1, FA0,  HAJ0, FO0,  LO0,  FA1,  HAJ1, FO1,
   LO1,  EHAJ, EVAJ, ZOOM, CR0,  CR1,  FR,   CR2
} crtc_reg_t;

typedef enum {
   V_CTL, V_PRI
} video_reg_t; 

typedef enum {
   CRTC_ADDR = 0x0440,
   CRTC_DATA = 0x0442,
   CRTC_DATA_LO = 0x0442,
   CRTC_DATA_HI = 0x0443,
   VIDEO_ADDR = 0x0448,
   VIDEO_DATA = 0x044a,
   PALETTE_CODE  = 0xfd90,
   PALETTE_BLUE  = 0xfd92,
   PALETTE_RED   = 0xfd94,
   PALETTE_GREEN = 0xfd96
} port_t;

typedef uint16_t crtc_set_t[32];
typedef uint8_t video_set_t[2];

void start_display(void);
void stop_display(void);
void set_crtc(const crtc_set_t);
void set_video(const video_set_t);

uint16_t crtc_in16(crtc_reg_t reg);
void crtc_out8_lo(crtc_reg_t reg, uint16_t val);
void crtc_out8_hi(crtc_reg_t reg, uint16_t val);
void crtc_out16(crtc_reg_t reg, uint16_t val);

uint8_t video_in(video_reg_t reg);
void video_out(video_reg_t reg, uint8_t val);

#define rgb15(r, g, b) (((g & 31) << 10) | ((r & 31) << 5) | (b & 31))

#endif
