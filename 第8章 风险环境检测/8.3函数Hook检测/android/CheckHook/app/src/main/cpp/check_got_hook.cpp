//
// Created by Ayl on 2022/8/18.
//

#include "check_got_hook.h"


uintptr_t getFuncOrginalAddress(const char* lib_path, const char* func_name)
{
    void *handle = dlopen(lib_path, RTLD_LAZY);
    return (uintptr_t) dlsym(handle, func_name);
}

void getLibPath(const char *src, char *dest)
{
    while (*src != '/')
    {
        *src++;
    }
    strncpy(dest, src, strlen(src) - 1);
}

uintptr_t getLibModuleAddr(const char *module_name, char* lib_path)
{
    char buff[256] = "\n";
    char cmd[256] = "\n";
    uintptr_t addr = 0;
    pid_t pid = getpid();
    sprintf(cmd, "/proc/%d/maps",pid);
    FILE *fp = fopen(cmd, "r");
    while (fgets(buff, sizeof(buff), fp))
    {
        if (strstr(buff, module_name) && sscanf(buff, "%" SCNxPTR, &addr) == 1)
        {
            getLibPath(buff, lib_path);
            return addr;
        }
    }
    fclose(fp);
    return 0;
}

bool GotHook::initELF(const char * lib_path)
{
    FILE *fp = fopen(lib_path, "rb");
    if (fp == nullptr)
    {
        return false;
    }
    int fd = fileno(fp);
    struct stat stat;
    if (fstat(fd, &stat) != 0)
    {
        return false;
    }
    int len = static_cast<size_t>(stat.st_size);
    base_addr = mmap(nullptr, len, PROT_READ, MAP_PRIVATE, fd, 0);
    if (base_addr == MAP_FAILED)
    {
        return false;
    }
    //初始化Elf header
    p_Elf_Ehdr = static_cast<Elf_Ehdr *>(base_addr);
    p_Elf_Phdr = reinterpret_cast<Elf_Phdr *>((char *)base_addr + p_Elf_Ehdr->e_phoff);
    initDynamic();
    return true;
}

bool GotHook::initDynamic()
{
    uintptr_t dynamic_size = 0;
    uintptr_t dyn_offset = 0;
    int ph_num = p_Elf_Ehdr->e_phnum;
    for (int index = 0; index < ph_num; index++)
    {
        if (PT_DYNAMIC == p_Elf_Phdr[index].p_type)
        {
            dynamic_size = p_Elf_Phdr[index].p_memsz;
            dyn_offset =  p_Elf_Phdr[index].p_vaddr;
        }
    }
    p_Elf_Dyn = reinterpret_cast<Elf_Dyn *>((char *) lib_base + dyn_offset);
    for (int i = 0; i < dynamic_size/sizeof(Elf_Dyn); ++i)
    {
        switch (p_Elf_Dyn[i].d_tag)
        {
            case DT_SYMTAB: // .dynsym
                p_Elf_Rel_Dyn.sym_tab_offset = p_Elf_Dyn[i].d_un.d_ptr;
                break;
            case DT_STRTAB: // .dynstr
                p_Elf_Rel_Dyn.str_tab_offset =  p_Elf_Dyn[i].d_un.d_ptr;
                break;
            case DT_JMPREL:// .rel.plt / .rela.plt
                p_Elf_Rel_Dyn.rela_plt_offset = p_Elf_Dyn[i].d_un.d_ptr;
                break;
            case DT_PLTRELSZ:
                p_Elf_Rel_Dyn.rela_plt_size = p_Elf_Dyn[i].d_un.d_val/sizeof(Elf_Rela);
                break;
            case DT_RELA: // .rel.dyn / .rela.dyn
                p_Elf_Rel_Dyn.rela_dyn_offset = p_Elf_Dyn[i].d_un.d_ptr;
                break;
            case DT_RELASZ:
                p_Elf_Rel_Dyn.rela_dyn_size = p_Elf_Dyn[i].d_un.d_val / sizeof(Elf_Rel);
                break;
            case DT_HASH: // .hash
                auto rawdata = (const uint32_t *) (lib_base + p_Elf_Dyn[i].d_un.d_ptr);
                p_Elf_Hash.buckets_cnt = rawdata[0];
                p_Elf_Hash.chains_cnt = rawdata[1];
                p_Elf_Hash.buckets = &(rawdata[2]);
                p_Elf_Hash.chains = &(p_Elf_Hash.buckets[p_Elf_Hash.buckets_cnt]);
                break;
        }
    }
    return true;
}
uint32_t elf_hash(const uint8_t *name)
{
    uint32_t h = 0, g;

    while (*name)
    {
        h = (h << 4) + *name++;
        g = h & 0xf0000000;
        h ^= g;
        h ^= g >> 24;
    }
    return h;
}

