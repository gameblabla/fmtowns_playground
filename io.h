#ifndef _ASM_IO_H
#define _ASM_IO_H

/*
 * This file contains the definitions for the x86 IO instructions
 * inb/inw/inl/outb/outw/outl and the "string versions" of the same
 * (insb/insw/insl/outsb/outsw/outsl). You can also use "pausing"
 * versions of the single-IO instructions (inb_p/inw_p/..).
 *
 * This file is not meant to be obfuscating: it's just complicated
 * to (a) handle it all in a way that makes gcc able to optimize it
 * as well as possible and (b) trying to avoid writing the same thing
 * over and over again with slight variations and possibly making a
 * mistake somewhere.
 */

#ifdef SLOW_IO_BY_JUMPING
#define __SLOW_DOWN_IO __asm__ __volatile__("jmp 1f\n1:\tjmp 1f\n1:")
#else
#define __SLOW_DOWN_IO __asm__ __volatile__("outb %al,$0x80")
#endif

#ifdef REALLY_SLOW_IO
#define SLOW_DOWN_IO { __SLOW_DOWN_IO; __SLOW_DOWN_IO; __SLOW_DOWN_IO; __SLOW_DOWN_IO; }
#else
#define SLOW_DOWN_IO __SLOW_DOWN_IO
#endif

/*
 * Talk about misusing macros..
 */
#include <stdint.h>

/* Byte Input */
static inline uint8_t __inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline uint8_t __inbc(uint16_t port) {
    return __inb(port);
}

/* Byte Output */
static inline void __outb(uint8_t val, uint16_t port) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void __outbc(uint8_t val, uint16_t port) {
    __outb(val, port);
}

/* Word Input (16-bit) */
static inline uint16_t __inw(uint16_t port) {
    uint16_t val;
    __asm__ volatile ("inw %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline uint16_t __inwc(uint16_t port) {
    return __inw(port);
}

/* Word Output (16-bit) */
static inline void __outw(uint16_t val, uint16_t port) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline void __outwc(uint16_t val, uint16_t port) {
    __outw(val, port);
}

/* Long Input (32-bit) */
static inline uint32_t __inl(uint16_t port) {
    uint32_t val;
    __asm__ volatile ("inl %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline uint32_t __inlc(uint16_t port) {
    return __inl(port);
}

/* Long Output (32-bit) */
static inline void __outl(uint32_t val, uint16_t port) {
    __asm__ volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}

static inline void __outlc(uint32_t val, uint16_t port) {
    __outl(val, port);
}

/* Macros for Port I/O */
#define inb(port) \
    ((__builtin_constant_p((port)) && (port) < 256) ? __inbc(port) : __inb(port))

#define outb(val, port) \
    ((__builtin_constant_p((port)) && (port) < 256) ? __outbc((val), (port)) : __outb((val), (port)))

#define inw(port) \
    ((__builtin_constant_p((port)) && (port) < 256) ? __inwc(port) : __inw(port))

#define outw(val, port) \
    ((__builtin_constant_p((port)) && (port) < 256) ? __outwc((val), (port)) : __outw((val), (port)))

#define inl(port) \
    ((__builtin_constant_p((port)) && (port) < 256) ? __inlc(port) : __inl(port))

#define outl(val, port) \
    ((__builtin_constant_p((port)) && (port) < 256) ? __outlc((val), (port)) : __outl((val), (port)))


#endif
