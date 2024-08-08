//
//  Created by AYL on 19/8/23.
//

#include <onload.h>
#include <main.h>
#include <anticall.h>
#include <tools.h>
/**
 * 返回 0 正常
 * 将防止被恶意调用程序将退出
*/
int anti_call(JNIEnv *env)
{
    // 正式签名的m包名
    char* real_pkg = "com.test.demo";
    // 正式签名的sha1值
    char* real_sign = "xxxxxxxx";

    jobject context = get_global_context(env);
    if (context == NULL)
    {
        return 0;
    }

    char* pkg = get_package_name(env, context);

    if (pkg != NULL)
    {
        //LOGI("package=%s", pkg);
        if(strncasecmp(pkg, real_pkg, strlen(real_pkg)) != 0)
        {
            exit(0);
        }
    }

    char* sign = get_signature(env, context);
    if (sign != NULL)
    {
        //LOGI("sign=%s", sign);
        if(strncasecmp(sign, real_sign, strlen(real_sign)) != 0)
        {
            exit(0);
        }
    }
    return 0;
}

// Java和JNI函数的绑定表
 __attribute ((visibility("hidden")))
 static JNINativeMethod method_table[] = {
    {NATIVE_AES_ENCRYPT , METHOD_AES_ENCRYPT_SIGN, (void*)EXPORT_AES_ENCRYPT },//绑定
    {NATIVE_AES_DECRYPT , METHOD_AES_DECRYPT_SIGN, (void*)EXPORT_AES_DECRYPT },//绑定
    {NATIVE_CREATE_KEY , METHOD_CREATE_KEY_SIGN, (void*)EXPORT_CREATE_KEY },//绑定
    {NATIVE_CALC_SECRET , METHOD_CALC_SECRET_SIGN, (void*)EXPORT_CALC_SECRET },//绑定
};

// 注册native方法到java中
static int registerNativeMethods(JNIEnv* env, const char* className,
        JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = (*env)->FindClass(env, className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

int register_ndk_load(JNIEnv *env)
{
    // 调用注册方法
    return registerNativeMethods(env, JNIREG_CLASS,method_table, sizeof(method_table)/sizeof(method_table[0]));
}

// UPX加壳使用
void _init(void){}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;
    if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }
    // 防调用
    anti_call(env);
    register_ndk_load(env);
    // 返回jni的版本
    return JNI_VERSION_1_4;
}
