#ifndef _HEAD_DEVICE_H
#define _HEAD_DEVICE_H
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <jni.h>
#include <iostream>

using namespace std;

namespace devicerisk
{
class BasicInfo
{

public:

    /**
     *  获取设备被基本信息
     *  c_clientinfo java传入的设备信息
     *  permission = 0 没有SDCard权限、没有获取设备状态权限
     *  permission = 1 客户端有sdcard读写权限和获取设备状态权限
     *  permission = 2 客户端有sdcard读写权限 没有获取设备状态权限
     *  permission = 3 只有获取取设备状态权限
     */
    static string ParseDeviceInfoFromJava(JNIEnv *env, const string &_client_info, int permission);
};
}

#endif

