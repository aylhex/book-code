package com.device.risk.utils.device;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;

import java.io.File;

public class MContext {
    private static Context sContext;

    public static void init(Context context) {
        sContext = context;
    }

    public static Context getContext() {
        return sContext;
    }


    public static String getVirtualAppFiles(Context mConttext) {
        try {
            File mFile = mConttext.getFilesDir();
            String result = mFile.getPath();
            return result;
        } catch (Exception ignore) {
        }
        return "Error";
    }
}
