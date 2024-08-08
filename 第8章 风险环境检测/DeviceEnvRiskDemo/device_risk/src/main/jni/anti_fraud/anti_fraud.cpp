#include <anti_fraud.h>
#include <cstring>
#include <unistd.h>
#include <tools.h>

#define MAX 128
#define HEX_VALUES "0123456789ABCDEF"

namespace devicerisk
{
    /*
    返回0 说明程序出错
    返回1 说明获取签名正常
     */
    char *CharToHex(unsigned char count, char *hex_value)
    {
        if (count < 16)
        {
            hex_value[0] = HEX_VALUES[0];
            hex_value[1] = HEX_VALUES[count];
        }
        else
        {
            int l = count / 16;
            int r = count % 16;
            hex_value[0] = HEX_VALUES[l];
            hex_value[1] = HEX_VALUES[r];
        }
        hex_value[2] = 0;
        return hex_value;
    }

    /*
    *返回NULL 说明程序出错
    *返回非NULL 说明正常获取签名SHA1
    */
    char *AntiFraud::GetSignature(JNIEnv *env, jobject _context)
    {
        if (_context == NULL)
        {
            return NULL;
        }
        // 获得Context类
        jclass cls = env->GetObjectClass(_context);
        if (cls == NULL)
        {
            return NULL;
        }
        // 得到getPackageManager方法的ID
        const char *package_manager = "getPackageManager";
        const char *package_manager_parms = "()Landroid/content/pm/PackageManager;";
        jmethodID mid = env->GetMethodID(cls, package_manager, package_manager_parms);
        if (mid == NULL)
        {
            return NULL;
        }
        // 获得应用包的管理器
        jobject pm = env->CallObjectMethod(_context, mid);
        if (pm == NULL)
        {
            return NULL;
        }
        // 得到getPackageName方法的ID
        const char *c_package_name = "getPackageName";
        const char *package_name_sign = "()Ljava/lang/String;";
        jmethodID mid_package_name = env->GetMethodID(cls, c_package_name, package_name_sign);
        if (mid_package_name == NULL)
        {
            return NULL;
        }
        // 获得当前应用包名
        jstring package_name = (jstring) env->CallObjectMethod(_context, mid_package_name);
        if (package_name == NULL)
        {
            return NULL;
        }
        // 获得PackageManager类
        jclass pm_cls = env->GetObjectClass(pm);
        if (pm_cls == NULL)
        {
            return NULL;
        }
        // 得到getPackageInfo方法的ID
        const char *c_package_info = "getPackageInfo";
        const char *package_info_sign = "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;";
        jmethodID mid_package_info = env->GetMethodID(pm_cls, c_package_info, package_info_sign);
        if (mid_package_info == NULL)
        {
            return NULL;
        }
        // 获得应用包的信息
        jobject package_info = env->CallObjectMethod(pm, mid_package_info, package_name,
                                                     0x40); //GET_SIGNATURES = 64;
        if (package_info == NULL)
        {
            return NULL;
        }
        // 获得PackageInfo 类
        jclass package_info_cls = env->GetObjectClass(package_info);
        if (package_info_cls == NULL)
        {
            return NULL;
        }
        // 获得签名数组属性的ID
        const char *c_signatures = "signatures";
        const char *c_signatures_sign = "[Landroid/content/pm/Signature;";
        jfieldID fid = env->GetFieldID(package_info_cls, c_signatures, c_signatures_sign);
        if (fid == 0)
        {
            return NULL;
        }
        // 得到签名数组
        jobjectArray signatures = (jobjectArray) env->GetObjectField(package_info, fid);
        if (signatures == NULL)
        {
            return NULL;
        }
        // 得到签名信息
        jobject signature_object = env->GetObjectArrayElement(signatures, 0);
        if (signature_object == NULL)
        {
            return NULL;
        }
        // 获得Signature类
        jclass signature_class = env->GetObjectClass(signature_object);
        if (signature_class == NULL)
        {
            return NULL;
        }

        jmethodID methodId_of_toByteArray = env->GetMethodID(signature_class, "toByteArray",
                                                             "()[B");
        if (methodId_of_toByteArray == NULL)
        {
            return NULL;
        }

        jbyteArray signature_byte = (jbyteArray) env->CallObjectMethod(signature_object,
                                                                       methodId_of_toByteArray);

        if (signature_byte == NULL)
        {
            return NULL;
        }
        // 将签名信息转化为SHA1的代码
        jclass byte_array_input_class = env->FindClass("java/io/ByteArrayInputStream");
        jmethodID methodId_of_byte_array_input_class = env->GetMethodID(byte_array_input_class,
                                                                        "<init>", "([B)V");
        jobject byte_array_input = env->NewObject(byte_array_input_class,
                                                  methodId_of_byte_array_input_class,
                                                  signature_byte);
        jclass certificate_factory_class = env->FindClass("java/security/cert/CertificateFactory");
        jmethodID methodId_of_getInstance = env->GetStaticMethodID(certificate_factory_class,
                                                                   "getInstance",
                                                                   "(Ljava/lang/String;)Ljava/security/cert/CertificateFactory;");
        jstring x_509_jstring = env->NewStringUTF("X.509");
        jobject cert_factory = env->CallStaticObjectMethod(certificate_factory_class,
                                                           methodId_of_getInstance, x_509_jstring);
        jmethodID methodId_of_generateCertificate = env->GetMethodID(certificate_factory_class,
                                                                     "generateCertificate",
                                                                     ("(Ljava/io/InputStream;)Ljava/security/cert/Certificate;"));
        jobject x509_cert = env->CallObjectMethod(cert_factory, methodId_of_generateCertificate,
                                                  byte_array_input);

        jclass x509_cert_class = env->GetObjectClass(x509_cert);
        jmethodID methodId_getEncoded = env->GetMethodID(x509_cert_class, "getEncoded", "()[B");
        jbyteArray cert_byte = (jbyteArray) env->CallObjectMethod(x509_cert, methodId_getEncoded);

        jclass message_digest_class = env->FindClass("java/security/MessageDigest");
        jmethodID methodId_getInstance = env->GetStaticMethodID(message_digest_class, "getInstance",
                                                                "(Ljava/lang/String;)Ljava/security/MessageDigest;");
        jstring sha1_jstring = env->NewStringUTF("SHA1");
        jobject sha1_digest = env->CallStaticObjectMethod(message_digest_class,
                                                          methodId_getInstance, sha1_jstring);
        jmethodID methodId_of_digest = env->GetMethodID(message_digest_class, "digest", "([B)[B");
        jbyteArray sha1_byte = (jbyteArray) env->CallObjectMethod(sha1_digest, methodId_of_digest,
                                                                  cert_byte);

        //转换成char
        jsize array_size = env->GetArrayLength(sha1_byte);
        jbyte *sha1 = env->GetByteArrayElements(sha1_byte, 0);

        // 对获取的key进程16进制转换
        char *sha1_result = (char *) calloc(array_size * 3, sizeof(char));
        if (sha1_result == NULL)
        {
            return NULL;
        }
        int index = 0;
        char temp_hex[3];
        for (int i = 0; i < array_size; i++)
        {
            CharToHex(sha1[i], temp_hex);
            sha1_result[index++] = temp_hex[0];
            sha1_result[index++] = temp_hex[1];
            if (i >= array_size - 1)
            {
                sha1_result[index++] = 0;
            }
        }

        // 释放资源
        if (cls != NULL)
        {
            env->DeleteLocalRef(cls);
            cls = NULL;
        }
        if (certificate_factory_class != NULL)
        {
            env->DeleteLocalRef(certificate_factory_class);
            certificate_factory_class = NULL;
        }
        if (x509_cert_class != NULL)
        {
            env->DeleteLocalRef(x509_cert_class);
            x509_cert_class = NULL;
        }
        if (message_digest_class != NULL)
        {
            env->DeleteLocalRef(message_digest_class);
            message_digest_class = NULL;
        }
        // 返回当前应用签名信息
        return sha1_result;

    }

