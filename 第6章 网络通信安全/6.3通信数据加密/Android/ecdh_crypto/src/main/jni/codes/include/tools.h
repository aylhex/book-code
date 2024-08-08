//
//  Created by AYL on 19/8/23.
//

#ifndef _TOOLS_
#define _TOOLS_

#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>

#define TAG "crypto_jni"

#define LOGINFO   0       //日志开关，1为开，其它为关
#define LOGIDEBUG 0       //日志开关，1为开，其它为关
#define LOGIERROR 0       //日志开关，1为开，其它为关

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
void mm_safe_free(void **pp);

#endif