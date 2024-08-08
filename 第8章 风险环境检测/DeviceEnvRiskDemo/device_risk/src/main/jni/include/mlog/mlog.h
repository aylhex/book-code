#ifndef _MLOG_H_
#define _MLOG_H_

// 引入log头文件
#include <android/log.h>

#define TAG "AYL"
 
#define LOGINFO   1       //日志开关，1为开，其它为关
#define LOGIDEBUG 1       //日志开关，1为开，其它为关
#define LOGIERROR 1       //日志开关，1为开，其它为关
 /**
  * release版本 需要关闭开关
  * 必须注释掉日志输出代码
  */
 
#if(LOGINFO==1)
    // 定义info信息
    //#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
    //#define LOGI(...) NULL
#else
    #define LOGI(...) NULL
#endif


#if(LOGIDEBUG==1)
    // 定义debug信息
    //#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
    #define LOGD(...) NULL
#else
    #define LOGD(...) NULL
#endif


#if(LOGIERROR==1)
    // 定义error信息
    //#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
    #define LOGE(...) NULL
#else
    #define LOGE(...) NULL
#endif


#endif
