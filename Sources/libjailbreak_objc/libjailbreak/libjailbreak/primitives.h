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

void jb_enumerate_pages(uint64_t start, size_t size, uint64_t pageSize, bool (^block)(uint64_t, size_t));

int jb_kreadbuf(uint64_t kaddr, void* output, size_t size);
int jb_kwritebuf(uint64_t kaddr, const void* input, size_t size);
int jb_physreadbuf(uint64_t physaddr, void* output, size_t size);
int jb_physwritebuf(uint64_t physaddr, const void* input, size_t size);

uint64_t jb_physread64(uint64_t pa);
uint64_t jb_physread_ptr(uint64_t va);
uint32_t jb_physread32(uint64_t pa);
uint16_t jb_physread16(uint64_t pa);
uint8_t jb_physread8(uint64_t pa);

int jb_physwrite64(uint64_t pa, uint64_t v);
int jb_physwrite32(uint64_t pa, uint32_t v);
int jb_physwrite16(uint64_t pa, uint16_t v);
int jb_physwrite8(uint64_t pa, uint8_t v);

uint64_t jb_kread64(uint64_t va);
uint64_t jb_kread_ptr(uint64_t va);
uint64_t jb_kread_smrptr(uint64_t va);
uint32_t jb_kread32(uint64_t va);
uint16_t jb_kread16(uint64_t va);
uint8_t jb_kread8(uint64_t va);

int jb_kwrite64(uint64_t va, uint64_t v);
int jb_kwrite_ptr(uint64_t kaddr, uint64_t pointer, uint16_t salt);
int jb_kwrite32(uint64_t va, uint32_t v);
int jb_kwrite16(uint64_t va, uint16_t v);
int jb_kwrite8(uint64_t va, uint8_t v);

int jb_kcall(uint64_t *result, uint64_t func, int argc, const uint64_t *argv);
int jb_kexec(kRegisterState *state);

int jb_kmap(uint64_t pa, uint64_t size, void **uaddr);
int jb_kalloc_with_options(uint64_t *addr, uint64_t size, kalloc_options options);
int jb_kalloc(uint64_t *addr, uint64_t size);

int jb_kfree(uint64_t addr, uint64_t size);

#endif
