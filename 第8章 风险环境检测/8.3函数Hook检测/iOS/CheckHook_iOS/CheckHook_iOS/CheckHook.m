//
//  CheckHook.m
//  CheckHook_iOS
//
//  Created by AYL on 2023/2/6.
//

#import "CheckHook.h"


#include <mach-o/dyld_images.h>
#include <mach/mach_init.h>
#include <mach/mach_port.h>
#include <mach/task_info.h>
#include <mach/task.h>

@implementation CheckHook


//
//typedef struct HookNode {
//    void* beg;
//    void* end;
//    int index;
//    struct HookNode *next; //+32
//} HookNode;
//
//
//static struct HookNode *rootNode = NULL;
//
//
//
///*
// 检测传入的函数地址是不是位于(_TEXT,__text)之中，
// 如果在说明是自己实现的伪函数，则位于(_TEXT,__text)之中
// 如果是系统的原函数，则位于 (__DATAl,a_symbol_ptr)里的
// */
//bool checkFishHook(void *method_name)
//{
//    BOOL inTEXT = FALSE;
//    struct HookNode *node;
//    for ( node = rootNode; node; node = node->next ) {
//
//        if (method_name > node->beg && method_name < node->end) {
//            inTEXT = TRUE;
//            break;
//        }
//
//    }
//    return true;
//}

//void* get_function_address(const char* symbol_name) {
//    Dl_info info;
//    if (dladdr((const void*)symbol_name, &info) == 0) {
//        return NULL;
//    }
//    return info.dli_saddr;
//}
//
//void* get_module_base_address(const char* module_path) {
//    Dl_info info;
//    if (dladdr((const void*)get_module_base_address, &info) == 0) {
//        return NULL;
//    }
//    return info.dli_fbase;
//}
//
//
//bool checkFishHook(char *functionName)
//{
//    const char* symbol_name = functionName;
//    void* function_address = get_function_address(symbol_name);
//    void* module_base_address = get_module_base_address(NULL);
//
//    if (function_address != NULL && module_base_address != NULL) {
//        if (function_address < module_base_address) {
//            NSLog(@"Function is in __TEXT segment.");
//        } else {
//            NSLog(@"Function is in __DATA segment.");
//        }
//    }
//
//    return false;
//
//}

// 检查一个Mach-O文件中，是否存在被 Fishhook Hook 的函数
//bool checkFishHook(char *functionName)
//{
//    NSLog(@"Start checkFishHook!");
//    void *functionPtr = dlsym(RTLD_DEFAULT, functionName);
//    Dl_info info;
//    if (dladdr(functionPtr, &info) != 0)
//    {
//        struct mach_header *executableHeader = (struct mach_header *) info.dli_fbase;
//        struct segment_command_64 *textSegment = NULL;
//        struct segment_command_64 *dataSegment = NULL;
//        struct load_command *command = (struct load_command *) (executableHeader + 1);
//        NSLog(@"executableHeader===>%d", executableHeader->ncmds);
//        for (uint32_t i = 0; i < executableHeader->ncmds; i++,command = (struct load_command *) ((char *) command + command->cmdsize))
//        {
//            NSLog(@"command->cmd===>%lx", command->cmd);
//            if (command->cmd == LC_SEGMENT_64 || command->cmd == LC_SEGMENT)
//            {
//                struct segment_command_64 *segment = (struct segment_command_64 *) command;
//                NSLog(@"segname====> %s",segment->segname);
//                if (strcmp(segment->segname, "__TEXT") == 0)
//                {
//                    textSegment = segment;
//                }
//                else if (strcmp(segment->segname, "__DATA") == 0)
//                {
//                    dataSegment = segment;
//                }
//            }
//            else if (command->cmd == LC_SEGMENT_SPLIT_INFO)
//            {
//                struct linkedit_data_command *segmentInfo = (struct linkedit_data_command *) command;
//                if (segmentInfo->dataoff > 0 && textSegment != NULL && dataSegment != NULL)
//                {
//                    uintptr_t slide = (uintptr_t) executableHeader - (uintptr_t) textSegment->vmaddr;
//                    uintptr_t *indirectSymbolPointers = (uintptr_t *)((uintptr_t)textSegment->fileoff + slide + segmentInfo->dataoff);
//                    uint32_t pointerCount = segmentInfo->datasize / sizeof(uintptr_t);
//                    for (uint32_t j = 0; j < pointerCount; j++)
//                    {
//                        uintptr_t *indirectSymbolPointer = indirectSymbolPointers + j;
//                        if (*indirectSymbolPointer == (uintptr_t) functionPtr)
//                        {
//                            // 如果函数指针地址在 Mach-O 文件的 __TEXT 区域，则表示该函数已经被 Fishhook Hook
//                            if ((uintptr_t) indirectSymbolPointer >= textSegment->fileoff &&
//                                (uintptr_t) indirectSymbolPointer < textSegment->fileoff + textSegment->filesize)
//                            {
//                                return true;
//                            }
//                        }
//                    }
//                }
//            }
//
//        }
//    }
//    return false;
//}



