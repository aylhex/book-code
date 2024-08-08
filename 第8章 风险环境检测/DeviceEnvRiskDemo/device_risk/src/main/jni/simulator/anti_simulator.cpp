#include <anti_simulator.h>
#include <cstring>
#include <dirent.h>
#include <tools.h>

#define MIN 128

namespace devicerisk
{
    int AntiSimulator::CheckFileByCommand(const char *_command, const char **_in, int _in_len, char **_out)
    {
        int count = 0;
        int MAX = 128;
        FILE *fp = popen(_command, "r");
        if (NULL == fp)
        {
            return count;
        }

        char *tm_buf = static_cast<char *>(calloc(MAX + 1, sizeof(char)));
        if (tm_buf == NULL)
        {
            return count;
        }

        while (fgets(tm_buf, MAX * sizeof(char), fp) != NULL)
        {
            //检测
            for (int num = 0; num < _in_len; num++)
            {
                if (NULL == strstr(tm_buf, _in[num]))
                {
                    memset(tm_buf, 0, MAX * sizeof(char));
                    continue;
                }

                _out[count] = static_cast<char *>(calloc(MAX, sizeof(char)));
                if (NULL == _out[count])
                {
                    //继续下一次 循环 防止出现失败
                    continue;
                }
                strcpy(_out[count], _in[num]);
                count++;
                // 防止数组越界
                if (count >= 30)
                {
                    goto end;
                }
                break;

            }
        }
end:
        free(tm_buf);
        pclose(fp);
        return count;
    }

    int AntiSimulator::CheckFileByOpendir(const char *_path, const char **_in, int _in_len, char **_out)
    {
        //创建一个DIR结构的指针
        int ret = 0;
        DIR *dp;
        struct dirent *dirp;
        if ((dp = opendir(_path)) == NULL)
        {
            return ret;
        }

        while ((dirp = readdir(dp)) != NULL)
        {
            for (int num = 0; num < _in_len; num++)
            {
                if (_in[num] == NULL || strstr(dirp->d_name, _in[num]) == NULL)
                {
                    continue;
                }


                _out[ret] = static_cast<char *>(calloc(strlen(dirp->d_name) + 1, sizeof(char)));
                if (NULL == _out[ret])
                {
                    continue;
                }
                strcpy(_out[ret], dirp->d_name);
                ret++;
                // 防止数组越界
                if (ret >= _in_len)
                {
                    goto end;
                }
                break;
            }
        }
end:
        // 关闭获取
        closedir(dp);
        return ret;
    }

    /**
     * 检测/system/bin/的模拟器特征文件
     */
    int AntiSimulator::CheckBinVmFileExist(char **_buffer, int *_ret_size)
    {
        int count = 0;
        const char *path = "/system/bin/";
        const char *androVM = "androVM-prop";
        const char *microvirt = "microvirt-prop";
        const char *windroyed = "windroyed";
        const char *microvirtd = "microvirtd";
        const char *nox = "nox-prop";
        const char *ttVM = "ttVM-prop";
        const char *droid4x = "droid4x-prop";
        const char *noxd = "noxd";
        const char *vbox = "nox-vbox-sf";
        const char *mount = "mount.vboxsf";
        const char *binfile[10] = {androVM, microvirt, windroyed, microvirtd, nox, ttVM, droid4x, noxd,
                             vbox, mount};

        char *out[10];
        int ret_count = CheckFileByOpendir(path, binfile, 10, out);

        if (ret_count == 0) {
            for (int num = 0; num < 10; num++)
            {
                // 防止数组越界
                if (ret_count >= 10)
                {
                    break;
                }

                if (!Tools::CheckFileExistByName(path, binfile[num]))
                {
                    continue;
                }
                // 文件存在
                out[num] = static_cast<char *>(calloc(strlen(binfile[num]) + 1, sizeof(char)));
                if (NULL == out[num])
                {
                    continue;
                }
                strcpy(out[num], binfile[num]);
                ret_count++;
                break;
            }
        }

        if (0 == ret_count) {
            return ret_count;
        }

        for (int num = 0; num < ret_count; num++)
        {
            _buffer[count] = static_cast<char *>(calloc(28, sizeof(char)));
            if (_buffer[count] != NULL)
            {
                memcpy(_buffer[count], out[num], strlen(out[num]));
                *_ret_size += atoi(out[num]);
                count++;
            }
            free(out[num]);
        }
        return count;
    }

