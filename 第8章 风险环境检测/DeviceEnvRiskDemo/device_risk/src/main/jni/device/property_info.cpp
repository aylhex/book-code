#include <property_info.h>
#include <unistd.h>
#include <tools.h>

#define MAX 128

namespace devicerisk
{
    /*
    读取 /proc/self/cmdline
    */
    int GetCmdline(char *value)
    {
        FILE *fp = NULL;
        char cmd_name[64] = {0};
        pid_t pid = getpid();
        const char* cmdline = "cat proc/%d/cmdline";
        sprintf(cmd_name, cmdline, pid);
        if ((fp = popen(cmd_name, "r")) != NULL)
        {
            fgets(value, 63 * sizeof(char), fp);
            pclose(fp);
            return 1;
        }
        else
        {
            return -1;
        }
    }

    int CheckPhoneDebugstate()
    {
        FILE *fp = NULL;
        char *buf = (char *)malloc(MAX * sizeof(char)+1);
        int flag = 0;

        if (NULL == buf)
        {
            return -2;
        }
        memset(buf, 0, MAX * sizeof(char));
        fp = fopen("/default.prop", "r");

        if (NULL == fp)
        {
            free(buf);
            return -1;
        }

        while (fgets(buf, MAX * sizeof(char), fp) != NULL)
        {
            if (strstr(buf, "ro.debuggable=1") != NULL)
            {
                flag = 1;
            }
        }
        free(buf);
        fclose(fp);
        return flag;
    }

    // 获取系统属性的相关代码
    void GetDeviceProperty(Document &_json)
    {
        int count = 7;
        const char *debugstate = "ro.debuggable";
        const char *devicefinger = "ro.build.fingerprint";
        const char *usbstate = "sys.usb.state";
        const char *devicename = "ro.product.device";
        const char *hostname = "net.hostname";
        const char *brand = "ro.product.brand";
        const char *bootserialno = "ro.boot.serialno";

        const char *key_debugstate = "debuggable";
        const char *key_devicefinger = "devicefinger";
        const char *key_usbstate = "usbstate";
        const char *key_devicename = "devicename";
        const char *key_hostname = "hostname";
        const char *key_brand = "brand";
        const char *key_bootserialno = "bootserialno";

        const char *propertys[7] = {debugstate, devicefinger, usbstate, devicename, hostname, brand, bootserialno};
        const char *keys[7] = {key_debugstate, key_devicefinger, key_usbstate, key_devicename, key_hostname,  key_brand, key_bootserialno};

        char* cmmand = (char*)calloc(128, sizeof(char));
        char* result = (char*)calloc(128, sizeof(char));
        // 检测手机调试状态是否开启 特殊处理
        int debug = CheckPhoneDebugstate();
        if (1 == debug)
        {
            MJson::AddString(_json, keys[0], "1");
        }
        else
        {
            int ret = __system_property_get(propertys[0], result);
            if (ret > 0)
            {
                MJson::AddString(_json, keys[0], result);
                goto start;
            }
            // 第一种检测手机调试状态失败采用这种方式获取
            strcat(cmmand, "getprop ");
            strcat(cmmand, propertys[0]);
            FILE* fp = popen(cmmand, "r");
            if (fp != NULL)
            {
                fgets(result, 127, fp);
                if (result != NULL && strlen(result) > 0)
                {
                    Tools::TrimString(result);
                    MJson::AddString(_json, keys[0], result);
                }else
                {
                    MJson::AddString(_json, keys[0], "0");
                }
                pclose(fp);
            }else
            {
                MJson::AddString(_json, keys[0], "0");
            }
        }

    start:
        for (size_t num = 1; num < count; num++)
        {
            memset(result, 0, 128);
            memset(cmmand, 0, 128);
            int ret = __system_property_get(propertys[num], result);
            if (ret > 0)
            {
                MJson::AddString(_json, keys[num], result);
                continue;
            }
            // 第一种方式获取失败采用这种方式获取
            strcat(cmmand, "getprop ");
            strcat(cmmand, propertys[num]);
            FILE* fp = popen(cmmand, "r");
            if (fp != NULL)
            {
                fgets(result, 127, fp);
                Tools::TrimString(result);
                if (result != NULL && strlen(result) > 0)
                {
                    MJson::AddString(_json, keys[num], result);
                }else
                {
                    MJson::AddString(_json, keys[num], DEFAULT);
                }
                memset(cmmand, 0, 128);
                pclose(fp);
            }
            else
            {
                MJson::AddString(_json, keys[num], DEFAULT);
            }
        }
        // 释放内存
        free(result);
        free(cmmand);
    }

    // 获取设备属性
    void PropertyInfo::AddDevicePropertyToJson(Document &_json)
    {
        // 获取cmdline
        char cmdline[64] = {0};
        int cmdline_flag = GetCmdline(cmdline);
        if (cmdline_flag > 0)
        {
            MJson::AddString(_json,"cmdline", cmdline);
        }
        else
        {
            MJson::AddString(_json,"cmdline", DEFAULT);
        }
        // 获取属性相关信息
        GetDeviceProperty(_json);
    }
}