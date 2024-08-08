#include <tools.h>
#include <dirent.h>
#include <asm/fcntl.h>


namespace devicerisk
{
    /**
     * 检测是否为空字符串
     * 返回true 为空
     * 返回false 为非空
     * @param data
     * @return
     */
    bool Tools::IsEmpty(const char *_data)
    {
        return _data == NULL || strlen(_data) == 0;
    }

    //去掉首尾空格
    string& Tools::Trim(string &str)
    {
        if (str.empty())
        {
            return str;
        }

        str.erase(0,str.find_first_not_of(" "));
        str.erase(str.find_last_not_of(" ") + 1);
        return str;
    }

    jbyteArray Tools::CovertByteToArray(JNIEnv *env, const char* _data)
    {
        if (_data == nullptr)
        {
            const char* invalid = "invalid";
            int array_len = strlen(invalid);
            jbyteArray result = env->NewByteArray(array_len);
            env->SetByteArrayRegion(result, 0, array_len, (jbyte*)invalid);
            return result;
        }
        else
        {
            int array_len = strlen(_data);
            jbyteArray result = env->NewByteArray(array_len);
            env->SetByteArrayRegion(result, 0, array_len, (jbyte*)_data);
            return result;
        }
    }

    /*
    检测目录是否存在
    返回 true 说明文件目录存在
    返回 false 说明文件目录bu存在
     */
    bool Tools::CheckDirExist(const char* _dir_path)
    {
        //创建一个DIR结构的指针
        DIR *dp ;

        if (Tools::IsEmpty(_dir_path))
        {
            return false;
        }
        dp = opendir(_dir_path);
        if (dp == NULL)
        {
            return false;
        }
        closedir(dp);
        return true;
    }
    /**
     * 根据路径和文件名检测是否存在
     * @param file_dir
     * @param file_name
     * @return
     */
    bool Tools::CheckFileExistByName(const char *_file_dir, const char* _file_name)
    {
        if(Tools::IsEmpty(_file_dir) || Tools::IsEmpty(_file_name))
        {
            return false;
        }
        // 完整路径拼接
        char* file_path = static_cast<char *>(calloc(strlen(_file_dir) + strlen(_file_name) + 1,sizeof(char)));
        memcpy(file_path,_file_dir, strlen(_file_dir));
        strncat(file_path, _file_name, strlen(_file_name));

        // 检测方法一
        if (access(file_path, R_OK) == 0)
        {
            free(file_path);
            return true;
        }
        // 检测方法三
        struct stat statBuf;
        int statRet = stat(file_path, &statBuf);
        if (statRet == 0)
        {
            free(file_path);
            return true;
        }
        free(file_path);
        return false;
    }

    /*
    检测文件大小
    返回 文件大小
     */
    int Tools::GetFileSize(const char* _file_path)
    {
        if (IsEmpty(_file_path))
        {
            return 0;
        }
        struct stat stat_buf;
        stat(_file_path, &stat_buf);
        int size=stat_buf.st_size;
        return size;
    }
    /**
     * 检测文件是否存在
     * 返回true  文件存在
     * 返回false 文件不存在
     * @param filePath
     * @return
     */
    bool Tools::CheckFileExist(const char* _file_path)
    {
        if (IsEmpty(_file_path))
        {
            return false;
        }
        // 检测方法一
        if (access(_file_path, R_OK) == 0)
        {
            return true;
        }
        // 检测方法二
        //int sys_access_ret = syscall(SYS_access, filePath, F_OK);
        //    long sys_access_ret = syscall(1033, filePath, F_OK);
        //    if (-1 != sys_access_ret)
        //    {
        //        return true;
        //    }

        // 检测方法三
        struct stat stat_buf;
        int stat_ret = stat(_file_path, &stat_buf);
        if (stat_ret == 0)
        {
            return true;
        }
        // 检测方法四
        char *dir_path = dirname(_file_path);
        char *file_name = basename(_file_path);
        int cd = chdir(dir_path);
        if (cd != 0)
        {
            return false;
        }
        if (access(file_name, R_OK) == 0)
        {
            return true;
        }
        chdir("/");
        return false;
    }

