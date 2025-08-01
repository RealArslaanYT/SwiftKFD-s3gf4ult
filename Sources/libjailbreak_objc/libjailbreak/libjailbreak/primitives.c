#include "primitives.h"
#include "kernel.h"
#include "info.h"
#include "translation.h"
#include "pte.h"
#include "util.h"
#include <errno.h>
#include <string.h>

struct kernel_primitives gPrimitives = { 0 };

// Wrappers physical <-> virtual

void jb_enumerate_pages(uint64_t start, size_t size, uint64_t pageSize, bool (^block)(uint64_t curStart, size_t curSize))
{
    uint64_t curStart = start;
    size_t sizeLeft = size;
    bool c = true;
    while (sizeLeft > 0 && c) {
        uint64_t pageOffset = curStart & (pageSize - 1);
        uint64_t readSize = min(sizeLeft, pageSize - pageOffset);
        c = block(curStart, readSize);
        curStart += readSize;
        sizeLeft -= readSize;
    }
}

int jb_kreadbuf_phys(uint64_t kaddr, void* output, size_t size)
{
    memset(output, 0, size);

    __block int pr = 0;
    jb_enumerate_pages(kaddr, size, P_PAGE_SIZE, ^bool(uint64_t curKaddr, size_t curSize){
        uint64_t curPhys = kvtophys(curKaddr);
        if (curPhys == 0 && errno != 0) {
            pr = errno;
            return false;
        }
        pr = jb_physreadbuf(curPhys, &output[curKaddr - kaddr], curSize);
        if (pr != 0) {
            return false;
        }
        return true;
    });
    return pr;
}

int jb_kwritebuf_phys(uint64_t kaddr, const void* input, size_t size)
{
    __block int pr = 0;
    jb_enumerate_pages(kaddr, size, P_PAGE_SIZE, ^bool(uint64_t curKaddr, size_t curSize){
        uint64_t curPhys = kvtophys(curKaddr);
        if (curPhys == 0 && errno != 0) {
            pr = errno;
            return false;
        }
        pr = jb_physwritebuf(curPhys, &input[curKaddr - kaddr], curSize);
        if (pr != 0) {
            return false;
        }
        return true;
    });
    return pr;
}

int jb_physreadbuf_virt(uint64_t physaddr, void* output, size_t size)
{
    memset(output, 0, size);

    __block int pr = 0;
    jb_enumerate_pages(physaddr, size, P_PAGE_SIZE, ^bool(uint64_t curPhys, size_t curSize){
        uint64_t curKaddr = jb_phystokv(curPhys);
        if (curKaddr == 0 && errno != 0) {
            pr = errno;
            return false;
        }
        pr = jb_kreadbuf(curKaddr, &output[curPhys - physaddr], curSize);
        if (pr != 0) {
            return false;
        }
        return true;
    });
    return pr;
}

int jb_physwritebuf_virt(uint64_t physaddr, const void* input, size_t size)
{
    __block int pr = 0;
    jb_enumerate_pages(physaddr, size, P_PAGE_SIZE, ^bool(uint64_t curPhys, size_t curSize){
        uint64_t curKaddr = jb_phystokv(curPhys);
        if (curKaddr == 0 && errno != 0) {
            pr = errno;
            return false;
        }
        pr = jb_kwritebuf(curKaddr, &input[curPhys - physaddr], curSize);
        if (pr != 0) {
            return false;
        }
        return true;
    });
    return pr;
}

// Wrappers to gPrimitives

int jb_kreadbuf(uint64_t kaddr, void* output, size_t size)
{
    if (gPrimitives.kreadbuf) {
        return gPrimitives.kreadbuf(kaddr, output, size);
    }
    else if (gPrimitives.physreadbuf && gPrimitives.vtophys) {
        return jb_kreadbuf_phys(kaddr, output, size);
    }
    return -1;
}

int jb_kwritebuf(uint64_t kaddr, const void* input, size_t size)
{
    if (gPrimitives.kwritebuf) {
        return gPrimitives.kwritebuf(kaddr, input, size);
    }
    else if (gPrimitives.physwritebuf && gPrimitives.vtophys) {
        return jb_kwritebuf_phys(kaddr, input, size);
    }
    return -1;
}

int jb_physreadbuf(uint64_t physaddr, void* output, size_t size)
{
    if (gPrimitives.physreadbuf) {
        return gPrimitives.physreadbuf(physaddr, output, size);
    }
    else if (gPrimitives.kreadbuf && gPrimitives.phystokv) {
        return jb_physreadbuf_virt(physaddr, output, size);
    }
    return -1;
}

int jb_physwritebuf(uint64_t physaddr, const void* input, size_t size)
{
    if (gPrimitives.physwritebuf) {
        return gPrimitives.physwritebuf(physaddr, input, size);
    }
    else if (gPrimitives.kwritebuf && gPrimitives.phystokv) {
        return jb_physwritebuf_virt(physaddr, input, size);
    }
    return -1;
}

// Convenience Wrappers

