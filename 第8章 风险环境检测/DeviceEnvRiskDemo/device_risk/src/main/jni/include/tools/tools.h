#ifndef TOOLS_H
#define TOOLS_H
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <libgen.h>
#include <fstream>

#define DEFAULT "6666"

using namespace std;

namespace devicerisk
{
class Tools
{
public:
    //去掉首尾空格
    static string& Trim(string &str);

    /*
    检测文件大小
    返回 文件大小
     */
    static int GetFileSize(const char* _file_path);

    /*
    检测目录是否存在
    返回 true 说明文件目录存在
    返回 false 说明文件目录bu存在
     */
    static bool CheckDirExist(const char* _dir_path);

    static jbyteArray CovertByteToArray(JNIEnv *env, const char* _data);

    /**
     * 检测文件是否存在
     * 返回true  文件存在
     * 返回false 文件不存在
     * @param filePath
     * @return
     */
    static bool CheckFileExist(const char* _file_path);


    /**
     * 根据路径和文件名检测是否存在
     * @param file_dir
     * @param file_name
     * @return
     */
    static bool CheckFileExistByName(const char *_file_dir, const char* _file_name);


    /**
     * 读取文件内容
     * 返回 -1 读取失败
     * 返回 读取的内容长度
    */
    static int ReadFile(const char *_file_name, char** _content);

    /**
     * 检测是否为空字符串
     * @param data
     * @return
     */
    static bool IsEmpty(const char *_data);


    /*
    检测字符串是否以XXX开头
    返回值为true 说明是以字符串开头
    返回值为false 说明不是以字符串开头
    */
    static bool StartWith(const char *line, const char *start);

    /*
    检测字符串是否以XXX结尾
    返回值为true 说明是以字符串结尾
    返回值为false 说明不是以字符串结尾
     */
    static bool EndWith(const char *line, const char *end);

    // 生成随机字符串
    static char* RandString(int length);

    // 去除字符串两端空格及回车
    static void TrimString(char *line);
    /*
    * 把java字符串转化为c字符串
    * 返回NULL 转化失败
    */
    static char *JstringToChar(JNIEnv *env, jstring jstr);
};
}
#endif //TOOLS_H
