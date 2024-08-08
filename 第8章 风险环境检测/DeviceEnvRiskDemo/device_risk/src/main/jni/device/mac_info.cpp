#include <mac_info.h>
#include <sys/stat.h>
#include <tools.h>

namespace devicerisk
{
    /**
     * 检测mac地址是否有效
     * return  1 有效
     * return -1 无效
     */
    bool MacInfo::CheckMacValid(const char* _mac_address)
    {
        int result = true;
        if (_mac_address == NULL)
        {
            return false;
        }

        if (strlen(_mac_address) < 17 )
        {
            return false;
        }
        const char* abormal = "abnormal";
        const char* invalid = "invalid";
        const char* Error = "Error";
        const char* except_error = "except_error";
        const char* None = "None";
        const char* valid[5] = {abormal,  invalid, Error, except_error, None};
        for (size_t i = 0; i < 5; i++)
        {
            if (valid[i] != NULL && strlen(valid[i]) <= 12 && strcasecmp(_mac_address, valid[i]) == 0)
            {
                result = false;
                break;
            }
        }
        return result;
    }

    /*
    获取mac地址 并在本地隐藏获取到的MAC地址
    */
    char*  MacInfo::GetMacAddress(const char *_sdcard_path, const char *_mac_info, int _permission)
    {
        /*
        *  permission = 0 没有SDCard权限
        *  permission = 1 有sdcard读写权限
        */
        char* ndkmacaddress = NULL;
        char* wlan_mac = GetMacAddressByWlan();
        bool  mac_info_flag= CheckMacValid(_mac_info);

        // 拼接隐藏路径
        string dir_path;
        if(!Tools::IsEmpty(_sdcard_path)) //防止c_sdpath 为空 引起crash
        {
            dir_path = _sdcard_path;
            dir_path.append("/Android/data");
        }
        else if (dir_path.empty())
        {
            dir_path = "/sdcard/Android/data";
        }

        // 首先尝试从隐藏文件中读取mac地址
        if (_permission == 1)
        {
            dir_path.append("/.mac");
            if (!Tools::CheckFileExist(dir_path.c_str()))
            {
                goto end;
            }

            char* wlan = NULL;
            if (Tools::ReadFile(dir_path.c_str(), &wlan) > 0)
            {
                // 不要忘记释放内存
                char* mac_decode64 = Base64::Decode(wlan, strlen(wlan));
                if (mac_decode64 == NULL)
                {
                    goto end;
                }
                ndkmacaddress = (char *)calloc(20, sizeof(char));
                strcpy(ndkmacaddress, mac_decode64);
                // 释放内存
                free(mac_decode64);
                free(wlan);
                goto end;
            }
        }

        // 隐藏文件中获取失败 实时获取MAC地址
        if (wlan_mac == NULL && !mac_info_flag)
        {
            // 无法获取mac地址
            return ndkmacaddress;
        }
        if (wlan_mac != NULL )
        {
            ndkmacaddress = (char *)calloc(20, sizeof(char));
            if (ndkmacaddress == NULL)
            {
                goto end;
            }
            strcpy(ndkmacaddress, wlan_mac);
            goto end;
        }

        if (mac_info_flag)
        {
            ndkmacaddress = (char *)calloc(20, sizeof(char));
            if (ndkmacaddress == NULL)
            {
                goto end;
            }
            strcpy(ndkmacaddress, _mac_info);
            goto end;
        }

    end: // 清理数据
        if(wlan_mac != NULL)
        {
            free(wlan_mac);
        }
        return ndkmacaddress;
    }

