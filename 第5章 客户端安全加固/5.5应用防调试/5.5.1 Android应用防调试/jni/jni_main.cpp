//
// Created by Ayl on 12/26/22.
//

#include <jni_main.h>
#include "anti_debug.h"

extern "C" JNIEXPORT void JNICALL antidebug(JNIEnv *env, jclass  clazz)
{
    check_debug();
}

