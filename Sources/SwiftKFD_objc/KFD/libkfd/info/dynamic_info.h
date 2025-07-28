// // /*
// //  * Copyright (c) 2023 Félix Poulin-Bélanger. All rights reserved.
// //  */

// // #ifndef dynamic_info_h
// // #define dynamic_info_h

#include "../../fun/kpf/patchfinder.h"

// // struct dynamic_info {
// //     const char* kern_version;
// //     bool kread_kqueue_workloop_ctl_supported;
// //     bool perf_supported;
// //     // struct proc
// //     u64 proc__p_list__le_prev;
// //     u64 proc__p_pid;
// //     u64 proc__p_fd__fd_ofiles;
// //     u64 proc__object_size;
// //     // struct task
// //     u64 task__map;
// //     // struct thread
// //     u64 thread__thread_id;
// //     // kernelcache static addresses (perf)
// //     u64 kernelcache__cdevsw;                          // "spec_open type" or "Can't mark ptc as kqueue ok"
// //     u64 kernelcache__gPhysBase;                       // "%s: illegal PA: 0x%llx; phys base 0x%llx, size 0x%llx"
// //     u64 kernelcache__gPhysSize;                       // (gPhysBase + 0x8)
// //     u64 kernelcache__gVirtBase;                       // "%s: illegal PA: 0x%llx; phys base 0x%llx, size 0x%llx"
// //     u64 kernelcache__perfmon_dev_open;                // "perfmon: attempt to open unsupported source: 0x%x"
// //     u64 kernelcache__perfmon_devices;                 // "perfmon: %s: devfs_make_node_clone failed"
// //     u64 kernelcache__ptov_table;                      // "%s: illegal PA: 0x%llx; phys base 0x%llx, size 0x%llx"
// //     u64 kernelcache__vn_kqfilter;                     // "Invalid knote filter on a vnode!"
// // };

