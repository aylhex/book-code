//
// Created by Ayl on 2022/8/18.
//

#ifndef CHECK_GOT_HOOK_H
#define CHECK_GOT_HOOK_H
#include <jni.h>
#include <string>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <elf.h>
#include "comm_elf.h"
#include "common.h"

#if defined(__aarch64__)
#define LIB_PATH "/system/lib64/libc.so"
#else
#define LIB_PATH "/system/lib/libc.so"
#endif

#if defined(__arm__)
#define ELF_R_JUMP_SLOT R_ARM_JUMP_SLOT     //.rela.plt
#define ELF_R_GLOB_DAT  R_ARM_GLOB_DAT      //.rela.dyn
#define ELF_R_ABS       R_ARM_ABS32         //.rela.dyn
#elif defined(__aarch64__)
#define ELF_R_JUMP_SLOT R_AARCH64_JUMP_SLOT
#define ELF_R_GLOB_DAT  R_AARCH64_GLOB_DAT
#define ELF_R_ABS       R_AARCH64_ABS64
#endif


class GotHook
{
public:
    GotHook(){
        base_addr = nullptr;
        lib_base = 0;
        p_Elf_Ehdr = nullptr;
        p_Elf_Shdr = nullptr;
        p_Elf_Phdr = nullptr;
        p_Elf_Dyn = nullptr;
        p_Elf_Rel_Table = nullptr;
    }

    bool checkGotHook(const char* lib_name, const char* func_name);

private:
    bool initELF(const char * lib_path);
    bool initDynamic();
    uintptr_t getFuncAddrInMem(const char* target_func);

private:

    typedef struct{
        uint64_t str_tab_offset;
        uint64_t sym_tab_offset;
        uint64_t rela_plt_offset;
        uint32_t rela_plt_size;
        uint64_t rela_dyn_offset;
        uint32_t rela_dyn_size;
    }Elf_Rel_Dyn;

    typedef struct{ //.hash
        const uint32_t *buckets;
        uint32_t buckets_cnt;
        const uint32_t *chains;
        uint32_t chains_cnt;
    }Elf_Hash;

    void * base_addr;
    Elf_Ehdr * p_Elf_Ehdr;  			//ELF文件头
    Elf_Shdr * p_Elf_Shdr;  			//ELF节头
    Elf_Phdr * p_Elf_Phdr;  			//ELF程序头

    Elf_Dyn * p_Elf_Dyn;                //指向.dynamic段
    Elf_Rel * p_Elf_Rel_Table;		    //指向重定位表
    Elf_Sym * p_Elf_Sym_Got_Table;      //指向Got表数组

    uintptr_t lib_base;

    Elf_Rel_Dyn p_Elf_Rel_Dyn;	    //指向重定向信息
    Elf_Hash p_Elf_Hash;
};
#endif //CHECK_GOT_HOOK_H
