package com.check.utils.tools;

import android.content.Context;

public class MContext {
    private static Context mContext;

    public static void init(Context context) {
        mContext = context;
    }

    public static Context getContext() {
        return mContext;
    }

}