// // struct dynamic_info kern_versions[] = {
// //     // iOS 16.x / arm64e
// //     {
// //         .kern_version = "Darwin Kernel Version 22.x.x / arm64e",
// //         .kread_kqueue_workloop_ctl_supported = false,
// //         .perf_supported = true,
// //         .proc__p_list__le_prev = 0x0008,
// //         .proc__p_pid = 0x0060,
// //         .proc__p_fd__fd_ofiles = 0x00f8,
// //         .proc__object_size = 0,
// //         .task__map = 0x0028,
// //         .thread__thread_id = 0,
// //         .kernelcache__cdevsw = 0,
// //         .kernelcache__gPhysBase = 0,
// //         .kernelcache__gPhysSize = 0,
// //         .kernelcache__gVirtBase = 0,
// //         .kernelcache__perfmon_dev_open = 0,
// //         .kernelcache__perfmon_devices = 0,
// //         .kernelcache__ptov_table = 0,
// //         .kernelcache__vn_kqfilter = 0,
// //     },
// // //    // iOS 16.6 - iPhone 12 Pro
// // //    // T1SZ_BOOT must be changed to 25 instead of 17
// // //    {
// // //        .kern_version = "Darwin Kernel Version 22.6.0: Wed Jun 28 20:50:15 PDT 2023; root:xnu-8796.142.1~1/RELEASE_ARM64_T8101",
// // //        .kread_kqueue_workloop_ctl_supported = false,
// // //        .perf_supported = true,
// // //        .proc__p_list__le_prev = 0x0008,
// // //        .proc__p_pid = 0x0060,
// // //        .proc__p_fd__fd_ofiles = 0x00f8,
// // //        .proc__object_size = 0x0730,
// // //        .task__map = 0x0028,
// // //        .thread__thread_id = 0,
// // //        .kernelcache__cdevsw = 0xfffffff00a4a5288,
// // //        .kernelcache__gPhysBase = 0xfffffff0079303b8,
// // //        .kernelcache__gPhysSize = 0xfffffff0079303c0,
// // //        .kernelcache__gVirtBase = 0xfffffff00792e570,
// // //        .kernelcache__perfmon_dev_open = 0xfffffff007ef4278,
// // //        .kernelcache__perfmon_devices = 0xfffffff00a4e5320,
// // //        .kernelcache__ptov_table = 0xfffffff0078e38f0,
// // //        .kernelcache__vn_kqfilter = 0xfffffff007f42f40,
// // //    },
// // //    // macOS 13.4 - MacBook Air (M2, 2022)
// // //    {
// // //        .kern_version = "todo",
// // //        .kread_kqueue_workloop_ctl_supported = false,
// // //        .perf_supported = false,
// // //        .proc__p_list__le_prev = 0x0008,
// // //        .proc__p_pid = 0x0060,
// // //        .proc__p_fd__fd_ofiles = 0x00f8,
// // //        .proc__object_size = 0x0778,
// // //        .task__map = 0x0028,
// // //        .thread__thread_id = 0,
// // //        .kernelcache__cdevsw = 0,
// // //        .kernelcache__gPhysBase = 0,
// // //        .kernelcache__gPhysSize = 0,
// // //        .kernelcache__gVirtBase = 0,
// // //        .kernelcache__perfmon_dev_open = 0,
// // //        .kernelcache__perfmon_devices = 0,
// // //        .kernelcache__ptov_table = 0,
// // //        .kernelcache__vn_kqfilter = 0,
// // //    },
// // //    // macOS 13.5 - MacBook Air (M2, 2022)
// // //    {
// // //        .kern_version = "Darwin Kernel Version 22.6.0: Wed Jul  5 22:17:35 PDT 2023; root:xnu-8796.141.3~6/RELEASE_ARM64_T8112",
// // //        .kread_kqueue_workloop_ctl_supported = false,
// // //        .perf_supported = false,
// // //        .proc__p_list__le_prev = 0x0008,
// // //        .proc__p_pid = 0x0060,
// // //        .proc__p_fd__fd_ofiles = 0x00f8,
// // //        .proc__object_size = 0x0778,
// // //        .task__map = 0x0028,
// // //        .thread__thread_id = 0,
// // //        .kernelcache__cdevsw = 0,
// // //        .kernelcache__gPhysBase = 0,
// // //        .kernelcache__gPhysSize = 0,
// // //        .kernelcache__gVirtBase = 0,
// // //        .kernelcache__perfmon_dev_open = 0,
// // //        .kernelcache__perfmon_devices = 0,
// // //        .kernelcache__ptov_table = 0,
// // //        .kernelcache__vn_kqfilter = 0,
// // //    },
// // //    // iOS 16.1.2 - iPhone 14 Pro
// // //    // T1SZ_BOOT = 25
// // //    {
// // //        .kern_version = "Darwin Kernel Version 22.1.0: Thu Oct  6 19:34:16 PDT 2022; root:xnu-8792.42.7~1/RELEASE_ARM64_T8120",
// // //        .kread_kqueue_workloop_ctl_supported = false,
// // //        .perf_supported = true,
// // //        .proc__p_list__le_prev = 0x0008,
// // //        .proc__p_pid = 0x0060,
// // //        .proc__p_fd__fd_ofiles = 0x00f8,
// // //        .proc__object_size = 0x530,
// // //        .task__map = 0x0028,
// // //        .thread__thread_id = 0,
// // //        .kernelcache__cdevsw = 0xFFFFFFF00A311168,
// // //        .kernelcache__gPhysBase = 0xFFFFFFF00784BD50,
// // //        .kernelcache__gPhysSize = 0xFFFFFFF00784BD58,
// // //        .kernelcache__gVirtBase = 0xFFFFFFF007849F38,
// // //        .kernelcache__perfmon_dev_open = 0xFFFFFFF007EC0288,
// // //        .kernelcache__perfmon_devices = 0xFFFFFFF00A34C310,
// // //        .kernelcache__ptov_table = 0xFFFFFFF0077FFA18,
// // //        .kernelcache__vn_kqfilter = 0xFFFFFFF007F101A0,
// // //    },
// // };

