//
//  offsets.c
//  kfd
//
//  Created by Seo Hyun-gyu on 2023/07/29.
//  Edited by Arslaan Pathan on 2025/08/01.
//

#include "offsets.h"
#include "../../../libjailbreak_objc/libjailbreak/libjailbreak/info.h"
#include "../../../libjailbreak_objc/libjailbreak/libjailbreak/primitives_external.h"
#include "../libkfd.h"
#undef T1SZ_BOOT
#include <Foundation/Foundation.h>
#import <os/proc.h>
#import <xpc/xpc.h>

uint32_t off_p_list_le_prev = 0;
uint32_t off_p_proc_ro = 0;
uint32_t off_p_ppid = 0;
uint32_t off_p_original_ppid = 0;
uint32_t off_p_pgrpid = 0;
uint32_t off_p_uid = 0;
uint32_t off_p_gid = 0;
uint32_t off_p_ruid = 0;
uint32_t off_p_rgid = 0;
uint32_t off_p_svuid = 0;
uint32_t off_p_svgid = 0;
uint32_t off_p_sessionid = 0;
uint32_t off_p_puniqueid = 0;
uint32_t off_p_pid = 0;
uint32_t off_p_pfd = 0;
uint32_t off_p_textvp = 0;
uint32_t off_p_name = 0;
uint32_t off_p_ro_p_csflags = 0;
uint32_t off_p_ro_p_ucred = 0;
uint32_t off_p_ro_pr_proc = 0;
uint32_t off_p_ro_pr_task = 0;
uint32_t off_p_ro_t_flags_ro = 0;
uint32_t off_u_cr_label = 0;
uint32_t off_u_cr_posix = 0;
uint32_t off_cr_uid = 0;
uint32_t off_cr_ruid = 0;
uint32_t off_cr_svuid = 0;
uint32_t off_cr_ngroups = 0;
uint32_t off_cr_groups = 0;
uint32_t off_cr_rgid = 0;
uint32_t off_cr_svgid = 0;
uint32_t off_cr_gmuid = 0;
uint32_t off_cr_flags = 0;
uint32_t off_task_t_flags = 0;
uint32_t off_task_itk_space = 0;
uint32_t off_fd_ofiles = 0;
uint32_t off_fd_cdir = 0;
uint32_t off_fp_glob = 0;
uint32_t off_fg_data = 0;
uint32_t off_fg_flag = 0;
uint32_t off_vnode_v_ncchildren_tqh_first = 0;
uint32_t off_vnode_v_ncchildren_tqh_last = 0;
uint32_t off_vnode_v_nclinks_lh_first = 0;
uint32_t off_vnode_v_iocount = 0;
uint32_t off_vnode_v_usecount = 0;
uint32_t off_vnode_v_flag = 0;
uint32_t off_vnode_v_name = 0;
uint32_t off_vnode_v_mount = 0;
uint32_t off_vnode_v_data = 0;
uint32_t off_vnode_v_kusecount = 0;
uint32_t off_vnode_v_references = 0;
uint32_t off_vnode_v_lflag = 0;
uint32_t off_vnode_v_owner = 0;
uint32_t off_vnode_v_parent = 0;
uint32_t off_vnode_v_label = 0;
uint32_t off_vnode_v_cred = 0;
uint32_t off_vnode_v_writecount = 0;
uint32_t off_vnode_v_type = 0;
uint32_t off_vnode_v_id = 0;
uint32_t off_vnode_vu_ubcinfo = 0;
uint32_t off_mount_mnt_data = 0;
uint32_t off_mount_mnt_fsowner = 0;
uint32_t off_mount_mnt_fsgroup = 0;
uint32_t off_mount_mnt_devvp = 0;
uint32_t off_mount_mnt_flag = 0;
uint32_t off_specinfo_si_flags = 0;
uint32_t off_specinfo_si_rdev = 0;
uint32_t off_namecache_nc_dvp = 0;
uint32_t off_namecache_nc_vp = 0;
uint32_t off_namecache_nc_hashval = 0;
uint32_t off_namecache_nc_name = 0;
uint32_t off_namecache_nc_child_tqe_prev = 0;
uint32_t off_ipc_space_is_table = 0;
uint32_t off_ubc_info_cs_blobs = 0;
uint32_t off_ubc_info_cs_add_gen = 0;
uint32_t off_cs_blob_csb_pmap_cs_entry = 0;
uint32_t off_cs_blob_csb_cdhash = 0;
uint32_t off_cs_blob_csb_flags = 0;
uint32_t off_cs_blob_csb_teamid = 0;
uint32_t off_cs_blob_csb_validation_category = 0;
uint32_t off_pmap_cs_code_directory_ce_ctx = 0;
uint32_t off_pmap_cs_code_directory_der_entitlements_size = 0;
uint32_t off_pmap_cs_code_directory_trust = 0;
uint32_t off_ipc_entry_ie_object = 0;
uint32_t off_ipc_object_io_bits = 0;
uint32_t off_ipc_object_io_references = 0;
uint32_t off_ipc_port_ip_kobject = 0;

uint64_t off_gphysbase = 0;
uint64_t off_gphysize = 0;
uint64_t off_gvirtbase = 0;
uint64_t off_ptov__table = 0;

