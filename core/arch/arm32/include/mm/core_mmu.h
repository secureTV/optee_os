/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef CORE_MMU_H
#define CORE_MMU_H

#include <stdint.h>
#include <kernel/tee_common_unpg.h>
#include <mm/core_memprot.h>

/*
 * @type:  enumerate: specifiy the purpose of the memory area.
 * @pa:    memory area physical start address
 * @size:  memory area size in bytes
 * @va:    virtual start address (0 if memory is not mapped)
 * @region_size: size of the mapping region used (4k, 64K, 1MB)
 * @secure: true if memory area in inside a A9 secure area
 */
struct map_area {
	unsigned int type;
	unsigned int pa;
	size_t size;
	/* below here are core_mmu.c internal data */
	unsigned int va;
	unsigned int region_size;
	bool secure;
	bool cached;
	bool device;
	bool rw;
	bool exec;
};

/*
 * Memory area type:
 * MEM_AREA_NOTYPE:   Undefined type. Used as end of table.
 * MEM_AREA_TEE_RAM:  teecore execution RAM (secure, reserved to TEEtz, unused)
 * MEM_AREA_TEE_COHERENT: teecore coherent RAM (secure, reserved to TEEtz)
 * MEM_AREA_TA_RAM:   Secure RAM where teecore loads/exec TA instances.
 * MEM_AREA_NS_SHM:   NonSecure shared RAM between NSec and TEEtz.
 * MEM_AREA_KEYVAULT: Secure RAM storing some secrets
 * MEM_AREA_IO_SEC:   Secure HW mapped registers
 * MEM_AREA_IO_NSEC:  NonSecure HW mapped registers
 * MEM_AREA_MAXTYPE:  lower invalid 'type' value
 */
enum teecore_memtypes {
	MEM_AREA_NOTYPE = 0,
	MEM_AREA_TEE_RAM,
	MEM_AREA_TEE_COHERENT,
	MEM_AREA_TA_RAM,
	MEM_AREA_NSEC_SHM,
	MEM_AREA_KEYVAULT,
	MEM_AREA_IO_SEC,
	MEM_AREA_IO_NSEC,
	MEM_AREA_MAXTYPE
};

/* Default NSec shared memory allocated from NSec world */
extern unsigned long default_nsec_shm_paddr;
extern unsigned long default_nsec_shm_size;

uint32_t core_map_area_flag(void *p, size_t l);
unsigned int core_init_mmu(unsigned int core_ttbr0, unsigned int ta_ttbr0);

int core_mmu_map(unsigned long paddr, size_t size, unsigned long flags);
int core_mmu_unmap(unsigned long paddr, size_t size);

void core_mmu_get_mem_by_type(unsigned int type, unsigned int *s,
			      unsigned int *e);

int core_va2pa(uint32_t va, uint32_t *pa);
int core_pa2va(uint32_t pa, uint32_t *va);

/* get phys base addr of MMU L1 table used for tee core */
uint32_t core_mmu_get_ttbr0_base(void);
uint32_t core_mmu_get_ttbr0_attr(void);
uint32_t core_mmu_get_ttbr0(void);

/* get virt and phys base addr of MMU L1 table used for TAs */
uint32_t core_mmu_get_ta_ul1_va(void);
uint32_t core_mmu_get_ta_ul1_pa(void);

/* routines to retreive shared mem configuration */
bool core_mmu_is_shm_cached(void);

/* L1/L2 cache maintenance (op: refer to ???) */
unsigned int core_cache_maintenance(int op, void *start, size_t len);
unsigned int cache_maintenance_l2(int op, void *start, size_t len);
void core_l2cc_mutex_set(void *mutex);
void core_l2cc_mutex_activate(bool en);
void core_l2cc_mutex_lock(void);
void core_l2cc_mutex_unlock(void);

/* various invalidate secure TLB */
enum teecore_tlb_op {
	TLBINV_DATATLB,		/* invalidate data tlb */
	TLBINV_UNIFIEDTLB,	/* invalidate unified tlb */
	TLBINV_CURRENT_ASID,	/* invalidate unified tlb for current ASID */
	TLBINV_BY_ASID,		/* invalidate unified tlb by ASID */
	TLBINV_BY_MVA,		/* invalidate unified tlb by MVA */
};

struct map_area *bootcfg_get_memory(void);
int core_tlb_maintenance(int op, unsigned int a);
unsigned long bootcfg_get_pbuf_is_handler(void);

/* Cache maintenance operation type */
typedef enum {
	DCACHE_CLEAN = 0x1,
	DCACHE_AREA_CLEAN = 0x2,
	DCACHE_INVALIDATE = 0x3,
	DCACHE_AREA_INVALIDATE = 0x4,
	ICACHE_INVALIDATE = 0x5,
	ICACHE_AREA_INVALIDATE = 0x6,
	WRITE_BUFFER_DRAIN = 0x7,
	DCACHE_CLEAN_INV = 0x8,
	DCACHE_AREA_CLEAN_INV = 0x9,
	L2CACHE_INVALIDATE = 0xA,
	L2CACHE_AREA_INVALIDATE = 0xB,
	L2CACHE_CLEAN = 0xC,
	L2CACHE_AREA_CLEAN = 0xD,
	L2CACHE_CLEAN_INV = 0xE,
	L2CACHE_AREA_CLEAN_INV = 0xF
} t_cache_operation_id;

#endif /* CORE_MMU_H */
