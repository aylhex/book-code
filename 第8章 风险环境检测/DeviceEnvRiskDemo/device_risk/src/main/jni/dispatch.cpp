#include <dispatch.h>

#include <mjson.h>
#include <mlog.h>
#include <tools.h>
#include <basic_info.h>

namespace devicerisk
{
    /**
     * 获取采集的数据
     * @param env
     * @param context
     * @param origin_params
     * @return
     */
    jbyteArray Dispatch::GetUploadData(JNIEnv *env, char* _origin_params)
    {
        int ret = 0;
        int sdcard_perm_state = 0;
        int signal_switch = 0;

        // 参数解析逻辑
        string clinet_info = MJson::GetString(_origin_params, "client_info");

        LOGI("clinet_info = %s", clinet_info.c_str());

        if (clinet_info.empty())
        {
            free(_origin_params);
            return Tools::CovertByteToArray(env, nullptr);
        }

        string result =  BasicInfo::ParseDeviceInfoFromJava(env, clinet_info, sdcard_perm_state);

        LOGI("json_data = %s", result.c_str());

        return Tools::CovertByteToArray(env, result.c_str());
    }

}