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



#undef TEST_TIMES
#define DEFTESTS 9

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
	int i, ip;
	ulong sh, sl, l, h, t;

	// FM TOWNS
	for (i=0; i<n*1000; i++) outb(0, 0x006C);	// 1us wait 20F-
}

void cput(int y, int x, char c)
{
	int i;
	unsigned short pos = y*(0x500)+x;	// 1 line = 0x50 * 16 line
	char *cvram = (char *)SCREEN_ADR+y*160+x*2;
	char *vram = (char *)0xC0000;
	char *cgram = (char *)(0xCA000+c*8);
	*cvram = c;
	outb(1, IO_KVRAM_OR_ANKFONT);	// Use ANK
	for (i=0; i<8; i++) {
		vram[pos] = vram[pos+0x50] = cgram[i];
		pos += 0xA0;
	}
	outb(0, IO_KVRAM_OR_ANKFONT);	// Use KANJI
}


#define SCREEN_SIZE 0x2000
#define FM_SCREEN_ADR  ((volatile unsigned char *)0xc8000)

void my_memset(volatile unsigned char *dest, unsigned char value, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++)
    {
        dest[i] = value;
    }
}

void do_test(void)
{
	int i;
	volatile char *pp;
	
	set_cache(1);

	my_memset(FM_SCREEN_ADR, 0xFF, 80*25*2);
	
	cput(0, 0, 'C');

	while(1)
	{
		
	}
}