    /**
     * 读取文件内容
     * 返回 -1 读取失败
     * 返回 读取的内容长度
    */
    int Tools::ReadFile(const char *_file_name, char** _content)
    {
        int length = -1;
        int read_chars = 0;
        // 文件是否存在
        if (!Tools::CheckFileExist(_file_name))
        {
            return length;
        }
        // 获取文件句柄
        ifstream in(_file_name, ios::in|ios::binary);
        if(!in.is_open())
        {
            return length;
        }

        // 获取文件内容长度
        length = Tools::GetFileSize(_file_name);
        // 申请内存
        *_content = (char*)calloc(length + 1, sizeof(char));
        if (*_content == NULL)
        {
            in.close();
            return length;
        }
        // 读取文件内容
        in.getline(*_content, length);
        (*_content)[length] = '\0';
        in.close();
        return length;
    }

    // 去除字符串两端空格及回车
    void Tools::TrimString(char *line)
    {
        if (Tools::IsEmpty(line))
        {
            return;
        }

        int len = strlen(line);
        int conut = 0;
        // 去除行首的空字符或换行符
        for	(int num=0; num<len; num++)
        {
            if(line[0] != '\n' ||  line[0] != '\r' ||
               line[0] != '\t' ||  line[0] != ' ')
            {
                break;
            }
            else
            {
                for (int i = 0; i < len-conut; ++i)
                {
                    line[i]=line[i+1];
                }
                line[len-conut-1] = '\0';
                conut++;
            }
        }

        // 去除行尾的空字符或换行符
        int new_len = strlen(line);
        for	(; new_len >= 0; new_len--)
        {
            if(line[new_len-1] == '\n' ||  line[new_len-1] == '\r' ||
               line[new_len-1] == '\t' ||  line[new_len-1] == ' ')
            {
                line[new_len-1] = '\0';
            }
            else
            {
                break;
            }
        }
    }
    /*
    检测字符串是否以XXX开头
    返回值为true 说明是以字符串开头
    返回值为false 说明不是以字符串开头
     */
    bool Tools::StartWith(const char *line, const char *start)
    {
        bool flag = false;
        int lenpre = strlen(line);
        int lenstr = strlen(start);

        if( lenpre < lenstr)
        {
            return flag;
        }
        else if (strncmp(line, start, lenstr) == 0)
        {
            flag = true;
        }

        return flag;
    }

    /*
    检测字符串是否以XXX结尾
    返回值为true 说明是以字符串结尾
    返回值为false 说明不是以字符串结尾
     */
    bool Tools::EndWith(const char *line, const char *end)
    {
        int flag = false;
        int start_len = strlen(line);
        int end_len = strlen(end);

        if (start_len < end_len)
        {
            return flag;
        }

        if (end_len >= 0)
        {
            if (strncmp(line + start_len - end_len, end, end_len) == 0)
            {
                flag = true;
            }
        }
        return flag;
    }

    // 生成随机字符串
    char* Tools::RandString(int length)
    {
        char buffer[256] ;
        srand((int)time(0));
        memset(buffer, 0, 256);
        for (int i= 0; i< length;i++)
        {
            buffer[i] = 'a' + ( 0+ (int)(26.0 *rand()/(RAND_MAX + 1.0)));
        }
        buffer[strlen(buffer)] = '\0';

        char *result = (char*)calloc(strlen(buffer)+1, sizeof(char));
        memcpy(result, buffer, strlen(buffer));
        return result;
    }

    /*
    *把java字符串转化为c字符串
    *返回NULL 转化失败
    */
    char *Tools::JstringToChar(JNIEnv *env, jstring jstr)
    {
        if (env == NULL || jstr == NULL)
        {
            return NULL;
        }
        char *result = NULL;
        jclass clsstring = env->FindClass("java/lang/String");
        if (clsstring == NULL)
        {
            return NULL;
        }

        jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
        if (mid == NULL)
        {
            return NULL;
        }
        jstring coding = env->NewStringUTF("utf-8");
        if (coding == NULL)
        {
            return NULL;
        }
        jbyteArray barr_obj = (jbyteArray) env->CallObjectMethod(jstr, mid, coding);
        if (barr_obj == NULL)
        {
            return NULL;
        }
        jsize array_len = env->GetArrayLength(barr_obj);
        jbyte *pbyte_array = env->GetByteArrayElements(barr_obj, JNI_FALSE);
        if (pbyte_array == NULL)
        {
            return NULL;
        }
        if (array_len > 0)
        {
            result = (char *) calloc(array_len + 1, sizeof(char));
            memcpy(result, pbyte_array, array_len);
        }else
        {
            return NULL;
        }
        //资源清理
        env->DeleteLocalRef(clsstring);
        clsstring = NULL;

        env->DeleteLocalRef(coding);
        coding = NULL;

        env->ReleaseByteArrayElements(barr_obj, pbyte_array, 0);
        mid = NULL;
        return result;
    }
}
