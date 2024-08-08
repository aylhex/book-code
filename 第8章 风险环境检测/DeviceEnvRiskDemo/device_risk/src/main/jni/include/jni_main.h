#ifndef JNI_MAIN_H
#define JNI_MAIN_H

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <dispatch.h>
#include <string>
#include <iostream>

namespace devicerisk
{
    jbyteArray JNICALL do_command(JNIEnv *env, jclass  clazz, jbyteArray _origin_params);
}

#endif //JNI_MAIN_H
