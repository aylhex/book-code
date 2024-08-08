#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <elf.h>

#define IMAGE_BASE 0x80000000

//获取phdr结构
Elf32_Phdr *get_Phdr(const char *filename, u_int32_t *out_phnumber, FILE **pFile)
{
	void *imageaddr = 0;
	FILE *fp = NULL;
	fp = fopen(filename, "rb");
	//解析ELF头  以及ELF 程序头来读取内存，当然读取0X1000个字节可以。为了学习这里解析

	Elf32_Ehdr header = {0}; //头
	if (NULL != fp)
	{

		fread(&header, sizeof(Elf32_Ehdr), 1, fp);
		
		printf("开始解析文件e_phoff\n");
		printf("phsize = %x \n",header.e_phoff);
		printf("ph每一项大小为： %x \n",header.e_phentsize);
		printf("pheadr有 %x 项 \n",header.e_phnum);
		

		u_int32_t phheader_phoff = header.e_phoff;
		u_int32_t phheader_size = header.e_phentsize * header.e_phnum;
		Elf32_Phdr *phheaderAry = NULL;
		phheaderAry = (Elf32_Phdr *)malloc(phheader_size);
		if (phheaderAry == NULL)
		{
			fclose(fp);
			fp = NULL;
			return NULL;
		}
		fseek(fp, phheader_phoff, SEEK_SET);
		fread(phheaderAry, phheader_size, 1, fp);
		*out_phnumber = header.e_phnum;
		*pFile = fp;
		return phheaderAry;
	}
	return NULL;
}

//获取shdr结构
Elf32_Shdr *get_shdr(const char *filename, u_int32_t *out_shnumber, FILE **pFile)
{
	void *imageaddr = 0;
	FILE *fp = NULL;
	fp = fopen(filename, "rb");
	//解析ELF头  以及ELF 程序头来读取内存，当然读取0X1000个字节可以。为了学习这里解析

	Elf32_Ehdr header = {0}; //头
	if (NULL != fp)
	{

		fread(&header, sizeof(Elf32_Ehdr), 1, fp);
		
		printf("开始解析文件e_shoff\n");
		printf("sh偏移 = %x \n",header.e_shoff);
		printf("sh每一项大小为： %x \n",header.e_shentsize);
		printf("sheadr有 %x 项 \n",header.e_shnum);
		

		u_int32_t shheader_shoff = header.e_shoff;
		u_int32_t shheader_size = header.e_shentsize * header.e_shnum;
		Elf32_Shdr *shheaderAry = NULL;
		shheaderAry = (Elf32_Shdr *)malloc(shheader_size);
		if (shheaderAry == NULL)
		{
			fclose(fp);
			fp = NULL;
			return NULL;
		}
		fseek(fp, shheader_shoff, SEEK_SET);
		fread(shheaderAry, shheader_size, 1, fp);
		*out_shnumber = header.e_shnum;
		*pFile = fp;
		//return shheader_shoff;
	}
	return NULL;
}

//映射所有的PT_LOAD可加载段，所以便利PHDR进行映射
bool mapViewPtload()
{
	Elf32_Phdr *phdrbase = NULL;
	u_int32_t phdrnumber = 0;
	FILE *pFile = NULL;
	phdrbase = get_Phdr("/system/lib/libc.so", &phdrnumber, &pFile);
	printf("phdrbase = 0x%0x \n", phdrbase);
	printf("phdrnumbr = %d \n", phdrnumber);

	printf("start map view pt_load ......\n");
	/*
		1.遍历所有phdrnumber 找寻PT_LOAD类型的段
		2.开始映射 映射到addr中 
	*/
	Elf32_Phdr *phdrTmp = phdrbase;
	for (u_int32_t i = 0; i < phdrnumber; i++, phdrTmp++)
	{
		if (phdrTmp->p_type == PT_LOAD)
		{

			/*
			 1.映射文件中记录的PT段的数据映射到内存中
			 2.如果文件大小和内存大小在对齐后 文件大小-内存大小 大于0的情况
			 那么还需要进行内存映射  
			*/
			//对齐记录的内存中的偏移
			u_int32_t map_addr = phdrTmp->p_vaddr & 0xfffff000;
			//映射文件中p_offset所在的数据，映射p_filesz大小 到 mapaddr
			void *mapViewAddr = mmap(
				(void *)(IMAGE_BASE + map_addr),
				phdrTmp->p_filesz,
				PROT_EXEC | PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_FIXED,
				fileno(pFile),
				phdrTmp->p_offset & 0xfffff000);
			if (mapViewAddr == MAP_FAILED)
			{
				printf("映射失败 \n");
				return false;
			}
			//判断是否需要进行对齐
			uint32_t mem_size = (phdrTmp->p_memsz + 0xfff + (phdrTmp->p_vaddr & 0xfff)) & 0xfffff000;
			uint32_t file_size = ((phdrTmp->p_filesz + 0xfff) + (phdrTmp->p_vaddr & 0xfff)) & 0xfffff000;
			if (mem_size - file_size > 0)
			{
				//继续映射，但是映射不属于文件了
				mmap(
					(void *)(IMAGE_BASE + map_addr + file_size),
					mem_size - file_size,
					PROT_EXEC | PROT_READ | PROT_WRITE,
					MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS,
					fileno(pFile),
					phdrTmp->p_offset & 0xfffff000);
			}
			printf("mapview addr = %x \n", mapViewAddr);
		}
	}
	return true;
}

int main(int argc, char **argv)
{
	Elf32_Shdr *shdrbase = NULL;
	u_int32_t shdrnumber = 0;
	FILE *pFile = NULL;
	shdrbase = get_shdr("/system/lib/libc.so", &shdrnumber, &pFile);
	printf("shdrbase = 0x%0x \n", shdrbase);
	printf("shdrnumbr = %d \n", shdrnumber);
	mapViewPtload();
	getchar();
	return 0;
}

