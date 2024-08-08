package com.test.demo;

public class JniWrapper {
    // Used to load the 'demo' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * A native method that is implemented by the 'demo' native library,
     * which is packaged with this application.
     */
    public static native String stringFromJNI();
    public static native String stringFromJNI2();
}
