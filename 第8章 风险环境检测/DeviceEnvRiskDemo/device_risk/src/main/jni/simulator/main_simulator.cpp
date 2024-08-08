#include <main_simulator.h>
#include <anti_simulator.h>

#define DEFAULT "6666"

namespace devicerisk
{
    /*
    获取模拟器属性特征列表
    返回值为0  说明 没有检测到模拟器属性
    返回值为非0  说明检测到模拟器属性
     */
    int AddVmPropertysToJson(Document &_json)
    {
        int count = 0;
        int ret_size = 0;
        char *gpnames[31];
        char *buffer = NULL;

        count = AntiSimulator::CheckProperty(gpnames, &ret_size);
        //没有检测到特征属性
        if (0 == count)
        {
            MJson::AddString(_json, "VMProperty", DEFAULT);
            return count;
        }

        buffer = (char *) calloc(ret_size + count + 2, sizeof(char));
        if (buffer == NULL)
        {
            return 0;
        }

        for (size_t i = 0; i < count; i++)
        {
            strcat(buffer, gpnames[i]);
            strcat(buffer, "#");
            free(gpnames[i]);
        }

        if (count > 0)
        {
            MJson::AddString(_json, "VMProperty", buffer);
        }
        else
        {
            MJson::AddString(_json, "VMProperty", DEFAULT);
        }

        free(buffer);
        return count;
    }


    /*
    检测指定模拟器文件是否存在,返回检测到的文件个数
    调用函数负责释放申请的内存
    返回值为0 说明不存在模拟器特征文件
    正常返回值 为检测到存在的文件个数
     */
    int AddVmFilesToJson(Document &_json)
    {
        int ret_bin_size = 0;
        int ret_dev_size = 0;
        int ret_lib_size = 0;
        char *bin_buf[7];
        char *dev_buf[5];
        char *lib_buf[8];
        int bin_count = AntiSimulator::CheckBinVmFileExist(bin_buf, &ret_bin_size);
        int dev_count = AntiSimulator::CheckDevVmFileExist(dev_buf, &ret_dev_size);
        int lib_count = AntiSimulator::CheckLibVmFileExist(lib_buf, &ret_lib_size);

        int all_count = bin_count + dev_count + lib_count;
        int all_size = ret_bin_size + ret_dev_size + ret_lib_size;

        int tmp_count = 0;
        char *result = (char *) calloc(all_size + all_count + 2, sizeof(char));
        if (result == NULL)
        {
            MJson::AddString(_json, "VMFile", DEFAULT);
            return 0;
        }

        // 将检测出的特殊文件返回
        for (size_t num = 0; num < bin_count; num++)
        {
            strcat(result, bin_buf[num]);
            strcat(result, "#");
            tmp_count++;
        }


        for (size_t num = 0; num < dev_count; num++)
        {
            strcat(result, dev_buf[num]);
            strcat(result, "#");
            tmp_count++;
        }


        for (size_t num = 0; num < lib_count; num++)
        {
            strcat(result, lib_buf[num]);
            strcat(result, "#");
            tmp_count++;
        }
        MJson::AddString(_json, "VMFile", result);
        free(result);
        return tmp_count;
    }


    /*
    获取模拟器文件列表
    返回值为0  说明 没有检测文件
    返回值为非0  说明检测到模拟器文件个数
    */
    int AddRootDirVmSpecialsFile(Document &_json)
    {
        char *specialnames[512];
        char *buffer = NULL;
        int ret_size = 0;
        int file_count = AntiSimulator::CheckRootDirSpecialFile(specialnames, &ret_size);

        if (0 == file_count)
        {
            MJson::AddString(_json, "VMSpecialFile", DEFAULT);
            return 0;
        }

        buffer = (char *) calloc(ret_size + file_count + 2, sizeof(char));
        if (buffer == NULL)
        {
            return 0;
        }

        for (size_t i = 0; i < file_count; i++)
        {
            strcat(buffer, specialnames[i]);
            strcat(buffer, "#");
            free(specialnames[i]);
        }

        if (file_count > 0)
        {
            MJson::AddString(_json, "VMSpecialFile", buffer);
        }
        else
        {
            MJson::AddString(_json, "VMSpecialFile", DEFAULT);
        }

        free(buffer);
        return file_count;
    }


    /*
    获取指定特征的进行信息
    ret 返回检测的数量
    */
    int AddExtendVmFeaturesToJson(Document &_json)
    {
        int proc_count = 0;
        int count = 0;
        int proc_length = 0;
        char *buffer = NULL;
        char *proc_names[128];
        proc_count = AntiSimulator::CheckVmProcessExist(proc_names);

        if (0 == proc_count)
        {
            MJson::AddString(_json, "VMProc", DEFAULT);
            return proc_count;
        }

        while (count < proc_count)
        {
            //+1 计算连接符#的长度
            proc_length += strlen(proc_names[count]) + 2;
            count++;
        }

        buffer = static_cast<char *>(calloc(proc_length + 1, sizeof(char)));
        if (buffer == NULL)
        {
            // 重置计数器 内存分配失败以后同样需要释放之前申请的内存
            count = 0;
            while (count < proc_count)
            {
                // 释放子函数中申请的内存
                free(proc_names[count]);
                count++;
            }
            return 0;
        }

        // 重置计数器
        count = 0;
        while (count < proc_count)
        {
            // 判断是不是有效值
            if (proc_names[count] != NULL && strlen(proc_names[count]) > 0)
            {
                //trim_string(proc_names[count]);
            }
            else
            {
                // 释放子函数中申请的内存
                free(proc_names[count]);
                count++;
                continue;
            }

            if (strstr(buffer, proc_names[count]) == NULL)
            {
                strcat(buffer, proc_names[count]);
                strcat(buffer, "#");
            }
            // 释放子函数中申请的内存
            free(proc_names[count]);
            count++;
        }

        if (count > 0 && buffer != NULL)
        {
            MJson::AddString(_json, "VMProc", buffer);
        }
        else
        {
            MJson::AddString(_json, "VMProc", DEFAULT);
        }
        free(buffer);
        return count;
    }

    /*
    获取设备模拟器信息
    返回值为0 说明没有信息
    返回值为非0 说明获取到模拟器信息
     */

    int MainSimulator::AddSimulatorDataToJson(Document &_json)
    {
        //获取模拟器文件列表
        int vmspecial_count = AddRootDirVmSpecialsFile(_json);
        //获取模拟器属性列表
        int vmpropertys_count = AddVmPropertysToJson(_json);
        //获取模拟器特征文件列表
        int vmfiles_count = AddVmFilesToJson(_json);
        //获取模拟进程相关信息
        int vmfeatures_count = AddExtendVmFeaturesToJson(_json);

        return vmspecial_count + vmpropertys_count + vmfiles_count + vmfeatures_count;
    }
}


