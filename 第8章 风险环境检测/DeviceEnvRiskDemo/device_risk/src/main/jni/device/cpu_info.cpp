#include <cpu_info.h>
#include <tools.h>
#include <sys/system_properties.h>
namespace devicerisk
{
    /**
     * 获取CPU属性值
     */
    void  CpuInfo::GetCpuInfoProperty(Document &_json)
    {
        string cmmand;
        char* result = (char*)calloc(128, sizeof(char));
        const char* hardware_key = "hardware";
        const char* cpuabi_key = "cpuabi";
        const char* cpuabilist_key = "cpuabilist";

        const char* hardware = "ro.boot.hardware";
        const char* cpuabi = "ro.product.cpu.abi";
        const char* cpuabilist = "ro.product.cpu.abilist";

        const char* propertys[3] = {hardware, cpuabi, cpuabilist};
        const char* keys[3] = {hardware_key, cpuabi_key, cpuabilist_key};

        for (size_t num = 0; num < 3; num++)
        {
            int ret = __system_property_get(propertys[num], result);
            if (ret > 0)
            {
                MJson::AddString(_json, keys[num], result);

                memset(result, 0, 128);
                continue;
            }
            // 第一种方式获取失败采用这种方式获取
            cmmand = "getprop ";
            cmmand.append(propertys[num]);
            FILE* fp = popen(cmmand.c_str(), "r");
            if (fp != NULL)
            {
                fgets(result, 127, fp);
                Tools::TrimString(result);
                if (result != NULL && strlen(result) > 0)
                {
                    MJson::AddString(_json, keys[num], result);
                }
                else
                {
                    MJson::AddString(_json, keys[num], DEFAULT);
                }
                memset(result, 0, 128);
                pclose(fp);
            }
            else
            {
                MJson::AddString(_json, keys[num], DEFAULT);
            }
            memset(result, 0, 128);
        }
        free(result);
    }



    /**
     * 获取CPU核心数
     * ret -1 获取失败
     * ret 1  获取成功
     */
    int CpuInfo::GetCpuCoreCount(char **buffer)
    {
        int ret = -1;
        *buffer = (char*)calloc(16, sizeof(char));
        FILE* fp = popen("cat /sys/devices/system/cpu/present", "r");

        if (fp != NULL)
        {
            fgets(*buffer, 16, fp);
            Tools::TrimString(*buffer);
            ret = 1;
            pclose(fp);
        }
        return ret;
    }

    /**
     * 获取CPU类型
     * 返回-1 获取失败
     * 返回 1 获取成功
    */
    int CpuInfo::GetCpuType(char **buffer)
    {
        int flag = -1;
        *buffer = (char*)calloc(128, sizeof(char));
        FILE* fp = NULL;

        fp = fopen("/proc/cpuinfo", "r");

        if (NULL != fp)
        {
            while (fgets(*buffer, 127, fp) != NULL)
            {
                if ((strstr(*buffer, "GenuineIntel") != NULL) || (strstr(*buffer, "Intel") != NULL) ||
                    (strstr(*buffer, "AMD") != NULL) || (strstr(*buffer, "GenuineAMD") != NULL) ||
                    (strstr(*buffer, "AArch64") != NULL) || (strstr(*buffer, "ARMv7") != NULL))
                {
                    //trim_string(*buffer);
                    flag = 1;
                    break;
                }
                memset(*buffer, 0, 128);
            }
            fclose(fp);
        }
        return flag;
    }

    // 获取cpu信息 添加到json
    void CpuInfo::AddCpuDataToJson(Document &_json)
    {
        // 获取CPU属性信息
        CpuInfo::GetCpuInfoProperty(_json);

        //获取CPU核心数
        char* cpucount;
        int cpucount_flag = CpuInfo::GetCpuCoreCount(&cpucount);
        if (cpucount_flag == 1)
        {
            MJson::AddString(_json, "cpucount", cpucount);
            free(cpucount);
        }
        else
        {
            MJson::AddString(_json, "cpucount", DEFAULT);
        }

        //获取CPU类型
        char *cputype = NULL;
        int cputype_flag = CpuInfo::GetCpuType(&cputype);
        if (cputype_flag == 1)
        {
            char *point = strtok(cputype, ":");
            point = strtok(NULL, ":");
            if (point != NULL)
            {
                Tools::TrimString(point);
                MJson::AddString(_json, "cputype", point);
            }
        }else
        {
            MJson::AddString(_json, "cputype", DEFAULT);
        }
        free(cputype);
    }
}