// // #endif /* dynamic_info_h */


// /*
//  * Edited by c22dev and Arslaan Pathan
//  * Credit to alfiecg_dev for the offsets (taken from TrollInstallerX)
//  * Copyright (c) 2023 Félix Poulin-Bélanger. All rights reserved.
//  */

// #ifndef dynamic_info_h
// #define dynamic_info_h

// struct dynamic_info {
//     const char* kern_version;
//     bool kread_kqueue_workloop_ctl_supported;
//     bool perf_supported; 
//     u64 fileglob__fg_ops;
//     u64 fileglob__fg_data;
//     u64 fileops__fo_kqfilter;
//     u64 fileproc_guard__fpg_guard;
//     u64 kqworkloop__kqwl_state;
//     u64 kqworkloop__kqwl_p;
//     u64 kqworkloop__kqwl_owner;
//     u64 kqworkloop__kqwl_dynamicid;
//     u64 kqworkloop__object_size;
//     u64 pmap__tte;
//     u64 pmap__ttep;
//     u64 proc__p_list__le_next;
//     u64 proc__p_list__le_prev;
//     u64 proc__p_pid;
//     u64 proc__p_fd__fd_ofiles;
//     u64 proc__object_size;
//     u64 pseminfo__psem_usecount;
//     u64 pseminfo__psem_uid;
//     u64 pseminfo__psem_gid;
//     u64 pseminfo__psem_name;
//     u64 pseminfo__psem_semobject;
//     u64 semaphore__owner;
//     u64 specinfo__si_rdev;
//     u64 task__map;
//     u64 task__threads__next;
//     u64 task__threads__prev;
//     u64 task__itk_space;
//     u64 task__object_size;
//     u64 thread__task_threads__next;
//     u64 thread__task_threads__prev;
//     u64 thread__map;
//     u64 thread__thread_id;
//     u64 thread__object_size;
//     u64 uthread__object_size;
//     u64 vm_map_entry__links__prev;
//     u64 vm_map_entry__links__next;
//     u64 vm_map_entry__links__start;
//     u64 vm_map_entry__links__end;
//     u64 vm_map_entry__store__entry__rbe_left;
//     u64 vm_map_entry__store__entry__rbe_right;
//     u64 vm_map_entry__store__entry__rbe_parent;
//     u64 vnode__v_un__vu_specinfo;
//     u64 _vm_map__hdr__links__prev;
//     u64 _vm_map__hdr__links__next;
//     u64 _vm_map__hdr__links__start;
//     u64 _vm_map__hdr__links__end;
//     u64 _vm_map__hdr__nentries;
//     u64 _vm_map__hdr__rb_head_store__rbh_root;
//     u64 _vm_map__pmap;
//     u64 _vm_map__hint;
//     u64 _vm_map__hole_hint;
//     u64 _vm_map__holes_list;
//     u64 _vm_map__object_size;
//     u64 kernelcache__kernel_base;
//     u64 kernelcache__cdevsw;
//     u64 kernelcache__gPhysBase;
//     u64 kernelcache__gPhysSize;
//     u64 kernelcache__gVirtBase;
//     u64 kernelcache__perfmon_devices;
//     u64 kernelcache__perfmon_dev_open;
//     u64 kernelcache__ptov_table;
//     u64 kernelcache__vm_first_phys_ppnum;
//     u64 kernelcache__vm_pages;
//     u64 kernelcache__vm_page_array_beginning_addr;
//     u64 kernelcache__vm_page_array_ending_addr;
//     u64 kernelcache__vn_kqfilter;
// };

