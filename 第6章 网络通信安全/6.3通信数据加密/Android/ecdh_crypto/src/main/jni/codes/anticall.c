
//
//  Created by AYL on 19/8/23.
//

#include <anticall.h>
#include <tools.h>

//  SO文件防调用
// 去除字符串两端空格及回车
void trim_string(char *line)
{
  char *start = NULL;
  char *end = NULL;

  int len = strlen(line);

  // 去掉最后的换行符
  if (line[len - 1] == '\n' || line[len - 1] == '\r')
  {
    len--;         // 字符串长度减一
    line[len] = 0; // 给字符串最后一个字符赋0，即结束符
  }

  // 去掉两端的空格
  start = line;         // 指向首字符
  end = line + len - 1; // 指向最后一个字符
  while (*start && isspace(*start))
  {
    start++; // 如果是空格，首地址往前移一位，如果不是，则跳过该循环
  }
  while (*end && isspace(*end))
  {
    *end-- = 0; // 如果是空格，末地址往前移一位，并赋结束符
  }
  // 把首地址还给line
  strcpy(line, start);
}



/**
 * 获取全局的context
 */
jobject get_global_context(JNIEnv *env)
{
    //获取Activity Thread的实例对象
    jclass activity_thread = (*env)->FindClass(env, "android/app/ActivityThread");
    if (activity_thread == NULL)
    {
        goto error;
    }
    jmethodID mid_current_activity_thread = (*env)->GetStaticMethodID(env, activity_thread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    if (activity_thread == NULL)
    {
        goto error;
    }
    jobject obj = (*env)->CallStaticObjectMethod(env, activity_thread, mid_current_activity_thread);
    if (activity_thread == NULL)
    {
        goto error;
    }
    //获取Application，也就是全局的Context
    jmethodID mid_application = (*env)->GetMethodID(env, activity_thread, "getApplication", "()Landroid/app/Application;");
    if (activity_thread == NULL)
    {
        goto error;
    }
    jobject context = (*env)->CallObjectMethod(env, obj, mid_application);
    if (activity_thread == NULL)
    {
        goto error;
    }
    return context;
    //错误处理
error:
    return NULL;
}


#define HEX_VALUES "0123456789ABCDEF"
/*
返回0 说明程序出错
返回1 说明获取签名正常
 */
char *char_to_hex(unsigned char count, char *hex_value)
{
    if (count < 16) {
        hex_value[0] = HEX_VALUES[0];
        hex_value[1] = HEX_VALUES[count];
    } else {
        int l = count / 16;
        int r = count % 16;
        hex_value[0] = HEX_VALUES[l];
        hex_value[1] = HEX_VALUES[r];
    }
    hex_value[2] = 0;
    return hex_value;
}


/*
返回NULL   说明程序出错
返回非NULL 说明获取签名正常
 */
char* get_signature(JNIEnv *env, jobject context)
{
    if (context == NULL)
    {
        goto error;
    }
    // 获得Context类
    jclass cls = (*env)->GetObjectClass(env, context);
    if (cls == NULL)
    {
        goto error;
    }
    // 得到getPackageManager方法的ID
    char* package_manager = "getPackageManager";
    char* package_manager_parms = "()Landroid/content/pm/PackageManager;";
    jmethodID mid = (*env)->GetMethodID(env, cls, package_manager, package_manager_parms);
    if (mid == NULL)
    {
        goto error;
    }
    // 获得应用包的管理器
    jobject pm = (*env)->CallObjectMethod(env, context, mid);
    if (pm == NULL)
    {
       goto error;
    }
    // 得到getPackageName方法的ID
    char* c_package_name = "getPackageName";
    char* package_name_sign = "()Ljava/lang/String;";
    jmethodID mid_package_name = (*env)->GetMethodID(env, cls, c_package_name, package_name_sign);
    if (mid_package_name == NULL)
    {
        goto error;
    }
    // 获得当前应用包名
    jstring package_name = (jstring)(*env)->CallObjectMethod(env, context, mid_package_name);
    if (package_name == NULL)
    {
        goto error;
    }
    // 获得PackageManager类
    jclass pm_cls = (*env)->GetObjectClass(env, pm);
    if (pm_cls == NULL)
    {
        goto error;
    }
    // 得到getPackageInfo方法的ID
    char* c_package_info = "getPackageInfo";
    char* package_info_sign = "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;";
    jmethodID mid_package_info  = (*env)->GetMethodID(env, pm_cls, c_package_info, package_info_sign);
    if (mid_package_info == NULL)
    {
        goto error;
    }
    // 获得应用包的信息
    jobject package_info = (*env)->CallObjectMethod(env, pm, mid_package_info, package_name, 0x40); //GET_SIGNATURES = 64;
    if (package_info == NULL)
    {
        goto error;
    }
    // 获得PackageInfo 类
    jclass package_info_cls = (*env)->GetObjectClass(env, package_info);
    if (package_info_cls == NULL)
    {
        goto error;
    }
    // 获得签名数组属性的ID
    char* c_signatures = "signatures";
    char* c_signatures_sign = "[Landroid/content/pm/Signature;";
    jfieldID fid = (*env)->GetFieldID(env, package_info_cls, c_signatures, c_signatures_sign);
    if (fid == 0)
    {
        goto error;
    }
    // 得到签名数组
    jobjectArray signatures = (jobjectArray)(*env)->GetObjectField(env, package_info, fid);
    if (signatures == NULL)
    {
        goto error;
    }
    // 得到签名信息
    jobject signature_object = (*env)->GetObjectArrayElement(env, signatures, 0);
    if (signature_object == NULL)
    {
        goto error;
    }
    // 获得Signature类
    jclass  signature_class = (*env)->GetObjectClass(env, signature_object);
    if (signature_class == NULL)
    {
        goto error;
    }

    jmethodID methodId_of_toByteArray = (*env)->GetMethodID(env, signature_class, "toByteArray", "()[B");
    if(methodId_of_toByteArray == NULL)
    {
        goto error;
    }

    jbyteArray signature_byte = (jbyteArray)(*env)->CallObjectMethod(env, signature_object, methodId_of_toByteArray);

    if(signature_byte == NULL)
    {
        goto error;
    }
    // 将签名信息转化为SHA1的代码
    jclass byte_array_input_class=(*env)->FindClass(env, "java/io/ByteArrayInputStream");
    jmethodID methodId_of_byte_array_input_class = (*env)->GetMethodID(env, byte_array_input_class,"<init>","([B)V");
    jobject byte_array_input = (*env)->NewObject(env, byte_array_input_class,methodId_of_byte_array_input_class,signature_byte);
    jclass certificate_factory_class = (*env)->FindClass(env, "java/security/cert/CertificateFactory");
    jmethodID methodId_of_getInstance = (*env)->GetStaticMethodID(env, certificate_factory_class,"getInstance","(Ljava/lang/String;)Ljava/security/cert/CertificateFactory;");
    jstring x_509_jstring = (*env)->NewStringUTF(env, "X.509");
    jobject cert_factory = (*env)->CallStaticObjectMethod(env, certificate_factory_class, methodId_of_getInstance, x_509_jstring);
    jmethodID methodId_of_generateCertificate = (*env)->GetMethodID(env, certificate_factory_class,"generateCertificate",("(Ljava/io/InputStream;)Ljava/security/cert/Certificate;"));
    jobject x509_cert = (*env)->CallObjectMethod(env, cert_factory,methodId_of_generateCertificate,byte_array_input);

    jclass x509_cert_class=(*env)->GetObjectClass(env, x509_cert);
    jmethodID methodId_getEncoded = (*env)->GetMethodID(env, x509_cert_class,"getEncoded","()[B");
    jbyteArray cert_byte=(jbyteArray)(*env)->CallObjectMethod(env, x509_cert, methodId_getEncoded);

    jclass message_digest_class=(*env)->FindClass(env,"java/security/MessageDigest");
    jmethodID methodId_getInstance =(*env)->GetStaticMethodID(env, message_digest_class,"getInstance","(Ljava/lang/String;)Ljava/security/MessageDigest;");
    jstring sha1_jstring=(*env)->NewStringUTF(env, "SHA1");
    jobject sha1_digest=(*env)->CallStaticObjectMethod(env, message_digest_class,methodId_getInstance,sha1_jstring);
    jmethodID methodId_of_digest = (*env)->GetMethodID(env, message_digest_class,"digest","([B)[B");
    jbyteArray sha1_byte=(jbyteArray)(*env)->CallObjectMethod(env, sha1_digest,methodId_of_digest,cert_byte);

    //转换成char
    jsize array_size=(*env)->GetArrayLength(env, sha1_byte);
    jbyte* sha1 =(*env)->GetByteArrayElements(env, sha1_byte, 0);

    // 对获取的key进程16进制转换
    char *sha1_result = (char *)calloc(array_size * 3, sizeof(char));
    if(sha1_result == NULL )
    {
        goto error;
    }
    int index = 0;
    char temp_hex[3];
    for (int i = 0; i < array_size; i++)
    {
        char_to_hex(sha1[i], temp_hex);
        sha1_result[index++] = temp_hex[0];
        sha1_result[index++] = temp_hex[1];
        if (i >= array_size - 1)
        {
            sha1_result[index++] = 0;
        }
    }

    // 释放资源
    if (cls != NULL)
    {
        (*env)->DeleteLocalRef(env, cls);
        cls = NULL;
    }
    if (certificate_factory_class != NULL)
    {
        (*env)->DeleteLocalRef(env, certificate_factory_class);
        certificate_factory_class = NULL;
    }
    if (x509_cert_class != NULL)
    {
        (*env)->DeleteLocalRef(env, x509_cert_class);
        x509_cert_class = NULL;
    }
    if (message_digest_class != NULL)
    {
        (*env)->DeleteLocalRef(env, message_digest_class);
        message_digest_class = NULL;
    }
    // 返回当前应用签名信息
    return sha1_result;

error:
    return NULL;

}

// 获取应用的包名
char* get_package_name(JNIEnv *env, jobject context)
{
    char* result = (char*)calloc(64, sizeof(char));

    if (context == NULL)
    {
        goto error;
    }
    // 获得Context类
    jclass cls = (*env)->GetObjectClass(env, context);
    if (cls == NULL)
    {
        goto error;
    }

    // 得到getPackageManager方法的ID
    char* package_manager = "getPackageManager";
    char* package_manager_parms = "()Landroid/content/pm/PackageManager;";
    jmethodID mid = (*env)->GetMethodID(env, cls, package_manager, package_manager_parms);
    if (mid == NULL)
    {
        goto error;
    }

    // 获得应用包的管理器
    jobject pm = (*env)->CallObjectMethod(env, context, mid);
    if (pm == NULL)
    {
        goto error;
    }
    // 得到getPackageName方法的ID
    char* c_package_name = "getPackageName";
    char* package_name_sign = "()Ljava/lang/String;";
    jmethodID mid_package_name = (*env)->GetMethodID(env, cls, c_package_name, package_name_sign);
    if (mid_package_name == NULL)
    {
        goto error;
    }
    // 获得当前应用包名
    jstring package_name = (jstring)(*env)->CallObjectMethod(env, context, mid_package_name);
    if (package_name == NULL)
    {
        goto error;
    }
    else
    {
        char* c_package_name = (char*) (*env)->GetStringUTFChars(env, package_name, NULL);
        //char* c_package_name = jstring_to_char(env, package_name);
        strcpy(result, c_package_name);
        (*env)->ReleaseStringUTFChars(env, package_name, c_package_name);
        //free(c_package_name);
    }

    // 资源清理
    if (cls != NULL)
    {
        (*env)->DeleteLocalRef(env, cls);
        cls = NULL;
    }
    return result;

error:
    safe_free(result);
    return NULL;
}

