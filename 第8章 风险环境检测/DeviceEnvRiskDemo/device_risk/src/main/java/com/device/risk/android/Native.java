package com.device.risk.android;

import com.device.risk.utils.tools.MLog;

public class Native {

    static {
        try {
            System.loadLibrary("native-lib");
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }

        try {
            System.loadLibrary("c++_shared");
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
    }

    public static native byte[] doCommand(byte[] originalData);
}