    // 获取应用的包名
    char *AntiFraud::GetPackageName(JNIEnv *env, jobject context)
    {
        char *result = (char *) calloc(64, sizeof(char));

        if (context == NULL || result == NULL)
        {
            return NULL;
        }
        // 获得Context类  
        jclass cls = env->GetObjectClass(context);
        if (cls == NULL)
        {
            // 资源清理
            if (cls != NULL)
            {
                env->DeleteLocalRef(cls);
                cls = NULL;
            }
            return NULL;
        }

        // 得到getPackageManager方法的ID
        jmethodID mid = env->GetMethodID(cls, "getPackageManager","()Landroid/content/pm/PackageManager;");
        if (mid == NULL)
        {
            // 资源清理
            if (cls != NULL)
            {
                env->DeleteLocalRef(cls);
                cls = NULL;
            }
            return NULL;
        }

        // 获得应用包的管理器  
        jobject pm = env->CallObjectMethod(context, mid);
        if (pm == NULL)
        {
            // 资源清理
            if (cls != NULL)
            {
                env->DeleteLocalRef(cls);
                cls = NULL;
            }
            return NULL;
        }
        // 得到getPackageName方法的ID  
        jmethodID mid_package_name = env->GetMethodID(cls, "getPackageName","()Ljava/lang/String;");
        if (mid_package_name == NULL)
        {
            // 资源清理
            if (cls != NULL)
            {
                env->DeleteLocalRef(cls);
                cls = NULL;
            }
            return NULL;
        }
        // 获得当前应用包名  
        jstring package_name = (jstring) env->CallObjectMethod(context, mid_package_name);
        if (package_name == NULL)
        {
            goto error;
        }
        else
        {
            char *c_package_name = Tools::JstringToChar(env, package_name);
            if (c_package_name == NULL)
            {
                goto error;
            }
            strcpy(result, c_package_name);
            free(c_package_name);
        }
    error:
        // 资源清理
        if (cls != NULL)
        {
            env->DeleteLocalRef(cls);
            cls = NULL;
        }
        if (result != NULL && strlen(result) == 0)
        {
            free(result);
            return NULL;
        }
        return result;
    }

