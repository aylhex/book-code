package com.test.jsbridge;

import android.content.Context;

public class MContext {
    private static Context sContext = null;

    public static void init(Context context) {
        if (sContext == null){
            sContext = context;
        }
    }

    public static Context getContext() {
        return sContext;
    }

}