void _offsets_init(void) {
    bool isiOS15 = false;
    
    u64 kread_method = 0, kwrite_method = 0;
    kread_method = kread_sem_open;
    kwrite_method = kwrite_sem_open;
    
    uint64_t vm_map__pmap = koffsetof(vm_map, pmap);

    uint64_t pmap_to_hint = 0; // offset between vm_map->pmap and vm_map->hint
    if (@available(iOS 16.0, *)) {
        pmap_to_hint = 0x58;
    }
    else if(@available(iOS 15.4, *)) {
        pmap_to_hint = 0x38;
    }
    else {
        pmap_to_hint = 0xB8;
    }

    dynamic_system_info = (struct dynamic_info){
        .kread_kqueue_workloop_ctl_supported = true,
        .krkw_iosurface_supported = false,
        .perf_supported = true,
        
        .kernelcache__static_base = kconstant(staticBase),

        .proc__p_list__le_next = koffsetof(proc, list_next),
        .proc__p_list__le_prev = koffsetof(proc, list_prev),
        .proc__p_pid           = koffsetof(proc, pid),
        .proc__p_fd__fd_ofiles = koffsetof(proc, fd) + koffsetof(filedesc, ofiles_start),
        .proc__object_size     = ksizeof(proc),
    
        .task__map = koffsetof(task, map),
    
        .vm_map__hdr_links_prev             = koffsetof(vm_map, hdr) + koffsetof(vm_map_header, links) + koffsetof(vm_map_links, prev),
        .vm_map__hdr_links_next             = koffsetof(vm_map, hdr) + koffsetof(vm_map_header, links) + koffsetof(vm_map_links, next),
        .vm_map__min_offset                 = koffsetof(vm_map, hdr) + koffsetof(vm_map_header, links) + koffsetof(vm_map_links, min),
        .vm_map__max_offset                 = koffsetof(vm_map, hdr) + koffsetof(vm_map_header, links) + koffsetof(vm_map_links, max),
        .vm_map__hdr_nentries               = koffsetof(vm_map, hdr) + koffsetof(vm_map_header, links) + koffsetof(vm_map_links, max) + 0x8,
        .vm_map__hdr_nentries_u64           = koffsetof(vm_map, hdr) + koffsetof(vm_map_header, links) + koffsetof(vm_map_links, max) + 0x8,
        .vm_map__hdr_rb_head_store_rbh_root = koffsetof(vm_map, hdr) + koffsetof(vm_map_header, links) + koffsetof(vm_map_links, max) + 0x18,
    
        .vm_map__pmap        = vm_map__pmap,
        .vm_map__hint        = vm_map__pmap + pmap_to_hint,
        .vm_map__hole_hint   = vm_map__pmap + pmap_to_hint + 0x8,
        .vm_map__holes_list  = vm_map__pmap + pmap_to_hint + 0x10,
        .vm_map__object_size = vm_map__pmap + pmap_to_hint + 0x28,
        
        .IOSurface__isa                 =   0x0,
        .IOSurface__pixelFormat         =  0xa4,
        .IOSurface__allocSize           =  0xac,
        .IOSurface__useCountPtr         =  0xc0,
        .IOSurface__indexedTimestampPtr = 0x360,
        .IOSurface__readDisplacement    =  0x14,

        .thread__thread_id = 0x400, // TODO: Universalize (Only relevant for kread_kqueue_workloop_ctl)

        .kernelcache__allproc          = ksymbol(allproc),
        
        .kernelcache__cdevsw           = ksymbol(cdevsw),
        .kernelcache__gPhysBase        = ksymbol(gPhysBase),
        .kernelcache__gPhysSize        = ksymbol(gPhysSize),
        .kernelcache__gVirtBase        = ksymbol(gVirtBase),
        .kernelcache__perfmon_dev_open = ksymbol(perfmon_dev_open),
        .kernelcache__perfmon_devices  = ksymbol(perfmon_devices),
        .kernelcache__ptov_table       = ksymbol(ptov_table),
        .kernelcache__vn_kqfilter      = ksymbol(vn_kqfilter),
        
        .device__T1SZ_BOOT            = kconstant(T1SZ_BOOT),
        .device__ARM_TT_L1_INDEX_MASK = kconstant(ARM_TT_L1_INDEX_MASK),
    };

    if (isiOS15) {
        dynamic_system_info.proc__task = 0x10;
    }
    if (@available(iOS 15.4, *)) {
        dynamic_system_info.vm_map__hdr_rb_head_store_rbh_root -= 0x8;
    }
    
    // Kernelcache physical/virtual base addresses
    off_gphysbase = 0xFFFFFFF007A68000;
    off_gphysize = 0xFFFFFFF007A68008;
    off_gvirtbase = 0xFFFFFFF007A68010;
    off_ptov__table = 0xFFFFFFF007C00000;

    // proc struct offsets
    off_p_list_le_prev = 0x8;         // proc__p_list__le_prev
    off_p_pid = 0x68;                 // proc__p_pid
    off_p_pfd = 0xF8;                 // proc__p_fd__fd_ofiles

    // fileproc/fileglob offsets
    off_fp_glob = 0x10;               // fileglob__fg_ops (fileproc -> fileglob pointer offset)
    off_fg_data = 0x38;               // fileglob__fg_data

    // specinfo offsets
    off_specinfo_si_rdev = 0x10;     // specinfo__si_rdev

    // task struct offsets
    off_task_itk_space = 0x300;      // task__itk_space
}
