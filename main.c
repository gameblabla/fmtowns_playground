/* main.c - MemTest-86  Version 3.2
 *
 * Released under version 2 of the Gnu Public License.
 * By Chris Brady
 * ----------------------------------------------------
 * MemTest86+ V4.00 Specific code (GPL V2.0)
 * By Samuel DEMEULEMEESTER, sdemeule@memtest.org
 * http://www.canardpc.com - http://www.memtest.org
 */
#include <string.h>
#include <stddef.h>
#include "test.h"
#include "defs.h"
#include "config.h"
#include "io.h"
#include "settings.h"
#include "common.h"


struct cpu_ident cpu_id;

void inter(struct eregs *trap_regs)
{
}

void set_cache(int val)
{
	extern struct cpu_ident cpu_id;
	/* 386's don't have a cache */
	if ((cpu_id.cpuid < 1) && (cpu_id.type == 3)) {
		// No cache
		return;
	}
	switch(val) {
	case 0:
		cache_off();
		break;
	case 1:
		cache_on();
		break;
	}
}

void sleep(int n, int sms)
{
	int i;
	// FM TOWNS
	for (i=0; i<n*1000; i++) outb(0, 0x006C);	// 1us wait 20F-
}


static crtc_set_t crtc = CRTC_SET_31;
static video_set_t video = VIDEO_SET_31;

void start_main(void)
{
	volatile char *pp;
	
	set_cache(1);
	
	volatile uint16_t *vram = (uint16_t *)0x80100000;
	

	stop_display();
    set_crtc(crtc);
    set_video(video);
	
	start_display();
	
	//const uint32_t *src_line = picture;
	for (int i = 0; i < (512*480); i++) 
	{
		vram[i] = 0xAAAA;
	}


	while(1)
	{
		
	}
}