void* get_function_address(const char* symbol_name) {
    Dl_info info;
    if (dladdr((const void*)symbol_name, &info) == 0) {
        return NULL;
    }
    return info.dli_saddr;
}

const char* get_library_path(const void* address) {
    Dl_info info;
    if (dladdr(address, &info) == 0) {
        return NULL;
    }
    return info.dli_fname;
}


bool checkMSHook(void* method_name)
{
    /*
     armv7跳转指令
     df f8 00 f0 21 a3 59 00  c0 46 d0 f8 00 c0 bc f1
     arm64跳转指令
     50 00 00 58 00 02 1f d6  40 00 f0 7f 01 00 00 00
    */

    const unsigned char ins_armv7[] = {0xdf, 0xf8, 0x00, 0xf0, 0x21, 0xa3, 0x59, 0x00, 0xc0, 0x46, 0xd0, 0xf8, 0x00, 0xc0, 0xbc, 0xf1};
    const unsigned char ins_arm64[] = {0x50, 0x00, 0x00, 0x58, 0x00, 0x02, 0x1f, 0xd6, 0x40, 0x00, 0xf0, 0x7f, 0x01, 0x00, 0x00, 0x00};
  
    unsigned char *start_ins = (unsigned char*) method_name;
    //地址获取成功则调用
    if (start_ins) {
        //arm64
        if (memcmp(start_ins, ins_arm64, sizeof(ins_arm64)) == 0) {
            return true;
        }
        //armv7
        if (memcmp(start_ins, ins_armv7, sizeof(ins_armv7)) == 0) {
            return true;
        }
    }
    return false;
}

/**
 检查原始函数指针是否被修改：Fishhook使用rebind_symbols函数来替换原始函数的实现。因此，如果一个函数被Fishhook，那么它的原始函数指针应该已经被修改为指向Fishhook的实现，而不是原始的实现。可以使用dlsym函数获取原始函数指针，然后检查它是否与原始实现的地址相同。
 */
bool checkHook(const char* method_name)
{
    // 获取原始函数指针
    void (*original_function)(void) = dlsym(RTLD_DEFAULT, method_name);

    // 获取原始实现的地址
    void *original_implementation = &open;
//    if (original_function != NULL) {
//        Dl_info info;
//        if (dladdr(original_function, &info) != 0) {
//            original_implementation = info.dli_saddr;
//        }
//    }
    NSLog(@"original_function===> %lx", original_function);
    NSLog(@"original_implementation====>%lx", original_implementation);

    // 判断原始函数指针是否被修改
    if (original_function != original_implementation) {
        // 函数被hook
        return true;
    } else {
        // 函数未被hook
        return false;
    }
    
    return false;
}

bool checkHook()
{
    //  获取程序中真实使用的函数地址
    void *original_implementation = (void *)&open;
    // 获取函数在库中的原始实现的地址
    void *lib_function_address = dlsym(RTLD_DEFAULT, "open");

    // 判断原始函数指针是否被修改
    if (lib_function_address != original_implementation)
    {
        // 函数被hook
        return true;
    } else {
        // 函数未被hook
        return false;
    }
    return false;
}




bool checkSwizzleHook(const char* class_name,const char* method_name)
{
    Dl_info dl_info;
    Class cls = objc_getClass(class_name);
    SEL sel = sel_registerName(method_name);
    Method method = class_getInstanceMethod(cls, sel);
    if(!method)
    {
        method = class_getClassMethod(cls, sel);
    }
    IMP imp = method_getImplementation(method);
    // 通过dladdr获取当前函数所在库路径。
    if(0==dladdr((void*)imp, &dl_info))
    {
        return false;
    }
    NSLog(@"%s",dl_info.dli_fname);
    char* white_lib_path="/private/var/containers/Bundle/Application";
    if(!strncmp(dl_info.dli_fname, white_lib_path, strlen(white_lib_path)))
    {
        return false;
    }
    // _dyld_get_image_name(0) 获取应用可执行文件的路径
    if(!strcmp(dl_info.dli_fname, _dyld_get_image_name(0)))
    {
        return false;
    }
    return true;
}
@end