    /**
     * 检测/dev/目录下的模拟器特征文件
     */
    int AntiSimulator::CheckDevVmFileExist(char **_buffer, int *_ret_size)
    {
        int count = 0;
        const char *path = "/dev/";
        const char *vboxguest = "vboxguest";
        const char *vboxuser = "vboxuser";
        const char *vboxpci = "vboxpci";
        const char *nemuguest = "nemuguest";
        const char *nemuuser = "nemuuser";
        const char *devfile[5] = {vboxguest, vboxuser, vboxpci, nemuguest, nemuuser};

        char *out[5];
        int ret_count = CheckFileByOpendir(path, devfile, 5, out);

        if (ret_count == 0)
        {
            for (int num = 0; num < 5; num++)
            {
                // 防止数组越界
                if (ret_count >= 5)
                {
                    break;
                }

                if (!Tools::CheckFileExistByName(path, devfile[num]))
                {
                    continue;
                }
                // 文件存在
                out[num] = static_cast<char *>(calloc(strlen(devfile[num]) + 1, sizeof(char)));
                if (NULL == out[num])
                {
                    continue;
                }
                strcpy(out[num], devfile[num]);
                ret_count++;
                break;
            }
        }


        if (0 == ret_count)
        {
            return ret_count;
        }

        for (int num = 0; num < ret_count; num++)
        {
            _buffer[count] = static_cast<char *>(calloc(28, sizeof(char)));
            if (_buffer[count] != NULL)
            {
                memcpy(_buffer[count], out[num], strlen(out[num]));
                *_ret_size += atoi(out[num]);
                count++;
            }
            free(out[num]);
        }
        return count;
    }

    /**
     * 检测/system/lib/ 目录下的模拟器特征文件
     */
    int AntiSimulator::CheckLibVmFileExist(char **_buffer, int *_ret_size)
    {
        int count = 0;
        const char *path = "/system/lib/";
        const char *droid = "libdroid4x.so";
        const char *vboxguest = "vboxguest.ko";
        const char *vboxsf = "vboxsf.ko";
        const char *vboxvideo = "vboxvideo.ko";
        const char *nemuguest = "nemuguest.ko";
        const char *nemusf = "nemusf.ko";
        const char *nemuvideo = "nemuvideo.ko";
        const char *noxspeedup = "libnoxspeedup.so";
        const char *noxd = "libnoxd.so";
        const char *libfile[9] = {droid, vboxguest, vboxsf, vboxvideo, nemuguest, nemusf, nemuvideo,
                            noxspeedup, noxd};

        char *out[9];
        int ret_count = CheckFileByOpendir(path, libfile, 9, out);


        if (ret_count == 0)
        {
            for (int num = 0; num < 9; num++)
            {
                // 防止数组越界
                if (ret_count >= 9)
                {
                    break;
                }

                if (!Tools::CheckFileExistByName(path, libfile[num]))
                {
                    continue;
                }
                // 文件存在
                out[num] = static_cast<char *>(calloc(strlen(libfile[num]) + 1, sizeof(char)));
                if (NULL == out[num])
                {
                    continue;
                }
                strcpy(out[num], libfile[num]);
                ret_count++;
                break;
            }
        }

        if (0 == ret_count)
        {
            return ret_count;
        }

        for (int num = 0; num < ret_count; num++)
        {
            _buffer[count] = static_cast<char *>(calloc(28, sizeof(char)));
            if (_buffer[count] != NULL)
            {
                memcpy(_buffer[count], out[num], strlen(out[num]));
                *_ret_size += atoi(out[num]);
                count++;
            }
            free(out[num]);
        }
        return count;
    }

