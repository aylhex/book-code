package com.device.risk.android;

import android.content.Context;
import com.device.risk.utils.device.Utils;

public class Device {

    public static String collect(Context context){
        try {
            return Utils.getDeviceInfos(context);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "Error";
    }
}