// struct dynamic_info kern_versions[] = {
//     {
//         .kern_version = "Darwin Kernel Version 21.6.0: Sun Oct 15 00:17:00 PDT 2023; root:xnu-8020.241.42~8/RELEASE_ARM64_S8000",
//         .fileglob__fg_ops = 0x10,
//         .fileglob__fg_data = 0x38,
//         .fileops__fo_kqfilter = 0x30,
//         .fileproc_guard__fpg_guard = 0x0,
//         .kqworkloop__kqwl_state = 0x10,
//         .kqworkloop__kqwl_p = 0x18,
//         .kqworkloop__kqwl_owner = 0x40,
//         .kqworkloop__kqwl_dynamicid = 0x58,
//         .kqworkloop__object_size = 0x2B0,
//         .pmap__tte = 0x0,
//         .pmap__ttep = 0x8,
//         .proc__p_list__le_next = 0x0,
//         .proc__p_list__le_prev = 0x8,
//         .proc__p_pid = 0x68,
//         .proc__p_fd__fd_ofiles = 0xF8,
//         .proc__object_size = 0x4B0,
//         .pseminfo__psem_usecount = 0x10,
//         .pseminfo__psem_uid = 0x14,
//         .pseminfo__psem_gid = 0x18,
//         .pseminfo__psem_name = 0x20,
//         .pseminfo__psem_semobject = 0x30,
//         .semaphore__owner = 0x10,
//         .specinfo__si_rdev = 0x10,
//         .task__map = 0x28,
//         .task__threads__next = 0xD8,
//         .task__threads__prev = 0xE0,
//         .task__itk_space = 0x300,
//         .task__object_size = 0x640,
//         .thread__task_threads__next = 0x38,
//         .thread__task_threads__prev = 0x40,
//         .thread__map = 0x50,
//         .thread__thread_id = 0x440,
//         .thread__object_size = 0x620,
//         .uthread__object_size = 0x4D8,
//         .vm_map_entry__links__prev = 0x0,
//         .vm_map_entry__links__next = 0x8,
//         .vm_map_entry__links__start = 0x10,
//         .vm_map_entry__links__end = 0x18,
//         .vm_map_entry__store__entry__rbe_left = 0x20,
//         .vm_map_entry__store__entry__rbe_right = 0x28,
//         .vm_map_entry__store__entry__rbe_parent = 0x30,
//         .vnode__v_un__vu_specinfo = 0x78,
//         ._vm_map__hdr__links__prev = 0x10,
//         ._vm_map__hdr__links__next = 0x18,
//         ._vm_map__hdr__links__start = 0x20,
//         ._vm_map__hdr__links__end = 0x28,
//         ._vm_map__hdr__nentries = 0x30,
//         ._vm_map__hdr__rb_head_store__rbh_root = 0x38,
//         ._vm_map__pmap = 0x40,
//         ._vm_map__hint = 0x80,
//         ._vm_map__hole_hint = 0x80,
//         ._vm_map__holes_list = 0x88,
//         ._vm_map__object_size = 0xA0,
//         .kernelcache__kernel_base = 0xfffffff007004000,
//         .kernelcache__cdevsw = 0xFFFFFFF0079A8000,
//         .kernelcache__gPhysBase = 0xFFFFFFF007A68000,
//         .kernelcache__gPhysSize = 0xFFFFFFF007A68008,
//         .kernelcache__gVirtBase = 0xFFFFFFF007A68010,
//         .kernelcache__perfmon_devices = 0xFFFFFFF007B54000,
//         .kernelcache__perfmon_dev_open = 0xFFFFFFF007B54800,
//         .kernelcache__ptov_table = 0xFFFFFFF007C00000,
//         .kernelcache__vm_first_phys_ppnum = 0xFFFFFFF007C00008,
//         .kernelcache__vm_pages = 0xFFFFFFF007C01000,
//         .kernelcache__vm_page_array_beginning_addr = 0xFFFFFFF007C02000,
//         .kernelcache__vm_page_array_ending_addr = 0xFFFFFFF007C03000,
//         .kernelcache__vn_kqfilter = 0xFFFFFFF007D00000,
//     },
// };