    /*
    返回值为0 说明为正常的库文件
    返回值为-1 说明为异常的库文件
    type参数  根据传入的值不同  做不同的检测

    exposed特征
    /system/framework/eddexmaker.jar
    /system/framework/eddalvikdx.jar
    /system/framework/edxp.jar
    /system/lib/libwhale.edxp.so
    /system/lib/libmemtrack_real.so
    /system/lib/libriru_edxp.so

    taichi 特征
    /data/dalvik-cache/arm/system@framework@taichi_magisk49.jar@classes.vdex
    libmedia_legacy.so
     */

    int CheckHookFramworkInfo(const char *line)
    {
        int ret = 0;
        const char *var1 = "app_process64_xposed";
        const char *var2 = "libxposed_art.so";
        const char *var3 = "de.robv.android.xposed.installer";
        const char *var4 = "XposedBridge.jar";
        const char *var5 = "com.saurik.substrate";
        const char *var6 = "libsubstrate.so";
        const char *var7 = "libAndroidCydia.cy.so";
        const char *var8 = "re.frida.server";
        const char *var9 = "eddexmaker.jar";
        const char *var10 = "eddalvikdx.jar";
        const char *var11 = "edxp.jar";
        const char *var12 = "libwhale.edxp.so";
        const char *var13 = "libmemtrack_real.so";
        const char *var14 = "libriru_edxp.so";
        const char *var15 = "taichi_magisk";
        const char *var16 = "libmedia_legacy.so";
        const char *hookframwork[16] = {var1, var2, var3, var4, var5, var6, var7, var8, var9, var10,
                                  var11, var12, var13, var14, var15, var16};
        for (int num = 0; num < 16; num++)
        {
            if (line != NULL && NULL != strcasestr(line, hookframwork[num]))
            {
                ret = -1;
                break;
            }
        }
        return ret;
    }

