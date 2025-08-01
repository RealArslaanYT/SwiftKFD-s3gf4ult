#import "info.h"
#import "primitives.h"
#import "translation.h"
#import "kernel.h"
#import "util.h"
#import <Foundation/Foundation.h>
#import <IOSurface/IOSurfaceRef.h>
#import <CoreGraphics/CoreGraphics.h>

uint64_t jb_IOSurfaceRootUserClient_get_surfaceClientById(uint64_t rootUserClient, uint32_t surfaceId)
{
	uint64_t surfaceClientsArray = jb_kread_ptr(rootUserClient + 0x118);
	return jb_kread_ptr(surfaceClientsArray + (sizeof(uint64_t)*surfaceId));
}

uint64_t jb_IOSurfaceClient_get_surface(uint64_t surfaceClient)
{
	return jb_kread_ptr(surfaceClient + 0x40);
}

uint64_t jb_IOSurfaceSendRight_get_surface(uint64_t surfaceSendRight)
{
	return jb_kread_ptr(surfaceSendRight + 0x18);	
}

uint64_t jb_IOSurface_get_ranges(uint64_t surface)
{
	return jb_kread_ptr(surface + 0x3e0);
}

void jb_IOSurface_set_ranges(uint64_t surface, uint64_t ranges)
{
	jb_kwrite64(surface + 0x3e0, ranges);
}

uint64_t jb_IOSurface_get_memoryDescriptor(uint64_t surface)
{
	return jb_kread_ptr(surface + 0x38);
}

uint64_t jb_IOMemoryDescriptor_get_ranges(uint64_t memoryDescriptor)
{
	return jb_kread_ptr(memoryDescriptor + 0x60);
}

uint64_t jb_IOMemorydescriptor_get_size(uint64_t memoryDescriptor)
{
	return jb_kread64(memoryDescriptor + 0x50);
}

void jb_IOMemoryDescriptor_set_size(uint64_t memoryDescriptor, uint64_t size)
{
	jb_kwrite64(memoryDescriptor + 0x50, size);
}

void jb_IOMemoryDescriptor_set_wired(uint64_t memoryDescriptor, bool wired)
{
	jb_kwrite8(memoryDescriptor + 0x88, wired);
}

uint32_t jb_IOMemoryDescriptor_get_flags(uint64_t memoryDescriptor)
{
	return jb_kread32(memoryDescriptor + 0x20);
}

void jb_IOMemoryDescriptor_set_flags(uint64_t memoryDescriptor, uint32_t flags)
{
	jb_kwrite8(memoryDescriptor + 0x20, flags);
}

void jb_IOMemoryDescriptor_set_memRef(uint64_t memoryDescriptor, uint64_t memRef)
{
	jb_kwrite64(memoryDescriptor + 0x28, memRef);
}

uint64_t jb_IOSurface_get_rangeCount(uint64_t surface)
{
	return jb_kread_ptr(surface + 0x3e8);
}

void jb_IOSurface_set_rangeCount(uint64_t surface, uint32_t rangeCount)
{
	jb_kwrite32(surface + 0x3e8, rangeCount);
}

static mach_port_t jb_IOSurface_map_getSurfacePort(uint64_t magic)
{
	IOSurfaceRef surfaceRef = IOSurfaceCreate((__bridge CFDictionaryRef)@{
		(__bridge NSString *)kIOSurfaceWidth : @120,
		(__bridge NSString *)kIOSurfaceHeight : @120,
		(__bridge NSString *)kIOSurfaceBytesPerElement : @4,
	});
	mach_port_t port = IOSurfaceCreateMachPort(surfaceRef);
	*((uint64_t *)IOSurfaceGetBaseAddress(surfaceRef)) = magic;
	IOSurfaceDecrementUseCount(surfaceRef);
	CFRelease(surfaceRef);
	return port;
}