uintptr_t GotHook::getFuncAddrInMem(const char* target_func)
{
    //查找符号 .hash -> .dynsym -> .dynstr
    Elf_Sym *target = nullptr;
    uintptr_t target_addr = 0;
    uint32_t hash = elf_hash((const uint8_t *) target_func);
    auto *dynsym = reinterpret_cast<Elf_Sym *>(lib_base + p_Elf_Rel_Dyn.sym_tab_offset);
    char *dynstr = reinterpret_cast<char *>(lib_base + p_Elf_Rel_Dyn.str_tab_offset);
    for (uint32_t i = p_Elf_Hash.buckets[hash % p_Elf_Hash.buckets_cnt]; 0 != i; i = p_Elf_Hash.chains[i])
    {
        Elf_Sym *tmp_sym = dynsym + i;
        unsigned char type = ELF_ST_TYPE(tmp_sym->st_info);
        if (STT_FUNC != type && STT_GNU_IFUNC != type && STT_NOTYPE != type)
        {
            continue;
        }
        LOGE("find target function ====> %s", dynstr + tmp_sym->st_name);

        if (0 == strcmp(reinterpret_cast<const char *>(dynstr + tmp_sym->st_name), target_func))
        {
            target = tmp_sym;
            break;
        }
    }

    //遍历 .rel.plt / .rela.plt 和 .rel.dyn / .rela.dyn，获取偏移，计算内存地址
    auto * rela_plt =  reinterpret_cast<Elf_Rela *>(lib_base + p_Elf_Rel_Dyn.rela_plt_offset);
    for (int i = 0; i < p_Elf_Rel_Dyn.rela_plt_size; i++)
    {
        if (&(dynsym[ELF_R_SYM(rela_plt[i].r_info)]) == target && ELF_R_TYPE(rela_plt[i].r_info) == ELF_R_JUMP_SLOT)
        {
            target_addr = *(uintptr_t *) (lib_base+rela_plt[i].r_offset);
        }
    }
    auto * rela_dyn =  reinterpret_cast<Elf_Rela *>(lib_base + p_Elf_Rel_Dyn.rela_dyn_offset);
    for (int i = 0; i < p_Elf_Rel_Dyn.rela_dyn_size; i++)
    {
        if (&(dynsym[ELF_R_SYM(rela_dyn[i].r_info)]) == target && (ELF_R_TYPE(rela_dyn[i].r_info) == ELF_R_ABS || ELF_R_TYPE(rela_dyn[i].r_info) == ELF_R_GLOB_DAT))
        {
            target_addr = *(uintptr_t *) (lib_base+rela_dyn[i].r_offset);
        }
    }

    return target_addr;
}

bool GotHook::checkGotHook(const char* lib_name, const char* func_name)
{
    char lib_path[256] = {0};
    lib_base =  getLibModuleAddr(lib_name, lib_path);
    if (lib_base == 0)
    {
        return false;
    }
    LOGI("Check CheckJavaHook lib:[%s]\n", lib_path);
    // 获取文件中目标函数的原始地址
    uintptr_t orgin_func_addr = getFuncOrginalAddress(lib_path, func_name);
    LOGE("%s origin address == %lu\n", func_name, orgin_func_addr);
    initELF(lib_path);
    // 获取内存中目标函数的地址 (解析Segment)
    uintptr_t target_addr = getFuncAddrInMem(func_name);
    LOGE("target function addr ====> %lu", target_addr);

    if (orgin_func_addr != target_addr)
    {
        return true;
    }
    return false;
}