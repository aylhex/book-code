package com.test.inlinehook;

public class NativeLib {

    // Used to load the 'inlinehook' library on application startup.
    static {
        System.loadLibrary("inlinehook");
    }

    /**
     * A native method that is implemented by the 'inlinehook' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}