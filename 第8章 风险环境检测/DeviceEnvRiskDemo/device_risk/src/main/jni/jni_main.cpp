
#include <jni_main.h>
namespace devicerisk
{
    jbyteArray JNICALL do_command(JNIEnv *env, jclass  clazz, jbyteArray _origin_params)
    {
        // 将传入的jbyteArray参数类型转为char*类型
        jbyte *origin_params_array = env->GetByteArrayElements(_origin_params, 0);
        int origin_params_array_len= env->GetArrayLength(_origin_params);
        char* origin_params = (char*)calloc(origin_params_array_len+1, sizeof(char));
        if (origin_params == nullptr)
        {
            return nullptr;
        }
        for(int num=0; num < origin_params_array_len; num++)
        {
            origin_params[num] = origin_params_array[num];
        }
        // 释放资源
        env->ReleaseByteArrayElements(_origin_params, origin_params_array, 0);

        return Dispatch::GetUploadData(env, origin_params);
    }
}



