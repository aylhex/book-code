// #import "MSHook.h" 
#pragma clang module import Darwin.POSIX.dlfcn  
// #include <objc/runtime.h> 
// #import <UIKit/UIKit.h> 
#include <substrate.h>
#include <malloc/malloc.h>
#include <stdio.h>

// @implementation MSHook 

static void *(*original_malloc)(size_t);

void *hooked_malloc(size_t size) {
    printf("Allocating %ld bytes of memory\n", size);
    void *ptr = original_malloc(size);
    printf("Allocated memory at %p\n", ptr);
    return ptr;
}

__attribute__((constructor))
static void initialize() 
{
    original_malloc = dlsym(RTLD_NEXT, "malloc");
    MSHookFunction(original_malloc, hooked_malloc, (void **)&original_malloc);
    // 原方法 
//     int *p = malloc(sizeof(int));
//     *p = 42;
//     printf("The value of p is %d\n", *p);
//     free(p);
//     printf("This func has been hooked by MS!!!");
}


// + (void)load 
// {

// } 
// @end








