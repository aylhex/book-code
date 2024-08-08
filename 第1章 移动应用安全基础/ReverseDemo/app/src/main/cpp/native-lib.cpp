#include <jni.h>
#include <string>

// 指定要注册的类，对应完整的java类名
#define JNIREG_CLASS "com/test/demo/JniWrapper"
// 对应的java层函数名字
#define NATIVE_JAVA_FUNC  "stringFromJNI2"
// 导出函数名进行宏定义
#define EXPORT_JNI_NAME  helloJni
// 导出函数的签名
#define METHOD_SIGN   "()Ljava/lang/String;"
// 导出函数
extern "C" jstring EXPORT_JNI_NAME(JNIEnv *env, jclass clazz);

// Java和JNI函数的绑定表
static JNINativeMethod method_table[] = {
        {NATIVE_JAVA_FUNC , METHOD_SIGN, (void*)EXPORT_JNI_NAME },//绑定
};

// 注册native方法到java中
static int registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == nullptr) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

int register_ndk_load(JNIEnv *env)
{
    // 调用注册方法
    return registerNativeMethods(env, JNIREG_CLASS,method_table, 1);
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = nullptr;
    jint result = -1;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
    {
        return result;
    }

    register_ndk_load(env);
    // 返回jni的版本
    return JNI_VERSION_1_4;
}


extern "C" JNIEXPORT jstring JNICALL EXPORT_JNI_NAME(JNIEnv* env,jclass /* this */)
{
    std::string hello = "Hello from JNI2";
    return env->NewStringUTF(hello.c_str());
}
extern "C" JNIEXPORT jstring JNICALL
Java_com_test_demo_JniWrapper_stringFromJNI(
        JNIEnv* env,
        jclass /* this */) {
    std::string hello = "Hello from JNI";
    return env->NewStringUTF(hello.c_str());
}