    /*
    检测属性值中是否含有模拟器特征
    正常返回属性文件格式
    返回值为0 未获取模拟器特征
    返回值非0 取模拟器特征
    */
    int AntiSimulator::CheckProperty(char **_buffer, int *_ret_size)
    {
        int count = 0;
        const char* command = "getprop";
        const char *var1 = "[andy.";
        const char *var2 = ".andy.";
        const char *var3 = "[andy]";
        const char *var4 = "[andy_vm_custom";
        const char *var5 = "qemu.sf.fake_camera";
        const char *var6 = ".ttvmd.";
        const char *var7 = ".ttVM.";
        const char *var8 = "[ttVM.";
        const char *var9 = "[ttvmd.";
        const char *var10 = "[microvirtd.";
        const char *var11 = "[microvirt.";
        const char *var12 = ".microvirt.";
        const char *var13 = ".microvirtd.";
        const char *var14 = "ro.simulated.phone";
        const char *var15 = ".simulated.";
        const char *var16 = "virtualbox";
        const char *var17 = "[vbox86";
        const char *var18 = "[nox.";
        const char *var19 = ".nox.";
        const char *var20 = ".noxd]";
        const char *var21 = "[droid4x.";
        const char *var22 = ".droid4x.";
        const char *var23 = "[Droid4X";
        const char *var24 = "[windroyed.";
        const char *var25 = ".windroyed.";
        const char *var26 = "[windroyed]";
        const char *var27 = "[windroy.";
        const char *var28 = ".windroy.";
        const char *var29 = "[windroy]";
        const char *var30 = "[nemud.";

        const char *gplist[30] = {var1, var2, var3, var4, var5, var6, var7, var8, var9, var10,
                            var11, var12, var13, var14, var15, var16, var17, var18, var19, var20,
                            var21, var22, var23, var24, var25, var26, var27, var28, var29, var30};

        char *out[30];
        count = CheckFileByCommand(command, gplist, 30, out);

        // 将检测出的特殊文件返回
        for (size_t num = 0; num < count; num++)
        {
            _buffer[num] = (char *) calloc(MIN, sizeof(char));
            strcpy(_buffer[num], out[num]);
            *_ret_size += strlen(out[num]);
            free(out[num]);
        }

        return count;
    }

    /*
    检测根目录下的文件
    正常返回文件的个数
    返回0 说明文件获取失败或者没有文件
     */
    int AntiSimulator::CheckRootDirSpecialFile(char **_buffer, int *_ret_size)
    {
        const char *andy = ".andy.";
        const char *nox = ".nox.";
        const char *ttvm = ".ttVM_x86.";
        const char *android = ".android_x86";
        const char *x86 = ".x86.";
        const char *vbox = ".vbox86.";

        const char *rootfile[6] = {andy, nox, ttvm, android, x86, vbox};
        char *out[6];
        int count = CheckFileByOpendir("/", rootfile, 6, out);

        // 将检测出的特殊文件返回
        for (size_t num = 0; num < count; num++)
        {
            _buffer[num] = static_cast<char *>(calloc(MIN, sizeof(char)));
            strcpy(_buffer[num], out[num]);
            *_ret_size += strlen(out[num]);
            free(out[num]);
        }
        return count;
    }