    int CheckInjectSoInfo(const char *line, const char *pkg)
    {
        int ret = 0;
        char *datapath = (char *) calloc(128, sizeof(char));
        char *apppath = (char *) calloc(128, sizeof(char));
        char *libpath = (char *) calloc(128, sizeof(char));
        char *data_data_format = (char *) calloc(32, sizeof(char));
        const char *data = "/data/data/";
        strcat(data_data_format, data);
        strcat(data_data_format, "%s");
        sprintf(datapath, data_data_format, pkg);
        free(data_data_format);

        char *data_path_format = (char *) calloc(32, sizeof(char));
        const char *data_app = "/data/data/";
        strcat(data_path_format, data_app);
        strcat(data_path_format, "%s");
        sprintf(apppath, data_path_format, pkg);
        free(data_path_format);

        char *app_lib_format = (char *) calloc(32, sizeof(char));
        const char *app_lib = "/data/app-lib/";
        strcat(app_lib_format, app_lib);
        strcat(app_lib_format, "%s");
        sprintf(libpath, app_lib_format, pkg);
        free(app_lib_format);


        if (Tools::StartWith(line, "/data/local/tmp"))
        {
            ret = -1;
        }
        else if (Tools::StartWith(line, "/data/data/"))
        {
            if (!Tools::StartWith(line, datapath))                //判断是不是本应用目录下的数据
            {
                ret = -1;
            }
        }//此处顺序不能颠倒 因为/data/app-lib/ 包含/data/app
        else if (Tools::StartWith(line, "/data/app-lib/"))  //判断是不是本应用目录下的数据
        {
            if (!Tools::StartWith(line, libpath))
            {
                ret = -1;
            }
        }
        else if (Tools::StartWith(line, "/data/app"))       //判断是不是本应用目录下的数据
        {
            if (!Tools::StartWith(line, apppath))
            {
                ret = -1;
            }
        }
        // 释放内存
        free(datapath);
        free(apppath);
        free(libpath);
        return ret;
    }

    int CheckHookAppInfo(const char *line, const char *pkg)
    {
        int ret = 0;
        const char *system = "/system/";
        const char *dev = "/dev/";
        const char *data_arm64 = "/data/dalvik-cache/arm64/system@framework@boot";
        const char *data_arm = "/data/dalvik-cache/arm/system@framework@boot";
        const char *data_webview = "/data/dalvik-cache/arm/system@app@GoogleWebView@GoogleWebView.apk";
        const char *vendor = "/vendor/overlay/";

        if (line == NULL && strlen(line) == 0)
        {
            return ret;
        }

        // 筛选非系统的和非自己进程的
        if (Tools::StartWith(line, system) ||
            Tools::StartWith(line, dev) ||
            Tools::StartWith(line, data_arm64) ||
            Tools::StartWith(line, data_arm) ||
            Tools::StartWith(line, vendor) ||
            Tools::StartWith(line, data_webview) ||
            strstr(line, pkg) != NULL)
        {
            return ret;
        }

        if (Tools::EndWith(line, ".dex") ||
            Tools::EndWith(line, ".odex") ||
            Tools::EndWith(line, ".apk") ||
            Tools::EndWith(line, ".vdex") ||
            Tools::EndWith(line, ".art"))
        {

            ret = -1;
        }
        return ret;
    }

