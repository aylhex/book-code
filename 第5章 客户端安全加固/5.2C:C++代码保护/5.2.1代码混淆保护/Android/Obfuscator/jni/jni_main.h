//
// Created by Ayl on 12/26/22.
//

#ifndef JNI_MAIN_H
#define JNI_MAIN_H

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

extern "C" JNIEXPORT jstring JNICALL start(JNIEnv *env, jclass  clazz);

#endif //JNI_MAIN_H
