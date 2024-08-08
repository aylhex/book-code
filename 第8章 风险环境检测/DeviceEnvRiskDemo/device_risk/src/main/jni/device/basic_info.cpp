#include <basic_info.h>
#include <mjson.h>
#include <tools.h>
#include <mac_info.h>
#include <cpu_info.h>
#include <property_info.h>
#include <main_simulator.h>
#include <main_fraud.h>

#define SDK_VERSION "1.0.0"

namespace devicerisk {
    /*
    根据返回值判断是否为root状态
    返回值为1表示为root状态
    返回值为0表示为非root状态
     */
    bool CheckDeviceRootState()
    {
        const char *var1 = "/su";
        const char *var2 = "/su/bin/su";
        const char *var3 = "/sbin/su";
        const char *var4 = "/data/local/xbin/su";
        const char *var5 = "/data/local/bin/su";
        const char *var6 = "/data/local/su";
        const char *var7 = "/system/xbin/su";
        const char *var8 = "/system/bin/su";
        const char *var9 = "/system/sd/xbin/su";
        const char *var10 = "/system/bin/failsafe/su";
        const char *var11 = "/system/bin/cufsdosck";
        const char *var12 = "/system/xbin/cufsdosck";
        const char *var13 = "/system/bin/cufsmgr";
        const char *var14 = "/system/xbin/cufsmgr";
        const char *var15 = "/system/bin/cufaevdd";
        const char *var16 = "/system/xbin/cufaevdd";
        const char *var17 = "/system/bin/conbb";
        const char *var18 = "/system/xbin/conbb";

        const char *files[18] = {var1, var2, var3, var4, var5, var6, var7, var8, var9,
                                 var10, var11, var12, var13, var14, var15, var16, var17, var18};

        for (int count = 0; count < 18; count++)
        {
            if (Tools::CheckFileExist(files[count]))
            {
                return true;
            }
        }
        return false;
    }

    /**
     *  获取设备被基本信息
     *  clientinfo java传入的设备信息
     *  permission = 0 没有SDCard权限
     */
    void AddDeviceDataToJson(Document &_json,const string &_client_info)
    {
        // 初始化Json
        Document device_data = MJson::NewJsonObject();
        // 获取设备基本信息
        string basicinfo = MJson::GetString(_client_info, "basicinfo");
        if (!basicinfo.empty())
        {
            MJson::AddString(device_data, "basicinfo", basicinfo);
            MJson::AddString(_json, "devicedata", MJson::ToJsonString(device_data));
        }

        if (CheckDeviceRootState())
        {
            MJson::AddInt(_json, "root", 1);
        }
        else
        {
            MJson::AddInt(_json, "root", 0);
        }

        CpuInfo::AddCpuDataToJson(_json);

        PropertyInfo::AddDevicePropertyToJson(_json);

        MainSimulator::AddSimulatorDataToJson(_json);

    }
    /**
     *  获取设备被基本信息
     *  clientinfo java传入的设备信息
     *  permission = 0 没有SDCard权限
     */
    string BasicInfo::ParseDeviceInfoFromJava(JNIEnv *env, const string &_client_info, int permission)
    {
        // 初始化Json
        Document json = MJson::NewJsonObject();
        MJson::AddString(json, "sdkversion", SDK_VERSION);

        // 获取java层传入的sdcard 路径
        string sdcard_path = MJson::GetString(_client_info, "path");
        if (sdcard_path.empty())
        {
            sdcard_path = "/sdcard";
        }
        // 获取java层传入的macinfo地址
        string mac_info = MJson::GetString(_client_info, "mac");
        if (!mac_info.empty() && permission == 1)
        {
            MacInfo::HideMacAddress(mac_info, sdcard_path);
        }

        char* macinfo = MacInfo::GetMacAddress(sdcard_path.c_str(), mac_info.c_str(), permission);
        if(!Tools::IsEmpty(macinfo))
        {
            MJson::AddString(json, "macinfo", macinfo);
        } else{
            MJson::AddString(json, "macinfo", DEFAULT);
        }

        AddDeviceDataToJson(json, _client_info);
        MainFraud::AddAntiFraudDataToJson(env,json);

        return MJson::ToJsonString(json);
    }
}