    /*
     查看模拟器中特有的进程   解决夜神模拟器最新版检测失败的问题
     或者 magisk相关的进程
     ret 返回检测到的特征数
     */
    int AntiSimulator::CheckVmProcessExist(char **_buffer)
    {
        FILE *fp = NULL;
        FILE *fpe = NULL;
        FILE *fp_bin = NULL;
        int ret = 0;
        char *pfile_info = static_cast<char *>(calloc(128, sizeof(char)));

        char *pstr_item = NULL;
        char *psave_ptr = NULL;
        const char *proc_nox = "nox";
        const char *proc_magisk = "magisk";
        const char *proc_vphone = "vphone";
        const char *proc_xposed = "xposed";
        const char *ps = "ps";
        const char *ps_e = "ps -e";
        const char *path_cmd = "ls /proc/self/cwd/system/bin";
        const char *busybox = "busybox";

        if ((fp_bin = popen(path_cmd, "r")) != NULL)
        {
            while (fgets(pfile_info, 127, fp_bin) != NULL)
            {
                if (strstr(pfile_info, proc_nox) != NULL || strstr(pfile_info, busybox) != NULL)
                {
                    // 不要忘记释放内存
                    _buffer[ret] = static_cast<char *>(calloc(MIN, sizeof(char)));
                    if (NULL != _buffer[ret])
                    {
                        strncpy(_buffer[ret], pfile_info, strlen(pfile_info));
                        memset(pfile_info, 0, sizeof(char) * 128);
                        ret++;
                    }
                    else
                    {
                        memset(pfile_info, 0, sizeof(char) * 128);
                    }

                    // 防止buffer出现数组越界
                    if (ret >= 127)
                    {
                        free(pfile_info);
                        pclose(fp_bin);
                        return ret;
                    }
                }

            }
            // 关闭防止 野指针 内存泄露
            pclose(fp_bin);
        }

        // 使用ps命令
        if ((fp = popen(ps, "r")) != NULL)
        {
            memset(pfile_info, 0, sizeof(char) * 128);
            while (fgets(pfile_info, 127, fp) != NULL)
            {
                if (strstr(pfile_info, proc_nox) != NULL ||
                    strstr(pfile_info, proc_vphone) != NULL ||
                    strstr(pfile_info, proc_magisk) != NULL ||
                    strstr(pfile_info, proc_xposed) != NULL)
                {
                    // 提取指定的进程信息
                    strtok_r(pfile_info, " \t", &psave_ptr);        // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    pstr_item = strtok_r(NULL, " \t", &psave_ptr);  // 进程信息

                    if (NULL != pstr_item)
                    {
                        // 不要忘记释放内存
                        _buffer[ret] = static_cast<char *>(calloc(MIN, sizeof(char)));
                        if (NULL != _buffer[ret])
                        {
                            strncpy(_buffer[ret], pstr_item, strlen(pstr_item));
                            memset(pfile_info, 0, sizeof(char) * 128);
                            ret++;
                        }
                        else
                        {
                            memset(pfile_info, 0, sizeof(char) * 128);
                        }

                        // 防止buffer出现数组越界
                        if (ret >= 127)
                        {
                            free(pfile_info);
                            pclose(fp);
                            return ret;
                        }
                    }

                }
            }
            // 关闭防止 野指针 内存泄露
            pclose(fp);
        }

        // 使用ps -e命令
        if ((fpe = popen(ps_e, "r")) != NULL)
        {
            memset(pfile_info, 0, sizeof(char) * 128);
            while (fgets(pfile_info, 127, fp) != NULL)
            {
                if (strstr(pfile_info, proc_nox) != NULL ||
                    strstr(pfile_info, proc_vphone) != NULL ||
                    strstr(pfile_info, proc_magisk) != NULL ||
                    strstr(pfile_info, proc_xposed) != NULL)
                {
                    // 提取指定的进程信息
                    strtok_r(pfile_info, " \t", &psave_ptr);        // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    strtok_r(NULL, " \t", &psave_ptr);              // 节点信息
                    pstr_item = strtok_r(NULL, " \t", &psave_ptr);  // 进程信息

                    // 不要忘记释放内存
                    if (NULL != pstr_item)
                    {
                        _buffer[ret] = static_cast<char *>(calloc(MIN, sizeof(char)));
                        if (NULL != _buffer[ret])
                        {
                            strncpy(_buffer[ret], pstr_item, strlen(pstr_item));
                            memset(pfile_info, 0, sizeof(char) * 128);
                            ret++;
                        }
                        else
                        {
                            memset(pfile_info, 0, sizeof(char) * 128);
                            continue;
                        }
                        // 防止buffer出现数组越界
                        if (ret >= 127)
                        {
                            free(pfile_info);
                            pclose(fpe);
                            return ret;
                        }
                    }
                }
            }
            // 关闭防止 野指针 内存泄露
            pclose(fpe);
        }
        free(pfile_info);
        return ret;
    }
}