int jb_IOSurface_map(uint64_t pa, uint64_t size, void **uaddr)
{
	mach_port_t surfaceMachPort = jb_IOSurface_map_getSurfacePort(1337);

	uint64_t surfaceSendRight = task_get_ipc_port_kobject(task_self(), surfaceMachPort);
	uint64_t surface = jb_IOSurfaceSendRight_get_surface(surfaceSendRight);
	uint64_t desc = jb_IOSurface_get_memoryDescriptor(surface);
	uint64_t ranges = jb_IOMemoryDescriptor_get_ranges(desc);

	jb_kwrite64(ranges, pa);
	jb_kwrite64(ranges+8, size);

    jb_IOMemoryDescriptor_set_size(desc, size);

	jb_kwrite64(desc + 0x70, 0);
	jb_kwrite64(desc + 0x18, 0);
	jb_kwrite64(desc + 0x90, 0);

    jb_IOMemoryDescriptor_set_wired(desc, true);

	uint32_t flags = jb_IOMemoryDescriptor_get_flags(desc);
    jb_IOMemoryDescriptor_set_flags(desc, (flags & ~0x410) | 0x20);

    jb_IOMemoryDescriptor_set_memRef(desc, 0);

	IOSurfaceRef mappedSurfaceRef = IOSurfaceLookupFromMachPort(surfaceMachPort);
	*uaddr = IOSurfaceGetBaseAddress(mappedSurfaceRef);
	return 0;
}

static mach_port_t jb_IOSurface_kalloc_getSurfacePort(uint64_t size)
{
	uint64_t allocSize = 0x10;
	uint64_t *addressRangesBuf = (uint64_t *)malloc(size);
	memset(addressRangesBuf, 0, size);
	addressRangesBuf[0] = (uint64_t)malloc(allocSize);
	addressRangesBuf[1] = allocSize;
	NSData *addressRanges = [NSData dataWithBytes:addressRangesBuf length:size];
	free(addressRangesBuf);

	IOSurfaceRef surfaceRef = IOSurfaceCreate((__bridge CFDictionaryRef)@{
		@"IOSurfaceAllocSize" : @(allocSize),
		@"IOSurfaceAddressRanges" : addressRanges,
	});
	mach_port_t port = IOSurfaceCreateMachPort(surfaceRef);
	IOSurfaceDecrementUseCount(surfaceRef);
	return port;
}

uint64_t jb_IOSurface_kalloc(uint64_t size, bool leak)
{
	while (true) {
		uint64_t allocSize = max(size, 0x10000);
		mach_port_t surfaceMachPort = jb_IOSurface_kalloc_getSurfacePort(allocSize);

		uint64_t surfaceSendRight = task_get_ipc_port_kobject(task_self(), surfaceMachPort);
		uint64_t surface = jb_IOSurfaceSendRight_get_surface(surfaceSendRight);
		uint64_t va = jb_IOSurface_get_ranges(surface);

		if (kvtophys(va + allocSize) != 0) {
			mach_port_deallocate(mach_task_self(), surfaceMachPort);
			continue;
		}

		if (va == 0) continue;

		if (leak) {
            jb_IOSurface_set_ranges(surface, 0);
            jb_IOSurface_set_rangeCount(surface, 0);
		}

		return va + (allocSize - size);
	}

	return 0;
}

int jb_IOSurface_kalloc_global(uint64_t *addr, uint64_t size)
{
	uint64_t alloc = jb_IOSurface_kalloc(size, true);
	if (alloc != 0) {
		*addr = alloc;
		return 0;
	}
	return -1;
}

int jb_IOSurface_kalloc_local(uint64_t *addr, uint64_t size)
{
	uint64_t alloc = jb_IOSurface_kalloc(size, false);
	if (alloc != 0) {
		*addr = alloc;
		return 0;
	}
	return -1;
}

void jb_libjailbreak_IOSurface_primitives_init(void)
{
	IOSurfaceRef surfaceRef = IOSurfaceCreate((__bridge CFDictionaryRef)@{
		(__bridge NSString *)kIOSurfaceWidth : @120,
		(__bridge NSString *)kIOSurfaceHeight : @120,
		(__bridge NSString *)kIOSurfaceBytesPerElement : @4,
	});
	if (!surfaceRef) {
		printf("Failed to initialize IOSurface primitives, add \"IOSurfaceRootUserClient\" to the \"com.apple.security.exception.iokit-user-client-class\" dictionary of the binaries entitlements to fix this.\n");
		return;
	}
	CFRelease(surfaceRef);

	gPrimitives.kalloc_global = jb_IOSurface_kalloc_global;
	gPrimitives.kalloc_local  = jb_IOSurface_kalloc_local;
	gPrimitives.kmap          = jb_IOSurface_map;
}