uint64_t jb_physread64(uint64_t pa)
{
    uint64_t v;
    jb_physreadbuf(pa, &v, sizeof(v));
    return v;
}

uint64_t jb_physread_ptr(uint64_t pa)
{
    return UNSIGN_PTR(jb_physread64(pa));
}

uint32_t jb_physread32(uint64_t pa)
{
    uint32_t v;
    jb_physreadbuf(pa, &v, sizeof(v));
    return v;
}

uint16_t jb_physread16(uint64_t pa)
{
    uint16_t v;
    jb_physreadbuf(pa, &v, sizeof(v));
    return v;
}

uint8_t jb_physread8(uint64_t pa)
{
    uint8_t v;
    jb_physreadbuf(pa, &v, sizeof(v));
    return v;
}

int jb_physwrite64(uint64_t pa, uint64_t v)
{
    return jb_physwritebuf(pa, &v, sizeof(v));
}

int jb_physwrite32(uint64_t pa, uint32_t v)
{
    return jb_physwritebuf(pa, &v, sizeof(v));
}

int jb_physwrite16(uint64_t pa, uint16_t v)
{
    return jb_physwritebuf(pa, &v, sizeof(v));
}

int jb_physwrite8(uint64_t pa, uint8_t v)
{
    return jb_physwritebuf(pa, &v, sizeof(v));
}

uint64_t jb_kread64(uint64_t va)
{
    uint64_t v;
    jb_kreadbuf(va, &v, sizeof(v));
    return v;
}

uint64_t jb_kread_ptr(uint64_t va)
{
    return UNSIGN_PTR(jb_kread64(va));
}

uint64_t jb_kread_smrptr(uint64_t va)
{
    uint64_t value = jb_kread_ptr(va);

    uint64_t bits = (kconstant(smrBase) << (62-kconstant(T1SZ_BOOT)));

    uint64_t case1 = 0xFFFFFFFFFFFFC000 & ~bits;
    uint64_t case2 = 0xFFFFFFFFFFFFFFE0 & ~bits;

    if ((value & bits) == 0) {
        if (value) {
            value = (value & case1) | bits;
        }
    }
    else {
        value = (value & case2) | bits;
    }

    return value;
}

uint32_t jb_kread32(uint64_t va)
{
    uint32_t v;
    jb_kreadbuf(va, &v, sizeof(v));
    return v;
}

uint16_t jb_kread16(uint64_t va)
{
    uint16_t v;
    jb_kreadbuf(va, &v, sizeof(v));
    return v;
}

uint8_t jb_kread8(uint64_t va)
{
    uint8_t v;
    jb_kreadbuf(va, &v, sizeof(v));
    return v;
}

int jb_kwrite64(uint64_t va, uint64_t v)
{
    return jb_kwritebuf(va, &v, sizeof(v));
}

int jb_kwrite_ptr(uint64_t kaddr, uint64_t pointer, uint16_t salt)
{
#ifdef __arm64e__
    if (!gPrimitives.kexec || !kgadget(pacda)) return -1;
    jb_kwrite64(kaddr, kptr_sign(kaddr, pointer, salt));
#else
    jb_kwrite64(kaddr, pointer);
#endif
    return 0;
}

int jb_kwrite32(uint64_t va, uint32_t v)
{
    return jb_kwritebuf(va, &v, sizeof(v));
}

int jb_kwrite16(uint64_t va, uint16_t v)
{
    return jb_kwritebuf(va, &v, sizeof(v));
}

int jb_kwrite8(uint64_t va, uint8_t v)
{
    return jb_kwritebuf(va, &v, sizeof(v));
}

int jb_kcall(uint64_t *result, uint64_t func, int argc, const uint64_t *argv)
{
    if (gPrimitives.kcall) {
        uint64_t resultTmp = gPrimitives.kcall(func, argc, argv);
        if(result) *result = resultTmp;
        return 0;
    }
    return -1;
}

int jb_kexec(kRegisterState *state)
{
    if (gPrimitives.kexec) {
        gPrimitives.kexec(state);
    }
    return -1;
}

int jb_kmap(uint64_t pa, uint64_t size, void **uaddr)
{
    if (gPrimitives.kmap) {
        return gPrimitives.kmap(pa, size, uaddr);
    }
    return -1;
}

int jb_kalloc_with_options(uint64_t *addr, uint64_t size, kalloc_options options)
{
    if (options == KALLOC_OPTION_GLOBAL && gPrimitives.kalloc_global) {
        return gPrimitives.kalloc_global(addr, size);
    }
    else if (options == KALLOC_OPTION_LOCAL && gPrimitives.kalloc_local) {
        return gPrimitives.kalloc_local(addr, size);
    }
    return -1;
}

int jb_kalloc(uint64_t *addr, uint64_t size)
{
    return jb_kalloc_with_options(addr, size, KALLOC_OPTION_GLOBAL);
}

int jb_kfree(uint64_t addr, uint64_t size)
{
    if (gPrimitives.kfree_global) {
        return gPrimitives.kfree_global(addr, size);
    }
    return -1;
}