    /**
     * 隐藏mac地址
     */
    void MacInfo::HideMacAddress(string &_wlan_mac, string  &_sdcard_path)
    {
        int mac_len = 0;
        char* mac_base64 = NULL;
        char* ndk_mac = GetMacAddressByWlan();

        // 防止无效数据干扰
        bool flag = CheckMacValid(_wlan_mac.c_str());
        // 异常情况不保存
        if(_sdcard_path.empty() || !flag)
        {
            return;
        }
        // 判断java层和jni层获取的mac地址是否有效
        if ( flag && ndk_mac != NULL)
        {
            // 判断java层和jni层获取的mac地址都有效
            if (strcasecmp(ndk_mac, _wlan_mac.c_str()) != 0 )
            {
                mac_len = strlen(ndk_mac);
                // 不要忘记释放内存
                mac_base64 = Base64::Encode(ndk_mac, mac_len);
                //mac_base64 = base64_encode(ndk_mac, mac_len);
                free(ndk_mac);
            }
            else
            {
                mac_len = _wlan_mac.length();
                // 不要忘记释放内存
                mac_base64 = Base64::Encode(_wlan_mac.c_str(), mac_len);
                //mac_base64 = base64_encode(_wlan_mac.c_str(), mac_len);
            }
        }
        else if (ndk_mac != NULL)       // jni层获取的有效
        {
            mac_len = strlen(ndk_mac);
            // 不要忘记释放内存
            mac_base64 = Base64::Encode(ndk_mac, mac_len);
            //mac_base64 = base64_encode(ndk_mac, mac_len);
            free(ndk_mac);
        }
        else if (flag)              // java层获取的有效
        {
            mac_len = _wlan_mac.length();
            // 不要忘记释放内存
            mac_base64 = Base64::Encode(_wlan_mac.c_str(), mac_len);
            //mac_base64 = base64_encode(_wlan_mac.c_str(), mac_len);
        }

        // 防止因为路径太长导致内存溢出
        if (_sdcard_path.length() > 108)
        {
            return;
        }
        string dir_path = _sdcard_path;
        dir_path.append("/Android/data");
        // 隐藏的第一个文件
        if (Tools::CheckDirExist(dir_path.c_str()))
        {
            dir_path.append("/.mac");
            if (1 == Tools::CheckFileExist(dir_path.c_str()))
            {
                int len = Tools::GetFileSize(dir_path.c_str());
                if (len <=  18)
                {
                    FILE* fp = NULL;
                    if ((fp=fopen(dir_path.c_str(), "w")) != NULL)
                    {
                        fputs(mac_base64, fp);
                        fclose(fp);
                    }
                }
            }
            else
            {
                FILE* fp = NULL;
                if ((fp=fopen(dir_path.c_str(), "w")) != NULL)
                {
                    fputs(mac_base64, fp);
                    fclose(fp);
                }
            }
        }
        else if (0 == mkdir(dir_path.c_str(),0777))
        {
            dir_path.append("/.mac");

            FILE* fp = NULL;
            if ((fp=fopen(dir_path.c_str(), "w")) != NULL)
            {
                fputs(mac_base64, fp);
                fclose(fp);
            }
        }

        // 隐藏的第二个文件
        string sd_root_dir = _sdcard_path;
        sd_root_dir.append("/.mac2");
        if (1 == Tools::CheckFileExist(sd_root_dir.c_str()))
        {
            int len = Tools::GetFileSize(sd_root_dir.c_str());
            if (len <=  18)
            {
                FILE* fp = NULL;
                if ((fp=fopen(sd_root_dir.c_str(), "w")) != NULL)
                {
                    fputs(mac_base64, fp);
                    fclose(fp);
                }
            }
        }else
        {
            FILE* fp = NULL;
            if ((fp=fopen(sd_root_dir.c_str(), "w")) != NULL)
            {
                fputs(mac_base64, fp);
                fclose(fp);
            }
        }
        // 释放内存
        free(mac_base64);
    }



    /*
    根据网络端口获取对应的mac地址
    返回 NULL 说明获取mac地址失败
    返回  非NULL 说明获取mac地址成功
     */
    char* MacInfo::GetMacAddressByWlan()
    {
        char* mac_address = NULL;
        const char* wlan_name = "wlan0";
        int msocket = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 != msocket)
        {
            struct ifreq ifr;
            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, wlan_name, strlen(wlan_name));
            /**
             * 获取IP地址
             * ioctl(sock, SIOCGIFADDR, &ifr);
             * 获取网卡mac地址
             * ioctl(msocket, SIOCGIFHWADDR, &ifr)
             * 获取网卡接口地址
             * ioctl(sock, SIOCGIFINDEX, &ifr)
             */
            if (ioctl(msocket, SIOCGIFHWADDR, &ifr) >= 0)
            {
                // LOGI("mac = %s", "ioctl");
                mac_address = (char *) calloc(20, sizeof(char));
                if(mac_address != NULL)
                {
                    for (size_t i = 0; i < 6; i++)
                    {
                        //格式化 mac address
                        sprintf(mac_address+3*i, "%02x:", ifr.ifr_hwaddr.sa_data[i]);
                    }
                }
            }
            close(msocket);
        }

        // 检测获取的mac地址是否有效
        if (mac_address == NULL || strlen(mac_address) < 17 || !CheckMacValid(mac_address))
        {
            free(mac_address);
            return NULL;
        }

        return mac_address;
    }
}