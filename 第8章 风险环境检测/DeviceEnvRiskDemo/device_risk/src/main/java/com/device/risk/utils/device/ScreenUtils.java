package com.device.risk.utils.device;

import android.content.Context;
import android.content.res.Resources;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.WindowManager;
import java.lang.reflect.Method;

public class ScreenUtils {

    /**
     * 获取屏幕分辨率宽度 *
     */
    public static int getScreenWidth() {
        return getDisplayMetrics().widthPixels;
    }

    /**
     * 获取屏幕分辨率高度 *
     */
    public static int getScreenHeigh() {
        return getDisplayMetrics().heightPixels;
    }

    public static Resources getResources() {
        return MContext.getContext().getResources();
    }

    public static DisplayMetrics getDisplayMetrics() {
        return getResources().getDisplayMetrics();
    }

    /**
     * 获取虚拟按键高度
     *
     * @return
     */
    public static int getVirtualBarHeight() {
        return getRealScreenHeigh() - getScreenHeigh();
    }

    /**
     * 获取真正屏幕高度（若有虚拟按键，会加上虚拟按键）
     *
     * @return
     */
    public static int getRealScreenHeigh() {
        int h = 0;
        WindowManager windowManager = (WindowManager) MContext.getContext().getSystemService(Context.WINDOW_SERVICE);
        Display display = windowManager.getDefaultDisplay();
        DisplayMetrics dm = new DisplayMetrics();
        try {
            Class c = Class.forName("android.view.Display");
            Method method = c.getMethod("getRealMetrics", DisplayMetrics.class);
            method.invoke(display, dm);
            h = dm.heightPixels;
        } catch (Exception e) {
            h = -1;
        }
        if (h <= 0)
            h = -1;
        return h;
    }

    /**
     * 获取真正屏幕高度（若有虚拟按键，会加上虚拟按键）
     *
     * @return
     */
    public static int getRealScreenWidth() {
        int w = 0;
        WindowManager windowManager = (WindowManager) MContext.getContext().getSystemService(Context.WINDOW_SERVICE);
        Display display = windowManager.getDefaultDisplay();
        DisplayMetrics dm = new DisplayMetrics();
        try {
            Class c = Class.forName("android.view.Display");
            Method method = c.getMethod("getRealMetrics", DisplayMetrics.class);
            method.invoke(display, dm);
            w = dm.widthPixels;
        } catch (Exception e) {
            w = -1;
        }
        if (w <= 0)
            w = -1;
        return w;
    }

}