    void CheckModuleFraudInfo(char *package_name, char **hookframwork_buffer, int *ret_hookframwork,
                            char **inject_so_buffer, int *ret_injectso, char **inject_dex_buffer,
                            int *ret_inject_dex)
    {
        FILE *fp = NULL;
        int count = 0;
        char command[32] = {0};
        char *line = (char *) calloc(256, sizeof(char));
        char *map_info = NULL;
        char *save_ptr = NULL;
        pid_t pid = getpid();
        char *tmp_str = (char *) calloc(256, sizeof(char));
        if (pid < 0)
        {
            const char *maps = "/proc/self/maps";
            strncpy(command, maps, strlen(maps));
        }
        else
        {
            char *format = (char *) calloc(32, sizeof(char));
            const char *proc = "/proc/";
            const char *maps = "maps";
            strcat(format, proc);
            strcat(format, "%d");
            strcat(format, maps);
            sprintf(command, format, pid);
            free(format);
        }
        fp = fopen(command, "r");
        if (fp == NULL)
        {
            return;
        }

        while (fgets(line, 255, fp) != NULL)
        {
            if (line == NULL)
            {
                continue;
            }
            strtok_r(line, " \t", &save_ptr); // 地址信息
            strtok_r(NULL, " \t", &save_ptr); // 权限信息
            strtok_r(NULL, " \t", &save_ptr); // 偏移信息
            strtok_r(NULL, " \t", &save_ptr); // 设备信息
            strtok_r(NULL, " \t", &save_ptr); // 节点信息
            map_info = strtok_r(NULL, " \t", &save_ptr); // 路径信息
            if (map_info != NULL) // 判断是否有路径信息
            {
                if (map_info[strlen(map_info) - 1] == '\n')
                {
                    //去除空格
                    if (map_info != NULL)
                    {
                        if (strlen(map_info) == 0)
                        {
                            continue;
                        }
                        else
                        {
                            //trim_string(map_info);
                        }
                    }
                    // 去除空值和重复项
                    if (strcmp(map_info, "") == 0 || strcmp(tmp_str, map_info) == 0)
                    {
                        continue;
                    }
                    // 检测hook框架
                    if (CheckHookFramworkInfo(map_info) == -1)
                    {
                        if (*ret_hookframwork >= 127)
                        {
                            continue;
                        }

                        hookframwork_buffer[*ret_hookframwork] = (char *) calloc(MAX, sizeof(char));
                        if (hookframwork_buffer[*ret_hookframwork] != NULL)
                        {
                            strncpy(hookframwork_buffer[*ret_hookframwork], map_info,
                                    strlen(map_info));
                            *ret_hookframwork += 1;
                        }
                    }
                    // 检测注入的so文件
                    if (package_name != NULL && CheckInjectSoInfo(map_info, package_name) == -1)
                    {
                        if (*ret_injectso >= 127)
                        {
                            continue;
                        }
                        inject_so_buffer[*ret_injectso] = (char *) calloc(MAX, sizeof(char));
                        if (inject_so_buffer[*ret_injectso] != NULL)
                        {
                            strncpy(inject_so_buffer[*ret_injectso], map_info, strlen(map_info));
                            *ret_injectso += 1;
                        }
                    }
                    // 检测注入的dex文件
                    if (package_name != NULL && CheckHookAppInfo(map_info, package_name) == -1)
                    {
                        if (*ret_inject_dex >= 127)
                        {
                            continue;
                        }
                        inject_dex_buffer[*ret_inject_dex] = (char *) calloc(MAX, sizeof(char));
                        if (inject_dex_buffer[*ret_inject_dex] != NULL)
                        {
                            strncpy(inject_dex_buffer[*ret_inject_dex], map_info, strlen(map_info));
                            *ret_inject_dex += 1;
                        }
                    }
                    memset(tmp_str, 0, 256);
                    strncpy(tmp_str, map_info, strlen(map_info));
                }
            }
            memset(line, 0, 256);
        }
        free(tmp_str);
        free(line);
        fclose(fp);
    }


    void AntiFraud::GetModuleFraudInfo(char *package_name, char **hookframwork, char **injectso,char **injectdex)
    {
        int hookframwork_size = 0;
        int injectso_size = 0;
        int injectdex_size = 0;

        int ret_hookframwork = 0;
        int ret_injectso = 0;
        int ret_inject_dex = 0;
        char *hookframwork_buffer[128];
        char *inject_so_buffer[128];
        char *inject_dex_buffer[128];

        CheckModuleFraudInfo(package_name, hookframwork_buffer, &ret_hookframwork,
                                inject_so_buffer, &ret_injectso, inject_dex_buffer,
                                &ret_inject_dex);
        // 计算hookframwork_buffer的长度
        for (size_t i = 0; i < ret_hookframwork; i++)
        {
            if (hookframwork_buffer[i] != NULL)
            {
                hookframwork_size += strlen(hookframwork_buffer[i]) + 1;
            }
        }
        // 将返回数据拼接到hookframwork
        *hookframwork = (char *) calloc(hookframwork_size + 1, sizeof(char));
        for (size_t i = 0; i < ret_hookframwork; i++)
        {
            if (hookframwork_buffer[i] != NULL &&
                strstr(*hookframwork, hookframwork_buffer[i]) == NULL)
            {
                strcat(*hookframwork, hookframwork_buffer[i]);
                strcat(*hookframwork, "#");
            }
            free(hookframwork_buffer[i]);
        }
        // 计算inject_so_buffer的长度
        for (size_t i = 0; i < ret_injectso; i++)
        {
            if (inject_so_buffer[i] != NULL)
            {
                injectso_size += strlen(inject_so_buffer[i]) + 1;
            }
        }
        // 将返回数据拼接到的injectso
        *injectso = (char *) calloc(injectso_size + 1, sizeof(char));
        for (size_t i = 0; i < ret_injectso; i++)
        {
            if (inject_so_buffer[i] != NULL && strstr(*injectso, inject_so_buffer[i]) == NULL)
            {
                strcat(*injectso, inject_so_buffer[i]);
                strcat(*injectso, "#");
            }
            free(inject_so_buffer[i]);
        }
        // 计算injectdex_size
        for (size_t i = 0; i < ret_inject_dex; i++)
        {
            if (inject_dex_buffer[i] != NULL)
            {
                injectdex_size += strlen(inject_dex_buffer[i]) + 1;
            }
        }
        // 将返回的数据拼接到injectdex
        *injectdex = (char *) calloc(injectdex_size + 1, sizeof(char));
        for (size_t i = 0; i < ret_inject_dex; i++)
        {
            if (inject_dex_buffer[i] != NULL && strstr(*injectdex, inject_dex_buffer[i]) == NULL)
            {
                strcat(*injectdex, inject_dex_buffer[i]);
                strcat(*injectdex, "#");
            }
            free(inject_dex_buffer[i]);
        }
    }

