# Makefile for MemTest86+
#
# Author:		Chris Brady
# Created:		January 1, 1996

#
# Path for the floppy disk device
#
FDISK=FINAL_DISK.img

AS=as -32
CC=gcc
LD=ld -s

CFLAGS= -Wall -march=i486 -std=gnu90 -m32 -O2 -fomit-frame-pointer -fno-builtin -ffreestanding -fPIC -fno-stack-protector -s

OBJS= head.o reloc.o main.o

all: mygame.bin mygame

# Link it statically once so I know I don't have undefined
# symbols and then link it dynamically so I have full
# relocation information
mygame_shared: $(OBJS) mygame_shared.lds Makefile
	$(LD) --warn-constructors --warn-common -static -T mygame_shared.lds \
	-o $@ $(OBJS) && \
	$(LD) -shared -Bsymbolic -T mygame_shared.lds -o $@ $(OBJS)

mygame_shared.bin: mygame_shared
	objcopy -O binary $< mygame_shared.bin

mygame: mygame_shared.bin mygame.lds
	$(LD) -s -T mygame.lds -b binary mygame_shared.bin -o $@

head.s: head.S config.h defs.h test.h
	$(CC) -E -traditional $< -o $@

bootsect.s: bootsect.S config.h defs.h
	$(CC) -E -traditional $< -o $@

setup.s: setup.S config.h defs.h
	$(CC) -E -traditional $< -o $@

mygame.bin: mygame_shared.bin bootsect.o setup.o mygame.bin.lds
	$(LD) -T mygame.bin.lds bootsect.o setup.o -b binary \
	mygame_shared.bin -o mygame.bin

reloc.o: reloc.c
	$(CC) -c $(CFLAGS) -fno-strict-aliasing reloc.c
clean:
	rm -f *.o *.s *.iso mygame.bin mygame mygame_shared mygame_shared.bin

install: all
	dd if=/dev/zero of=$(FDISK) bs=1024 count=1232
	dd if=mygame.bin of=$(FDISK) conv=notrunc bs=8192

