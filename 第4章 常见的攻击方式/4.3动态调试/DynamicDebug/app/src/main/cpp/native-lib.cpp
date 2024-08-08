#include <jni.h>
#include <string>
#include <android/log.h>

#include <stdio.h>

#define TAG "DynamicDebug"
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

/**
 * 将一个jstring转换成一个c语言的char* 类型.
 */
char* JString2CStr(JNIEnv* env, jstring jstr) {
    char* rtn = NULL;
    jclass clsstring = env->FindClass( "java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID( clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray)env->CallObjectMethod( jstr, mid, strencode); // String .getByte("GB2312");
    jsize alen = env->GetArrayLength( barr);
    jbyte* ba = env->GetByteArrayElements( barr, JNI_FALSE);
    if(alen > 0) {
        rtn = (char*)malloc(alen+1); //"\0"
        memcpy(rtn, ba, alen);
        rtn[alen]=0;
    }
    env->ReleaseByteArrayElements( barr, ba,0);
    return rtn;
}


__attribute__ ((visibility ("hidden"))) int add(int a, int b){
    return a+b;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_dynamic_debug_MainActivity_checkInput(
        JNIEnv* env,
        jobject /* this */,jstring data) {

    std::string hello = "test";

    char* input = JString2CStr(env, data);

    LOGE("Input =>  %s", input);
    int sum = add(3,4);
    LOGE("Sum ==> %d", sum);

    jstring ttt = env->NewStringUTF(hello.c_str());
    if (strcmp(hello.c_str(),input) == 0){
        return true;
    }

    return false;
}