    // 检测SELinux状态  然后返回
    char *AntiFraud::GetSelinuxState()
    {
        FILE *fp = popen("getenforce", "r");
        if (fp == NULL)
        {
            return NULL;
        }
        char *buffer = (char *) calloc(24, sizeof(char));
        if (buffer == NULL)
        {
            return NULL;
        }
        fgets(buffer, 23, fp);
        pclose(fp);
        // 防止读取其它无效值check_magisk
        if (strlen(buffer) > 0)
        {
            //trim_string(buffer);
            return buffer;
        }
        else
        {
            return NULL;
        }
    }

    /*
    用于检测magisk框架
    ret 返回检测到的特征数
    */
    int CheckMagisk(char **buffer)
    {
        FILE *fp = NULL;
        int ret = 0;
        char *pread_info = (char *) calloc(26, sizeof(char));
        if (pread_info == NULL)
        {
            return ret;
        }
        const char *cmd = "magisk --list";
        const char *flag = "not found";
        if ((fp = popen(cmd, "r")) != NULL)
        {
            while (fgets(pread_info, 25, fp) != NULL)
            {
                if (strlen(pread_info) != 0)
                {
                    Tools::TrimString(pread_info);
                }
                else
                {
                    continue;
                }

                if (strstr(pread_info, flag) != NULL)
                {
                    goto end;
                }
                // 不要忘记释放内存
                buffer[ret] = (char *) calloc(26, sizeof(char));
                if (NULL != buffer[ret])
                {
                    strncpy(buffer[ret], pread_info, strlen(pread_info));
                    ret++;
                }
                memset(pread_info, 0, 26);
                if (ret >= 127)
                {
                    goto end;
                }
            }

        }
    end:
        if (fp != NULL)
        {
            pclose(fp);
        }
        free(pread_info);
        return ret;
    }

    /*
     * 获取magisk的命令信息
    */
    char *AntiFraud::GetMagiskInfo()
    {
        int cmd_count = 0;
        char *buffer = NULL;
        int cmd_length = 0;
        char *cmd_names[128];
        cmd_count = CheckMagisk(cmd_names);
        if (0 == cmd_count)
        {
            return NULL;
        }

        for (size_t i = 0; i < cmd_count; i++)
        {
            //计算连接符#的长度
            if (cmd_names[i] != NULL)
            {
                cmd_length += strlen(cmd_names[i]) + 1;
            }
        }
        buffer = (char *) calloc(cmd_length + 1, sizeof(char));
        if (buffer == NULL)
        {
            for (size_t i = 0; i < cmd_count; i++)
            {
                // 释放子函数中申请的内存
                free(cmd_names[i]);
            }
            return NULL;
        }

        for (size_t i = 0; i < cmd_count; i++)
        {
            if (cmd_names[i] != NULL && strstr(buffer, cmd_names[i]) == NULL)
            {
                strcat(buffer, cmd_names[i]);
                strcat(buffer, "#");
            }
            // 释放子函数中申请的内存
            free(cmd_names[i]);
        }
        return buffer;
    }
}