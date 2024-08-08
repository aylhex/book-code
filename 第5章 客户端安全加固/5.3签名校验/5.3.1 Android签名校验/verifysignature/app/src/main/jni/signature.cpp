//
// Created by Ann on 2022/5/6.
//

#include <jni.h>
#include <string.h>
#include <android/log.h>
#include <malloc.h>

#define  TAG    "Test"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

const char *APP_SIGNATURE = "10645EA8A12BE7A2C04B1F81DF3B4D90";

void ByteToHexStr(const char *source, char *dest, int sourceLen)
{
    short i;
    char highByte, lowByte;

    for (i = 0; i < sourceLen; i++)
    {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f;
        highByte += 0x30;

        if (highByte > 0x39)
        {
            dest[i * 2] = highByte + 0x07;
        }
        else
        {
            dest[i * 2] = highByte;
        }

        lowByte += 0x30;
        if (lowByte > 0x39)
        {
            dest[i * 2 + 1] = lowByte + 0x07;
        }
        else
        {
            dest[i * 2 + 1] = lowByte;
        }
    }
}

jstring sha1(JNIEnv *env, jbyteArray source)
{
    // MessageDigest类
    jclass classMessageDigest = env->FindClass("java/security/MessageDigest");
    // MessageDigest.getInstance()静态方法
    jmethodID midGetInstance = env->GetStaticMethodID(classMessageDigest, "getInstance","(Ljava/lang/String;)Ljava/security/MessageDigest;");
    // MessageDigest object
    jobject objMessageDigest = env->CallStaticObjectMethod(classMessageDigest,midGetInstance,env->NewStringUTF("sha1"));

    // update方法，这个函数的返回值是void，写V
    jmethodID midUpdate = env->GetMethodID(classMessageDigest, "update", "([B)V");
    env->CallVoidMethod(objMessageDigest, midUpdate, source);

    // digest方法
    jmethodID midDigest = env->GetMethodID(classMessageDigest, "digest", "()[B");
    jbyteArray objArraySign = (jbyteArray) env->CallObjectMethod(objMessageDigest, midDigest);

    jsize intArrayLength = env->GetArrayLength(objArraySign);
    jbyte *byte_array_elements = env->GetByteArrayElements(objArraySign, NULL);
    size_t length = (size_t) intArrayLength * 2 + 1;
    char *char_result = (char *) malloc(length);
    memset(char_result, 0, length);

    // 将byte数组转换成16进制字符串，发现这里不用强转，jbyte和unsigned char应该字节数是一样的
    ByteToHexStr((const char *) byte_array_elements, char_result, intArrayLength);
    // 在末尾补\0
    *(char_result + intArrayLength * 2) = '\0';

    jstring stringResult = env->NewStringUTF(char_result);
    // release
    env->ReleaseByteArrayElements(objArraySign, byte_array_elements, JNI_ABORT);
    // 释放指针使用free
    free(char_result);
    env->DeleteLocalRef(classMessageDigest);
    env->DeleteLocalRef(objMessageDigest);

    return stringResult;
}

static jobject getContext(JNIEnv *env)
{
    jobject application = NULL;
    jclass activity_thread_clz = env->FindClass("android/app/ActivityThread");
    if (activity_thread_clz != NULL)
    {
        jmethodID currentApplication = env->GetStaticMethodID(activity_thread_clz, "currentApplication", "()Landroid/app/Application;");
        if (currentApplication != NULL)
        {
           application = env->CallStaticObjectMethod(activity_thread_clz, currentApplication);
        }
        env->DeleteLocalRef(activity_thread_clz);
    }

    return application;
}

JNIEXPORT jstring JNICALL getSignByJni(JNIEnv *env, jobject thiz, jobject context)
{
    // 获得Context类
    jclass cls = env->GetObjectClass(context);
    // 得到getPackageManager方法的ID
    jmethodID mid = env->GetMethodID(cls, "getPackageManager","()Landroid/content/pm/PackageManager;");

    // 获得应用包的管理器
    jobject pm = env->CallObjectMethod(context, mid);

    // 得到getPackageName方法的ID
    mid = env->GetMethodID(cls, "getPackageName", "()Ljava/lang/String;");
    // 获得当前应用包名
    jstring packageName = (jstring) env->CallObjectMethod(context, mid);

    // 获得PackageManager类
    cls = env->GetObjectClass(pm);
    // 得到getPackageInfo方法的ID
    mid = env->GetMethodID(cls, "getPackageInfo","(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    // 获得应用包的信息
    jobject packageInfo = env->CallObjectMethod(pm, mid, packageName, 0x40); //GET_SIGNATURES = 64;
    // 获得PackageInfo 类
    cls = env->GetObjectClass(packageInfo);
    // 获得签名数组属性的ID
    jfieldID fid = env->GetFieldID(cls, "signatures", "[Landroid/content/pm/Signature;");
    // 得到签名数组
    jobjectArray signatures = (jobjectArray) env->GetObjectField(packageInfo, fid);
    // 得到签名
    jobject signature = env->GetObjectArrayElement(signatures, 0);

    // 获得Signature类
    cls = env->GetObjectClass(signature);
    // 得到toCharsString方法的ID
    mid = env->GetMethodID(cls, "toByteArray", "()[B");
    // 返回当前应用签名信息
    jbyteArray signatureByteArray = (jbyteArray) env->CallObjectMethod(signature, mid);

    return sha1(env, signatureByteArray);
}


/*
jboolean checkSignature(JNIEnv *env, jobject thiz, jobject context)
{
    jstring appSignature = Java_com_verify_signature_Tools_getSignByJni(env, thiz, context); // 当前 App 的签名

    jstring releaseSignature = env->NewStringUTF(APP_SIGNATURE); // 发布时候的签名
    const char *charAppSignature = env->GetStringUTFChars(appSignature, NULL);
    const char *charReleaseSignature = env->GetStringUTFChars(releaseSignature, NULL);

    LOGI(TAG, charAppSignature);

    jboolean result = JNI_FALSE;
    // 比较是否相等
    if (charAppSignature != NULL && charReleaseSignature != NULL)
    {
        if (strcmp(charAppSignature, charReleaseSignature) == 0)
        {
            result = JNI_TRUE;
        }
    }

    env->ReleaseStringUTFChars(appSignature, charAppSignature);
    env->ReleaseStringUTFChars(releaseSignature, charReleaseSignature);
    return result;
}
*/

/**
 * 检查加载该so的应用的签名，与预置的签名是否一致
 */
/*
static jboolean checkSignature(JNIEnv *env)
{
    // 调用 getContext 方法得到 Context 对象
    jobject appContext = getContext(env);
    if (appContext != NULL)
    {
        jboolean signatureValid = checkSignature(env, appContext);
        return signatureValid;
    }

    return JNI_FALSE;
}
*/
/**
 * 加载 so 文件的时候，会触发 OnLoad
 */

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return JNI_ERR;
    }

    jclass c = env->FindClass("com/verify/signature/Tools");
    if (c == nullptr) return JNI_ERR;

    static const JNINativeMethod methods[] = {
        {"getSignByJni", "(Landroid/content/Context;)Ljava/lang/String;", reinterpret_cast<void*>(getSignByJni)},
    };
    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}
