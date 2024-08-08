#ifndef _COMMON_H_
#define _COMMON_H_

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/file.h>
#include <android/log.h>

#define TAG "CHECK-HOOK"

#define MALLOC(num,type) (type*)malloc(num*sizeof(type))
#define CALLOC(num,type) (type*)calloc(num, sizeof(type))
 
#define LOGINFO   1       //日志开关，1为开，其它为关
#define LOGIDEBUG 1       //日志开关，1为开，其它为关
#define LOGIERROR 1       //日志开关，1为开，其它为关
 /**
  * release版本 需要关闭开关
  * 必须注释掉日志输出代码
  */
 
#if(LOGINFO==1)
    // 定义info信息
    #define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#else
    #define LOGI(...) NULL
#endif


#if(LOGIDEBUG==1)
    // 定义debug信息
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#else
    #define LOGD(...) NULL
#endif


#if(LOGIERROR==1)
    // 定义error信息
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#else
    #define LOGE(...) NULL
#endif


#define safe_free(p) mm_safe_free((void **)&(p))

#ifdef __cplusplus
extern "C" {
#endif

void mSafeFree(void **pp);

#ifdef __cplusplus
}
#endif

#endif
