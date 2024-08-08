package com.device.risk.utils.tools;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;

import androidx.core.content.ContextCompat;

public class PermUtils {
    public static boolean RW_EXTERNAL_STORAGE = false;
    public static boolean READ_PHONE_STATE = false;
    public static boolean ACCESS_WIFI_STATE = false;
    public static boolean ACCESS_NETWORK_STATE = false;

    /**
     * 检查是否已经获取到指定权限
     * @param context
     * @param permission
     * @return
     */
    public static boolean checkPermission(Context context, String permission) {
        int permissionCheck = ContextCompat.checkSelfPermission(context, permission);
        return permissionCheck == PackageManager.PERMISSION_GRANTED;
    }

    /**
     * 检测当前SDK获取的权限
     * @param context
     * @return
     */
    public static int checkCurrentPermission(Context context) {

        try {
            if (checkPermission(context, Manifest.permission.WRITE_EXTERNAL_STORAGE)&&
                    checkPermission(context, Manifest.permission.READ_EXTERNAL_STORAGE)){
                RW_EXTERNAL_STORAGE = true;
            }

            if (checkPermission(context, Manifest.permission.ACCESS_WIFI_STATE)){
                ACCESS_WIFI_STATE = true;
            }

            if (checkPermission(context, Manifest.permission.ACCESS_NETWORK_STATE)){
                ACCESS_NETWORK_STATE = true;
            }

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q){
                READ_PHONE_STATE = false;
            }else if (checkPermission(context, Manifest.permission.READ_PHONE_STATE)) {
                READ_PHONE_STATE = true;
            }

        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        return 0;
    }

}
