#include <jni_main.h>

// 把C字符串转化为java字符串
jstring char_to_jstring(JNIEnv *env, const char *info)
{
    int length = strlen(info);
    jstring error = env->NewStringUTF("error");
    jclass class_obj = env->FindClass("java/lang/String");
    if (env->ExceptionCheck()) 
    {  // 检查JNI调用是否有引发异常
        //env->ExceptionDescribe(env);
        env->ExceptionClear();  // 清除引发的异常，在Java层不会打印异常的堆栈信息
        return error;
    }
    if (class_obj == NULL) 
    {
        return error;
    }  
    jmethodID methodid = env->GetMethodID(class_obj, "<init>", "([BLjava/lang/String;)V");
    if (methodid == NULL) 
    {
        return error;
    }  
    jbyteArray bytes = env->NewByteArray(length);
    if (bytes == NULL)
    {
        return error;
    }
    env->SetByteArrayRegion(bytes, 0, length, (jbyte *) info);

    jstring coding = env->NewStringUTF("utf-8");

    return (jstring) env->NewObject(class_obj, methodid, bytes, coding);;
}


extern "C" JNIEXPORT jstring JNICALL start(JNIEnv *env, jclass  clazz)
{
    return char_to_jstring(env, "Hello Ollvm!");
}

