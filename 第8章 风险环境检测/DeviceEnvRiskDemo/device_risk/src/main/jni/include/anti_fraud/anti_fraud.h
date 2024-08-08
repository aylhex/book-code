#ifndef _ANTI_FRAUD_HEAD
#define _ANTI_FRAUD_HEAD
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <mjson.h>
#include <mjson.h>

namespace devicerisk
{
class AntiFraud
{
public:
    // 获取APP module中的风险信息
    static void GetModuleFraudInfo(char *package_name, char **hookframwork, char **injectso, char **injectdex);

    // 获取app签名
    static char *GetSignature(JNIEnv *env, jobject mContext);

    // 检测SELinux状态  然后返回
    static char *GetSelinuxState();

    // 获取应用的包名
    static char *GetPackageName(JNIEnv *env, jobject mContext);

    /*
     * 用于检测magisk框架
    */
    static char *GetMagiskInfo();
};
}
#endif
