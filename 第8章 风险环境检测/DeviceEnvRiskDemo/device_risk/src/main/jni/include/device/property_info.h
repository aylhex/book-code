#ifndef _HEAD_PROPERTY_H
#define _HEAD_PROPERTY_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/system_properties.h>
#include <mjson.h>

namespace devicerisk
{
class PropertyInfo
{
public:
    // 获取设备属性 添加到json
    static void AddDevicePropertyToJson(Document &_json);
};

}

#endif