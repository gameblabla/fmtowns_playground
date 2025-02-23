FDISK           = FINAL_DISK.img
AS              = as -32
CC              = gcc
LD              = ld -s

CFLAGS          = -Icommon -I. -Wall -march=i486 -std=gnu99 -m32 -Os \
                  -fomit-frame-pointer -fno-builtin -ffreestanding \
                  -fPIC -Wno-stack-protector -fno-stack-protector -s

OBJS            = head.o reloc.o main.o common/common.o common/palette.o 

# The default target
all: mygame.bin mygame_shared.bin

# Build shared binary normally (first static then shared for relocation info)
mygame_shared: $(OBJS) mygame_shared.lds Makefile
	$(LD) --warn-constructors --warn-common -static -T mygame_shared.lds \
		-o $@ $(OBJS) && \
	$(LD) -shared -Bsymbolic -T mygame_shared.lds -o $@ $(OBJS)

# Create binary image from the shared binary
mygame_shared.bin: mygame_shared
	objcopy -O binary $< mygame_shared.bin

# Generate load_limit.h from the size of mygame_shared.bin.
# We calculate the number of 64KB chunks needed (rounding up).
# If the computed value exceeds 7, force it to 7.
load_limit.h: mygame_shared.bin
	@size=$$(stat -c%s mygame_shared.bin); \
	chunks=$$(( (size + 65535) / 65536 )); \
	if [ $$chunks -gt 7 ]; then chunks=7; fi; \
	echo "/* Auto-generated load limit based on mygame_shared.bin size */" > load_limit.h; \
	echo "#define MAX_CHUNKS $$chunks" >> load_limit.h; \
	echo "Generated load_limit.h with MAX_CHUNKS = $$chunks"

# Preprocess bootsect.S into bootsect.s.
# Make sure bootsect.S includes load_limit.h so that MAX_CHUNKS is used.
bootsect.s: bootsect.S config.h defs.h load_limit.h
	$(CC) -E -traditional $< -o bootsect.s

# Similarly, preprocess setup.S (if needed)
setup.s: setup.S config.h defs.h
	$(CC) -E -traditional $< -o setup.s

# Build final bootable binary.
mygame.bin: mygame_shared.bin bootsect.o setup.o mygame.bin.lds
	$(LD) -T mygame.bin.lds bootsect.o setup.o -b binary mygame_shared.bin -o $@

head.s: head.S config.h defs.h
	$(CC) -E -traditional $< -o head.s

# Compile C source files
reloc.o: reloc.c
	$(CC) -c $(CFLAGS) -fno-strict-aliasing reloc.c

clean:
	rm -f *.o *.s *.iso mygame.bin mygame mygame_shared mygame_shared.bin load_limit.h

install: all
	dd if=/dev/zero of=$(FDISK) bs=1024 count=1232
	dd if=mygame.bin of=$(FDISK) conv=notrunc bs=8192