// #endif /* dynamic_info_h */

#ifndef dynamic_info_h
#define dynamic_info_h

#include <stdint.h>
typedef uint64_t u64;

struct dynamic_info {
    const char* kern_version;

    // --- offsets ---
    u64 fileglob__fg_ops;
    u64 fileglob__fg_data;
    u64 fileops__fo_kqfilter;
    u64 fileproc_guard__fpg_guard;

    u64 kqworkloop__kqwl_state;
    u64 kqworkloop__kqwl_p;
    u64 kqworkloop__kqwl_owner;
    u64 kqworkloop__kqwl_dynamicid;
    u64 kqworkloop__object_size;

    u64 pmap__tte;
    u64 pmap__ttep;

    u64 proc__p_list__le_next;
    u64 proc__p_list__le_prev;
    u64 proc__p_pid;
    u64 proc__p_fd__fd_ofiles;
    u64 proc__object_size;

    u64 pseminfo__psem_usecount;
    u64 pseminfo__psem_uid;
    u64 pseminfo__psem_gid;
    u64 pseminfo__psem_name;
    u64 pseminfo__psem_semobject;

    u64 semaphore__owner;
    u64 specinfo__si_rdev;

    u64 task__map;
    u64 task__threads__next;
    u64 task__threads__prev;
    u64 task__itk_space;
    u64 task__object_size;

    u64 thread__task_threads__next;
    u64 thread__task_threads__prev;
    u64 thread__map;
    u64 thread__thread_id;
    u64 thread__object_size;

    u64 uthread__object_size;

    u64 vm_map_entry__links__prev;
    u64 vm_map_entry__links__next;
    u64 vm_map_entry__links__start;
    u64 vm_map_entry__links__end;
    u64 vm_map_entry__store__entry__rbe_left;
    u64 vm_map_entry__store__entry__rbe_right;
    u64 vm_map_entry__store__entry__rbe_parent;

    u64 vnode__v_un__vu_specinfo;

    u64 _vm_map__hdr__links__prev;
    u64 _vm_map__hdr__links__next;
    u64 _vm_map__hdr__links__start;
    u64 _vm_map__hdr__links__end;
    u64 _vm_map__hdr__nentries;
    u64 _vm_map__hdr__rb_head_store__rbh_root;
    u64 _vm_map__pmap;
    u64 _vm_map__hint;
    u64 _vm_map__hole_hint;
    u64 _vm_map__holes_list;
    u64 _vm_map__object_size;

    u64 kernelcache__kernel_base;
    u64 kernelcache__cdevsw;
    u64 kernelcache__gPhysBase;
    u64 kernelcache__gPhysSize;
    u64 kernelcache__gVirtBase;
    u64 kernelcache__perfmon_devices;
    u64 kernelcache__perfmon_dev_open;
    u64 kernelcache__ptov_table;
    u64 kernelcache__vm_first_phys_ppnum;
    u64 kernelcache__vm_pages;
    u64 kernelcache__vm_page_array_beginning_addr;
    u64 kernelcache__vm_page_array_ending_addr;
    u64 kernelcache__vn_kqfilter;

    // --- flags moved to bottom for alignment safety ---
    bool kread_kqueue_workloop_ctl_supported;
    bool perf_supported;
    char _pad[6]; // explicit padding to keep struct 8-byte aligned
};

