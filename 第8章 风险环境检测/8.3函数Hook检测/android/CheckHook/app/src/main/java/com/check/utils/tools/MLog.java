package com.check.utils.tools;
import android.util.Log;

public class MLog {
    private static boolean DEBUG = true;
    private static final String TAG=  "AYL";

    public static void printStackTrace(String tag, Throwable thr) {
        if (DEBUG) {
           if (tag.equals(TAG)){
               Log.e(tag, thr.getMessage());
               thr.printStackTrace();
           }
           else {
               Log.e(TAG+"_"+tag, thr.getMessage());
               thr.printStackTrace();
           }
        }
    }

    public static void e(String tag, String log) {
        if (DEBUG) {
            if (tag.equals(TAG)){
                Log.e(tag, log);
            }
            else {
                Log.e(TAG+"_"+tag, log);
            }
        }
    }

    public static void d(String tag, String log) {
        if (DEBUG) {
            if (tag.equals(TAG)){
                Log.d(tag, log);
            }
            else {
                Log.d(TAG+"_"+tag, log);
            }
        }
    }

    public static void i(String tag, String log) {
        if (DEBUG) {
            if (tag.equals(TAG)){
                Log.i(tag, log);
            }
            else {
                Log.i(TAG+"_"+tag, log);
            }
        }
    }
}
