#ifndef _JNI_SIMULATOR_H
#define _JNI_SIMULATOR_H
#include <stdio.h>
#include <stdlib.h>
#include <mjson.h>

namespace devicerisk {

class MainSimulator
{
public:
    /*
    获取模拟器属性特征列表
    返回值为0  说明 没有检测到模拟器属性
    返回值为非0  说明检测到模拟器属性
     */
//    static int AddVmPropertysToJson(Document &_json);

    /*
    获取模拟器特征文件列表
    返回值为0  说明 没有检测到模拟器属性
    返回值为非0  说明检测到模拟器文件个数
     */
//    static int AddVmFilesToJson(Document &_json);

    /*
    获取模拟器文件列表
    返回值为0  说明 没有检测文件
    返回值为非0  说明检测到模拟器文件个数
    */
//    static int AddRootDirVmSpecialsFile(Document &_json);

    /*
    获取指定特征的进行信息
    ret 返回检测的数量
    */
//    static int AddExtendVmFeaturesToJson(Document &_json);

    /*
    获取设备模拟器信息
    返回值为0 说明没有信息
    返回值为非0 说明获取到模拟器信息
     */
    static int AddSimulatorDataToJson(Document &_json);
};
}
#endif