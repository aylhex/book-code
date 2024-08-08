#include <main_fraud.h>
#include <tools.h>

#define DEFAULT "6666"

namespace devicerisk
{
    jobject getGlobalContext(JNIEnv *env)
    {
        jobject context = NULL;
        //获取Activity Thread的实例对象
        jclass activityThread = env->FindClass("android/app/ActivityThread");
        jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
        jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);
        //获取Application，也就是全局的Context
        jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
        context = env->CallObjectMethod(at, getApplication);
        return context;
    }


    /*
    返回值为0 说明没有hook信息
    返回值为非0 说明获取到风险信息
     */
    void MainFraud::AddAntiFraudDataToJson(JNIEnv *env,  Document &_json)
    {
        jobject mContext = getGlobalContext(env);

        char* package_name = AntiFraud::GetPackageName(env, mContext);
        // 获取hook框架信息
        char* hookframworkbuffer = NULL;
        // 获取注入dex信息
        char* inject_dex_buffer = NULL;
        // 获取注入信息
        char* injectsobuffer = NULL;
        // 获取异常module信息
        AntiFraud::GetModuleFraudInfo(package_name, &hookframworkbuffer, &injectsobuffer, &inject_dex_buffer);
        if (!Tools::IsEmpty(hookframworkbuffer))
        {
            MJson::AddString(_json, "hookFramworkInfo", hookframworkbuffer);
        }
        else
        {
            MJson::AddString(_json, "hookFramworkInfo", DEFAULT);
        }
        free(hookframworkbuffer);

        if (!Tools::IsEmpty(injectsobuffer))
        {
            MJson::AddString(_json, "injectSoInfo", injectsobuffer);
        }
        else
        {
            MJson::AddString(_json, "injectSoInfo", DEFAULT);
        }
        free(injectsobuffer);

        if (!Tools::IsEmpty(inject_dex_buffer))
        {
            MJson::AddString(_json, "injectDexInfo", inject_dex_buffer);
        }
        else
        {
            MJson::AddString(_json, "injectDexInfo", DEFAULT);
        }
        free(inject_dex_buffer);
        free(package_name);
        // 获取客户端签名
        char* appsign = AntiFraud::GetSignature(env, mContext);
        if(!Tools::IsEmpty(appsign))
        {
            MJson::AddString(_json, "appsign", appsign);
            free(appsign);
        }
        else
        {
            MJson::AddString(_json, "appsign", DEFAULT);
        }
        // 获取SElinux状态信息
        char* selinux = AntiFraud::GetSelinuxState();
        if(!Tools::IsEmpty(selinux))
        {
            MJson::AddString(_json, "selinux", selinux);
            free(selinux);
        }
        else
        {
            MJson::AddString(_json, "selinux", DEFAULT);
        }
        //获取magisk相关信息
        char* magisk = AntiFraud::GetMagiskInfo();
        if (!Tools::IsEmpty(magisk))
        {
            MJson::AddString(_json, "magisk", magisk);
            free(magisk); 
        }
        else
        {
            MJson::AddString(_json, "magisk", DEFAULT);
        }
    }
}