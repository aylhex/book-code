#ifndef _HEAD_MAC_H
#define _HEAD_MAC_H
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/if.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <base64.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

namespace devicerisk
{
class MacInfo
{
public:

    /*
    根据网络端口获取对应的mac地址
    返回 NULL 说明获取mac地址失败
    返回  非NULL 说明获取mac地址成功
     */
    static char* GetMacAddressByWlan();

    // 获取mac地址 并在本地隐藏获取到的MAC地址
    static char* GetMacAddress(const char *_sdcard_path, const char *_mac_info, int _permission);

    // 隐藏mac地址
    static void HideMacAddress(string &_wlan_mac, string  &_sdcard_path);

    /**
     * 检测mac地址是否有效
     * return  1 有效
     * return -1 无效
     */
    static bool CheckMacValid(const char* mac_address);
};
}
#endif