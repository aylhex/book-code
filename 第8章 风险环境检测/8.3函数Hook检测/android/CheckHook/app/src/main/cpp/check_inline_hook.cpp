//
// Created by Ayl on 2022/12/10.
//

#include "check_inline_hook.h"
//
// Created by Ayl on 2022/8/18.
//
using namespace std;

string changeMachineCodeToHex(const char * input, int len)
{
    string hex_code;
    for(int i = 0; i < len; ++i)
    {
        char buffer[10] = {0};
        sprintf(buffer,"%02X ", input[i]);
        hex_code.append(buffer);
    }
    return hex_code;
}

/*
 * 释放内存
 */
InlineHook:: ~InlineHook()
{
    if (lib_fp)
    {
        fclose(lib_fp);
    }
    if (base_addr != nullptr && base_addr != MAP_FAILED)
    {
        munmap(base_addr, lib_file_size);
    }
}
/*
 * 加载elf到内存, 初始化：节头表指针 程序头表指针
 */
bool InlineHook::initELF(const char * lib_path)
{
    lib_fp = fopen(lib_path, "rb");
    if (lib_fp == nullptr)
    {
        return false;
    }

    int fd = fileno(lib_fp);
    struct stat stat;
    if (fstat(fd, &stat) != 0)
    {
        return false;
    }
    lib_file_size = static_cast<size_t>(stat.st_size);
    base_addr = mmap(nullptr, lib_file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (base_addr == MAP_FAILED)
    {
        return false;
    }
    //初始化Elf header
    p_Elf_Ehdr = static_cast<Elf_Ehdr *>(base_addr);
    p_Elf_Shdr = reinterpret_cast<Elf_Shdr *>((char *)base_addr + p_Elf_Ehdr->e_shoff);
    initSectionPtrs(base_addr);
    return true;
}

/**
 * 初始化各种常用的指针，方便其它功能使用
 * @param p_base elf文件映射到内存的起始地址
 */
void InlineHook::initSectionPtrs(void * p_base)
{
    p_shstrtab = ((char *)p_base + p_Elf_Shdr[p_Elf_Ehdr->e_shstrndx].sh_offset);;

    int sh_num = p_Elf_Ehdr->e_shnum;
    for(int indx = 0; indx < sh_num; ++indx)
    {
        switch (p_Elf_Shdr[indx].sh_type)
        {
            case SHT_DYNSYM:
            {//.dynsym (导入导出的符号(函数，全局变量))
                p_Elf_Sym_Dynsym = reinterpret_cast<Elf_Sym *>((char *) p_base + p_Elf_Shdr[indx].sh_offset);
                dynsym_item_count = p_Elf_Shdr[indx].sh_size / p_Elf_Shdr[indx].sh_entsize;
                break;
            }
            case SHT_STRTAB:
            {//.dynstr .shstrtab .strtab
                char *name = p_shstrtab + p_Elf_Shdr[indx].sh_name;
                char *table = ((char *) p_base + p_Elf_Shdr[indx].sh_offset);
                if (strcmp(name, ".dynstr") == 0)
                {
                    p_dynstr = table;
                }
                break;
            }
        }
    }
}

/**
 * 从dynsym对应的符号表中查找: 函数或者变量 对应的Elf_Sym结构体
 * @param target_func 要查找的符号名
 * @param sym 传出参数，如果查询成功，sym保存了to_find符号对应的符号信息
 * @return 成功返回true, 识别返回false
 */
bool InlineHook::findTargetFuncSym(const char* target_func, Elf_Sym & sym)
{
    bool result = false;
    if(target_func == nullptr || p_Elf_Sym_Dynsym == nullptr || dynsym_item_count == 0)
    {
        return result;
    }

    for(int index = 0; index < dynsym_item_count; ++index)
    {
        //st_name == 0, 表示符号没有名字，不是我们想要的
        if(p_Elf_Sym_Dynsym[index].st_name == 0)
        {
            continue;
        }

        const char * sys_name = p_dynstr + p_Elf_Sym_Dynsym[index].st_name;
        int type = ELF_ST_TYPE(p_Elf_Sym_Dynsym[index].st_info);

        if(strcmp(target_func, sys_name) == 0 && (type == STT_FUNC || type == STT_OBJECT))
        {//查找成功
            sym = p_Elf_Sym_Dynsym[index];
            result = true;
            break;
        }
    }
    return result;
}

bool InlineHook::getMachineCode(const char * target_func, char * buffer, int buffer_len)
{
    Elf_Sym sym;
    if(findTargetFuncSym(target_func, sym))
    {
        char * p_machine_code = nullptr;
        char * p_file = static_cast<char *>(base_addr);

        if(sym.st_value & 0x00000001)
        {   //for thumb
            p_machine_code = p_file + (sym.st_value - 1);
        }
        else
        {
            p_machine_code = p_file + sym.st_value;
        }

        if(buffer_len > sym.st_size)
        {
            memcpy(buffer, p_machine_code, sym.st_size);
        }
        else
        {
            memcpy(buffer, p_machine_code, buffer_len);
        }
        return true;
    }
    return false;
}

bool InlineHook::checkInlineHook(const char *target_func, int system_version)
{
    if (target_func == nullptr)
    {
        return false;
    }

    if (!initELF(LIB_PATH))
    {
        LOGE("initELF libc.so failed");
        return false;
    }
    void * p_lib_handle = dlopen(LIB_PATH, RTLD_NOW);
    if(!p_lib_handle)
    {
        LOGE("load libc.so failed");
        return false;
    }
    char file_code_buffer[8] = {0}; // 用于存储文件中的机器码
    char mem_code_buffer[8] = {0};  // 用于存储内存中的机器码

    // 获取文件中函数的机器码
    if(!getMachineCode(target_func, file_code_buffer, sizeof(file_code_buffer)))
    {
        dlclose(p_lib_handle);
        LOGE("parse libc.so failed");
        return false;
    }
    // 获取函数地址
    intptr_t p_func_address = reinterpret_cast<intptr_t>(dlsym(p_lib_handle, target_func));
    // android10部分机器代码段不可读的,强制修改为可读
    if(system_version >= 1)
    {
        intptr_t start = PAGE_START(p_func_address);
        int ret = mprotect(reinterpret_cast<void *>(start), static_cast<size_t>(getpagesize()), PROT_READ | PROT_EXEC);
        if(ret != 0)
        {
            dlclose(p_lib_handle);
            LOGE("mprotect failed");
            return false;
        }
    }
    //根据指令类型不同，从不同的起始地址拷贝机器码
    if(p_func_address & 0x00000001)
    {
        memcpy(mem_code_buffer, (char *)(p_func_address - 1), sizeof(mem_code_buffer));
    }
    else
    {
        memcpy(mem_code_buffer, (char *)p_func_address, sizeof(mem_code_buffer));
    }

    string hex_code_of_file = changeMachineCodeToHex(file_code_buffer, sizeof(file_code_buffer));
    string hex_code_of_mem = changeMachineCodeToHex(mem_code_buffer, sizeof(file_code_buffer));

    LOGE("file code:%s",hex_code_of_file.c_str());
    LOGE("meme code:%s",hex_code_of_mem.c_str());
    dlclose(p_lib_handle);

    if (hex_code_of_file == hex_code_of_mem)
    {
        return false;
    }
    return true;
}