#ifndef _SIMULATOR_H
#define _SIMULATOR_H
#include <stdio.h>
#include <stdlib.h>


namespace devicerisk {

class AntiSimulator
{
public:
    /*
    检测属性值中是否含有模拟器特征
    返回值为0 说明文件打开失败
    返回值为-2 说明分配内存失败
    */
    static int CheckProperty(char **_buffer, int *_ret_size);

    /*
    检测根目录下的文件
    正常返回文件的个数
    返回0 说明文件打开失败
    返回-2 说明内存申请失败
     */
    static int CheckSpecialFile(char **_buffer);


    /*
     查看模拟器中特有的进程   解决夜神模拟器最新版检测失败的问题
     或者 magisk相关的进程
     ret 返回检测到的特征数
     */
    static int CheckVmProcessExist(char **_buffer);

    /*
    检测根目录下的文件
    正常返回文件的个数
    返回0 说明文件获取失败或者没有文件
     */
    static int CheckRootDirSpecialFile(char **_buffer, int *_ret_size);

    /**
     * 检测/system/bin/的模拟器特征文件
     */
    static int CheckBinVmFileExist(char **_buffer, int *_ret_size);


    /**
     * 检测/dev/目录下的模拟器特征文件
     */
    static int CheckDevVmFileExist(char **_buffer, int *_ret_size);


    /**
     * 检测/system/lib/ 目录下的模拟器特征文件
     */
    static int CheckLibVmFileExist(char **_buffer, int *_ret_size);

    static int CheckFileByOpendir(const char *_path, const char **_in, int _in_len, char **_out);

    static int CheckFileByCommand(const char *_command, const char **_in, int _in_len, char **_out);
};
}
#endif
