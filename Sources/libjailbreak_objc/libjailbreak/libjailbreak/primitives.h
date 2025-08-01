#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "primitives_external.h"

#define BIT(b)    (1ULL << (b))
#define ONES(x)          (BIT((x))-1)
#define PAC_MASK kconstant(pointer_mask)
#define SIGN(p)          ((p) & BIT(55))
#define UNSIGN_PTR(p)    (SIGN(p) ? ((p) | PAC_MASK) : ((p) & ~PAC_MASK))

typedef enum
{
	KALLOC_OPTION_GLOBAL, // Global Allocation, never manually freed
	KALLOC_OPTION_LOCAL, // Allocation attached to this process, freed on process exit
} kalloc_options;

static void enumerate_pages(uint64_t start, size_t size, uint64_t pageSize, bool (^block)(uint64_t, size_t));

static int kreadbuf(uint64_t kaddr, void* output, size_t size);
static int kwritebuf(uint64_t kaddr, const void* input, size_t size);
static int physreadbuf(uint64_t physaddr, void* output, size_t size);
static int physwritebuf(uint64_t physaddr, const void* input, size_t size);

static uint64_t physread64(uint64_t pa);
static uint64_t physread_ptr(uint64_t va);
static uint32_t physread32(uint64_t pa);
static uint16_t physread16(uint64_t pa);
static uint8_t physread8(uint64_t pa);

static int physwrite64(uint64_t pa, uint64_t v);
static int physwrite32(uint64_t pa, uint32_t v);
static int physwrite16(uint64_t pa, uint16_t v);
static int physwrite8(uint64_t pa, uint8_t v);

static uint64_t kread64(uint64_t va);
static uint64_t kread_ptr(uint64_t va);
static uint64_t kread_smrptr(uint64_t va);
static uint32_t kread32(uint64_t va);
static uint16_t kread16(uint64_t va);
static uint8_t kread8(uint64_t va);

static int kwrite64(uint64_t va, uint64_t v);
static int kwrite_ptr(uint64_t kaddr, uint64_t pointer, uint16_t salt);
static int kwrite32(uint64_t va, uint32_t v);
static int kwrite16(uint64_t va, uint16_t v);
static int kwrite8(uint64_t va, uint8_t v);

static int kcall(uint64_t *result, uint64_t func, int argc, const uint64_t *argv);
static int kexec(kRegisterState *state);

static int kmap(uint64_t pa, uint64_t size, void **uaddr);
static int kalloc_with_options(uint64_t *addr, uint64_t size, kalloc_options options);
static int kalloc(uint64_t *addr, uint64_t size);

static int kfree(uint64_t addr, uint64_t size);

#endif
