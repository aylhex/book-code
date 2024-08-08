package com.ecdh.crypto;

import android.util.Log;

public class MLog {
    private static boolean DEBUG = false;
    private static String TAG = "mcrypto";

    public static void setDebuggable(boolean flag) {
        DEBUG = flag;
    }

    public static boolean isDebug() {
        return DEBUG;
    }

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
