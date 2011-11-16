/*
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __ASMBFIN_ELF_H
#define __ASMBFIN_ELF_H

/*
 * ELF register definitions..
 */

#include <asm/ptrace.h>
#include <asm/user.h>

/* Processor specific flags for the ELF header e_flags field.  */
#define EF_BFIN_PIC		0x00000001	/* -fpic */
#define EF_BFIN_FDPIC		0x00000002	/* -mfdpic */
#define EF_BFIN_CODE_IN_L1	0x00000010	/* --code-in-l1 */
#define EF_BFIN_DATA_IN_L1	0x00000020	/* --data-in-l1 */
#define EF_BFIN_CODE_IN_L2	0x00000040	/* --code-in-l2 */
#define EF_BFIN_DATA_IN_L2	0x00000080	/* --data-in-l2 */

#if 1	/* core dumps not supported, but linux/elfcore.h needs these */
typedef unsigned long elf_greg_t;

#define ELF_NGREG (sizeof(struct pt_regs) / sizeof(elf_greg_t))
typedef elf_greg_t elf_gregset_t[ELF_NGREG];

typedef struct { } elf_fpregset_t;
#endif

/*
 * This is used to ensure we don't load something for the wrong architecture.
 */
#define elf_check_arch(x) ((x)->e_machine == EM_BLACKFIN)

#define elf_check_fdpic(x) ((x)->e_flags & EF_BFIN_FDPIC /* && !((x)->e_flags & EF_FRV_NON_PIC_RELOCS) */)
#define elf_check_const_displacement(x) ((x)->e_flags & EF_BFIN_PIC)

/* EM_BLACKFIN defined in linux/elf.h	*/

/*
 * These are used to set parameters in the core dumps.
 */
#define ELF_CLASS	ELFCLASS32
#define ELF_DATA	ELFDATA2LSB
#define ELF_ARCH	EM_BLACKFIN

#define ELF_PLAT_INIT(_r)	_r->p1 = 0

#define ELF_FDPIC_PLAT_INIT(_regs, _exec_map_addr, _interp_map_addr, _dynamic_addr)	\
do {											\
	_regs->r7	= 0;						\
	_regs->p0	= _exec_map_addr;				\
	_regs->p1	= _interp_map_addr;				\
	_regs->p2	= _dynamic_addr;				\
} while(0)

#if 0
#define CORE_DUMP_USE_REGSET
#endif
#define ELF_FDPIC_CORE_EFLAGS	EF_BFIN_FDPIC
#define ELF_EXEC_PAGESIZE	4096

#define R_BFIN_UNUSED0         0   /* relocation type 0 is not defined */
#define R_BFIN_PCREL5M2        1   /* LSETUP part a */
#define R_BFIN_UNUSED1         2   /* relocation type 2 is not defined */
#define R_BFIN_PCREL10         3   /* type 3, if cc jump <target> */
#define R_BFIN_PCREL12_JUMP    4   /* type 4, jump <target> */
#define R_BFIN_RIMM16          5   /* type 0x5, rN = <target> */
#define R_BFIN_LUIMM16         6   /* # 0x6, preg.l=<target> Load imm 16 to lower half */
#define R_BFIN_HUIMM16         7   /* # 0x7, preg.h=<target> Load imm 16 to upper half */
#define R_BFIN_PCREL12_JUMP_S  8   /* # 0x8 jump.s <target> */
#define R_BFIN_PCREL24_JUMP_X  9   /* # 0x9 jump.x <target> */
#define R_BFIN_PCREL24         10  /* # 0xa call <target> , not expandable */
#define R_BFIN_UNUSEDB         11  /* # 0xb not generated */
#define R_BFIN_UNUSEDC         12  /* # 0xc  not used */
#define R_BFIN_PCREL24_JUMP_L  13  /* 0xd jump.l <target> */
#define R_BFIN_PCREL24_CALL_X  14  /* 0xE, call.x <target> if <target> is above 24 bit limit call through P1 */
#define R_BFIN_VAR_EQ_SYMB     15  /* 0xf, linker should treat it same as 0x12 */
#define R_BFIN_BYTE_DATA       16  /* 0x10, .byte var = symbol */
#define R_BFIN_BYTE2_DATA      17  /* 0x11, .byte2 var = symbol */
#define R_BFIN_BYTE4_DATA      18  /* 0x12, .byte4 var = symbol and .var var=symbol */
#define R_BFIN_PCREL11         19  /* 0x13, lsetup part b */
#define R_BFIN_UNUSED14        20  /* 0x14, undefined */
#define R_BFIN_UNUSED15        21  /* not generated by VDSP 3.5 */

/* arithmetic relocations */
#define R_BFIN_PUSH            0xE0
#define R_BFIN_CONST           0xE1
#define R_BFIN_ADD             0xE2
#define R_BFIN_SUB             0xE3
#define R_BFIN_MULT            0xE4
#define R_BFIN_DIV             0xE5
#define R_BFIN_MOD             0xE6
#define R_BFIN_LSHIFT          0xE7
#define R_BFIN_RSHIFT          0xE8
#define R_BFIN_AND             0xE9
#define R_BFIN_OR              0xEA
#define R_BFIN_XOR             0xEB
#define R_BFIN_LAND            0xEC
#define R_BFIN_LOR             0xED
#define R_BFIN_LEN             0xEE
#define R_BFIN_NEG             0xEF
#define R_BFIN_COMP            0xF0
#define R_BFIN_PAGE            0xF1
#define R_BFIN_HWPAGE          0xF2
#define R_BFIN_ADDR            0xF3

/* This is the location that an ET_DYN program is loaded if exec'ed.  Typical
   use of this is to invoke "./ld.so someprog" to test out a new version of
   the loader.  We need to make sure that it is out of the way of the program
   that it will "exec", and that there is sufficient room for the brk.  */

#define ELF_ET_DYN_BASE         0xD0000000UL

#define ELF_CORE_COPY_REGS(pr_reg, regs)	\
        memcpy((char *) &pr_reg, (char *)regs,  \
               sizeof(struct pt_regs));

/* This yields a mask that user programs can use to figure out what
   instruction set this cpu supports.  */

#define ELF_HWCAP	(0)

/* This yields a string that ld.so will use to load implementation
   specific libraries for optimization.  This is more specific in
   intent than poking at uname or /proc/cpuinfo.  */

#define ELF_PLATFORM  (NULL)

#define SET_PERSONALITY(ex) set_personality(PER_LINUX)

#endif
