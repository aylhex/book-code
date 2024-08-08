
#include <jni_main.h>
// 指定要注册的类，对应完整的java类名
#define JNIREG_CLASS "com/test/obfuscator/Coded"

#define NATIVE_MTEHOD  "test"
#define METHOD_SIGN "()Ljava/lang/String;"

// Java和JNI函数的绑定表
static JNINativeMethod method_table[] = {
        {NATIVE_MTEHOD , METHOD_SIGN, reinterpret_cast<void*>(start) },
};
// 注册native方法到java中
static int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr)
    {
        return JNI_ERR;
    }

    int rc = env->RegisterNatives(clazz, gMethods, numMethods);
    if ( rc != JNI_OK)
    {
        return rc;
    }

    return JNI_TRUE;
}

int register_ndk_load(JNIEnv *env)
{
    // 调用注册方法
    return registerNativeMethods(env, JNIREG_CLASS, method_table, sizeof(method_table)/sizeof(JNINativeMethod));
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return JNI_ERR;
    }
    register_ndk_load(env);
    // 返回jni的版本
    return JNI_VERSION_1_6;
}
