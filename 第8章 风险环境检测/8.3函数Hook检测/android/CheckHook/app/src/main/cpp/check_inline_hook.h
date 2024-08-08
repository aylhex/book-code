//
// Created by Ayl on 2022/12/10.
//

#ifndef CHECK_INLINE_HOOK_H
#define CHECK_INLINE_HOOK_H
#include <jni.h>
#include <fcntl.h>
#include <cstring>
#include <sys/system_properties.h>
#include <sys/mman.h>
#include <string>
#include <dlfcn.h>
#include <sys/ptrace.h>
#include "common.h"
#include "comm_elf.h"

#if defined(__aarch64__)
#define LIB_PATH "/system/lib64/libc.so"
#else
#define LIB_PATH "/system/lib/libc.so"
#endif

// Returns the address of the page containing address 'x'.
#define PAGE_START(x)  ((x) & PAGE_MASK) // 把后12位置0

class InlineHook
{
public:
    InlineHook(){
        base_addr = nullptr;
        lib_fp = nullptr;
        p_Elf_Ehdr = nullptr;
        lib_file_size = 0;

        p_Elf_Sym_Dynsym = nullptr;
        dynsym_item_count = 0;
        p_shstrtab = nullptr;
        p_dynstr = nullptr;
    }

    virtual ~InlineHook();


    bool checkInlineHook(const char* symbol, int version);

private:
    void initSectionPtrs(void * p_base);

    bool initELF(const char * lib_path);

    //获取函数的机器码
    bool getMachineCode(const char * target_func, char * buffer, int buffer_len);

    //给一个符号，查找它在dynsym表中对应的信息, 查找效率低
    bool findTargetFuncSym(const char* target_func, Elf_Sym & sym);

private:
    void * base_addr;
    size_t lib_file_size;
    FILE* lib_fp;
    Elf_Ehdr * p_Elf_Ehdr;  //ELF文件头
    Elf_Shdr * p_Elf_Shdr;  //ELF节头

    Elf_Sym * p_Elf_Sym_Dynsym;    //指向动态符号表数组
    size_t dynsym_item_count;      //数组元素个数

    char * p_shstrtab;             // 指向.shstrtab，字符串表, 这个表只存储节的名字
    char * p_dynstr;               // 指向.dynstr，字符串表，这个表里面存储了所有用于动态链接的符号(函数名，变量名)的名字

};

#endif //CHECK_INLINE_HOOK_H
