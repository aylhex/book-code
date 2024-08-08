package com.device.risk.utils.tools;
import android.util.Log;

public class MLog {
    private static boolean DEBUG = true;
    private static String TAG = "AYL";

    public static void printStackTrace(Throwable thr) {
        if (DEBUG) {
           thr.printStackTrace();
        }
    }

    public static void e(String msg) {
        if (DEBUG) {
            Log.e(TAG, msg);
        }
    }

    public static void d(String log) {
        if (DEBUG) {
            Log.d(TAG, log);
        }
    }

    public static void i(String log) {
        if (DEBUG) {
           Log.i(TAG, log);
        }
    }
}
