//
//  Created by AYL on 19/8/23.
//

#ifndef _ANTICALL_H
#define _ANTICALL_H
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <openssl/md5.h>
/*
返回0 说明程序出错
返回1 说明获取签名正常
 */
char* get_signature(JNIEnv *env, jobject context);

// 获取应用的包名
char* get_package_name(JNIEnv *env, jobject context);


/**
 * 获取全局的context
 */
jobject get_global_context(JNIEnv *env);


#endif //_ANTICALL_H
