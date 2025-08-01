#ifndef PRIMITIVES_IOSURFACE_H
#define PRIMITIVES_IOSURFACE_H

void *jb_IOSurface_map(uint64_t phys, uint64_t size);
uint64_t jb_IOSurface_kalloc(uint64_t size, bool leak);
int jb_IOSurface_kalloc_global(uint64_t *addr, uint64_t size);
int jb_IOSurface_kalloc_local(uint64_t *addr, uint64_t size);
void jb_libjailbreak_IOSurface_primitives_init(void);

#endif