static struct dynamic_info kern_versions[] = {
    {
        .kern_version = "Darwin Kernel Version 21.6.0: Sun Oct 15 00:17:00 PDT 2023; root:xnu-8020.241.42~8/RELEASE_ARM64_S8000",
        .fileglob__fg_ops = 0x10,
        .fileglob__fg_data = 0x38,
        .fileops__fo_kqfilter = 0x30,
        .fileproc_guard__fpg_guard = 0x0,
        .kqworkloop__kqwl_state = 0x10,
        .kqworkloop__kqwl_p = 0x18,
        .kqworkloop__kqwl_owner = 0x40,
        .kqworkloop__kqwl_dynamicid = 0x58,
        .kqworkloop__object_size = 0x2B0,
        .pmap__tte = 0x0,
        .pmap__ttep = 0x8,
        .proc__p_list__le_next = 0x0,
        .proc__p_list__le_prev = 0x8,
        .proc__p_pid = 0x68,
        .proc__p_fd__fd_ofiles = 0xF8,
        .proc__object_size = 0x4B0,
        .pseminfo__psem_usecount = 0x10,
        .pseminfo__psem_uid = 0x14,
        .pseminfo__psem_gid = 0x18,
        .pseminfo__psem_name = 0x20,
        .pseminfo__psem_semobject = 0x30,
        .semaphore__owner = 0x10,
        .specinfo__si_rdev = 0x10,
        .task__map = 0x28,
        .task__threads__next = 0xD8,
        .task__threads__prev = 0xE0,
        .task__itk_space = 0x300,
        .task__object_size = 0x640,
        .thread__task_threads__next = 0x38,
        .thread__task_threads__prev = 0x40,
        .thread__map = 0x50,
        .thread__thread_id = 0x440,
        .thread__object_size = 0x620,
        .uthread__object_size = 0x4D8,
        .vm_map_entry__links__prev = 0x0,
        .vm_map_entry__links__next = 0x8,
        .vm_map_entry__links__start = 0x10,
        .vm_map_entry__links__end = 0x18,
        .vm_map_entry__store__entry__rbe_left = 0x20,
        .vm_map_entry__store__entry__rbe_right = 0x28,
        .vm_map_entry__store__entry__rbe_parent = 0x30,
        .vnode__v_un__vu_specinfo = 0x78,
        ._vm_map__hdr__links__prev = 0x10,
        ._vm_map__hdr__links__next = 0x18,
        ._vm_map__hdr__links__start = 0x20,
        ._vm_map__hdr__links__end = 0x28,
        ._vm_map__hdr__nentries = 0x30,
        ._vm_map__hdr__rb_head_store__rbh_root = 0x38,
        ._vm_map__pmap = 0x40,
        ._vm_map__hint = 0x80,
        ._vm_map__hole_hint = 0x80,
        ._vm_map__holes_list = 0x88,
        ._vm_map__object_size = 0xA0,
        .kernelcache__kernel_base = 0xfffffff007004000,
        .kernelcache__cdevsw = 0xFFFFFFF0079A8000,
        .kernelcache__gPhysBase = 0xFFFFFFF007A68000,
        .kernelcache__gPhysSize = 0xFFFFFFF007A68008,
        .kernelcache__gVirtBase = 0xFFFFFFF007A68010,
        .kernelcache__perfmon_devices = 0xFFFFFFF007B54000,
        .kernelcache__perfmon_dev_open = 0xFFFFFFF007B54800,
        .kernelcache__ptov_table = 0xFFFFFFF007C00000,
        .kernelcache__vm_first_phys_ppnum = 0xFFFFFFF007C00008,
        .kernelcache__vm_pages = 0xFFFFFFF007C01000,
        .kernelcache__vm_page_array_beginning_addr = 0xFFFFFFF007C02000,
        .kernelcache__vm_page_array_ending_addr = 0xFFFFFFF007C03000,
        .kernelcache__vn_kqfilter = 0xFFFFFFF007D00000,
        .kread_kqueue_workloop_ctl_supported = false,
        .perf_supported = true,
    },
    // Placeholder for iOS 16.x / macOS 13.x to avoid null derefs
    {
        .kern_version = "Darwin Kernel Version 22.x.x / arm64e",
        .kread_kqueue_workloop_ctl_supported = false,
        .perf_supported = true,
    },
};

#endif /* dynamic_info_h */
