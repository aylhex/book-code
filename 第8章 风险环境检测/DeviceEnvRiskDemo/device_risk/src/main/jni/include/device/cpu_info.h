#ifndef _HEADE_CPU_H
#define _HEADE_CPU_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <mjson.h>

using namespace std;

namespace devicerisk
{
class CpuInfo
{
public:

    /**
     * 获取CPU类型
     * 返回-1 获取失败
     * 返回 1 获取成功
    */
    static int GetCpuType(char **buffer);

    /**
     * 获取CPU核心数
     * ret -1 获取失败
     * ret 1  获取成功
     */
    static int GetCpuCoreCount(char **buffer);

    /**
     * 获取CPU属性值
     */
    static void GetCpuInfoProperty(Document &_json);

    // 获取cpu信息 添加到json
    static void AddCpuDataToJson(Document &_json);
};
}


#endif