#include <jni.h>
#include <string>
#include <cstddef>
#include <sys/mman.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include "comm_elf.h"
#include "common.h"
#include "xhook.h"
#include "test.h"
#include "check_got_hook.h"
#include "check_inline_hook.h"


void* (*oldmalloc)(size_t __byte_count);

extern "C" JNIEXPORT jstring JNICALL Java_com_check_hook_MainActivity_checkGotHook(
        JNIEnv* env,
        jobject /* this */) {
    // 开启GOT CheckJavaHook
    xhook_register(".*/libnative-lib\\.so$", "malloc", (void *)(my_malloc), (void**)(&oldmalloc));
    xhook_refresh(1);

    say_hello();


    const char* target_func = "malloc";
    char hook[56] = "\n";
    char safe[56] = "\n";
    sprintf(hook, "%s was be got hooked!",target_func);
    sprintf(safe, "%s is safe!",target_func);

    //checkGotHook("libc.so", "malloc");

    GotHook mGotHook;
    bool result = mGotHook.checkGotHook("libnative-lib.so", target_func);

    if (result)
    {
        LOGE("%s was be got hooked!\n", target_func);
        return env->NewStringUTF(hook);
    }
    return env->NewStringUTF(safe);
}

extern "C" JNIEXPORT jstring JNICALL Java_com_check_hook_MainActivity_checkInlineHook(
        JNIEnv* env,
        jobject /* this */) {

    const char* target_func = "malloc";
    char hook[56] = "\n";
    char safe[56] = "\n";
    sprintf(hook, "%s was be inline hooked!",target_func);
    sprintf(safe, "%s is safe!",target_func);

    InlineHook mInlineHook;
    bool result = mInlineHook.checkInlineHook(target_func, 1);
    if (result)
    {
        LOGE("%s was be inline hooked!\n", target_func);
        return env->NewStringUTF(hook);
    }

    LOGE("%s is safe!\n", target_func);

    return env->NewStringUTF(safe);
}