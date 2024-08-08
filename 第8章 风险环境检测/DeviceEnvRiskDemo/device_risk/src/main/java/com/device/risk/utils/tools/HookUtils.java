package com.device.risk.utils.tools;

import android.content.Context;

import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

public class HookUtils {

    static StringBuilder mStringBuilder = new StringBuilder();
    /**
     * 检测设备的hook信息
     *
     * @param mClassLoader
     * @return
     */
    public static int checkXpoesd(ClassLoader mClassLoader, Context mContext) {

        Object mObjectXposedHelpers = null;
        //清空防止上次数据干扰
        mStringBuilder = new StringBuilder();

        byte mbyte = 0;

        try {
            // de.robv.android.xposed.XposedHelpers
            mObjectXposedHelpers = mClassLoader.loadClass("de.robv.android.xposed.XposedHelpers").newInstance();
        } catch (Throwable e) {
            //MLog.printStackTrace(e);
        }

        try {
            // fieldCache
            int flag_v3 = CheckInject(mObjectXposedHelpers, "fieldCache");
            if (flag_v3 != 0) {
                return flag_v3;
            }
            // methodCache
            int flag_v4 = CheckInject(mObjectXposedHelpers, "methodCache");
            if (flag_v4 != 0) {
                return flag_v4;
            }
            // constructorCache
            int flag_v22 = CheckInject(mObjectXposedHelpers, "constructorCache");
            if (flag_v22 != 0) {
                return flag_v22;
            }
        } catch (Throwable e) {

        }
        return mbyte;
    }

    /**
     * 检测设备的hook信息
     *
     * @param mObjectXposedHelpers
     * @param type
     * @return
     */
    private static int CheckInject(Object mObjectXposedHelpers, String type) {
        int flag = 0;
        String info = "";
        Set mSet = null;

        try {
            Field mField = mObjectXposedHelpers.getClass().getDeclaredField(type);
            mField.setAccessible(true);
            HashMap mHashMap = (HashMap) mField.get(mObjectXposedHelpers);
            mSet = mHashMap.keySet();

        } catch (Throwable e) {
            mSet = null;
            //MLog.printStackTrace(e);
        }

        if (mSet != null && !mSet.isEmpty()) {
            //MLog.i("Hook ==> test");
            try {
                Iterator mIterator = mSet.iterator();
                while (mIterator.hasNext()) {
                    info = mIterator.next().toString().toLowerCase();

                    //MLog.i("Hook ==> " + info);

                    mStringBuilder.append(info.split("#exact")[0] + "$$");
                    if (flag == 0 || flag == 3) {
                        flag = flag + 1;
                    }
                }
            } catch (Throwable e) {
                //MLog.printStackTrace(e);
            }
        }
        return flag;
    }

    /**
     * 检测设备的hook信息
     *
     * @param mClassLoader
     * @param checktype
     * @return
     */
    public static int checkXpoesdFake(ClassLoader mClassLoader, int checktype, Context mContext) {

        Object mObjectXposedHelpers = null;
        //清空防止上次数据干扰
        mStringBuilder = new StringBuilder();

        byte mbyte = 0;

        try {
            // de.robv.android.xposed.XposedHelpers
            mObjectXposedHelpers = mClassLoader.loadClass("de.robv.android.xposed.XposedHelpers").newInstance();
        } catch (Throwable e) {

        }

        try {
            // fieldCache
            int flag_v3 = CheckFakeState(mObjectXposedHelpers, "fieldCache", checktype);
            if (flag_v3 != 0) {
                return flag_v3;
            }
            // methodCache
            int flag_v4 = CheckFakeState(mObjectXposedHelpers, "methodCache", checktype);
            if (flag_v4 != 0) {
                return flag_v4;
            }
            // constructorCache
            int flag_v22 = CheckFakeState(mObjectXposedHelpers, "constructorCache", checktype);
            if (flag_v22 != 0) {
                return flag_v22;
            }
        } catch (Throwable e) {

        }

        return mbyte;
    }

    /**
     * 检测设备的hook信息
     *
     * @param mObjectXposedHelpers
     * @param type
     * @param checktype
     * @return
     */
    private static int CheckFakeState(Object mObjectXposedHelpers, String type, int checktype) {
        int flag = 0;
        Set mSet = null;

        try {
            Field mField = mObjectXposedHelpers.getClass().getDeclaredField(type);
            mField.setAccessible(true);
            HashMap mHashMap = (HashMap) mField.get(mObjectXposedHelpers);
            mSet = mHashMap.keySet();

        } catch (Throwable e) {
            mSet = null;
        }

        if (mSet != null && !mSet.isEmpty()) {
            try {
                Iterator mIterator = mSet.iterator();

                switch (checktype) {
                    case 1:
                        String[] funcnames = {"getCellLocation", "getLastLocation"};
                        while (mIterator.hasNext()) {
                            String info = mIterator.next().toString().toLowerCase();
                            for (int num = 0; num < funcnames.length; num++) {
                                if (info.contains(funcnames[num].toLowerCase())) {
                                    flag = 1;
                                }
                            }
                        }
                        break;
                    case 2:
                        while (mIterator.hasNext()) {
                            String info = mIterator.next().toString().toLowerCase();

                            if (info.contains("getbssid")) {
                                flag = 1;
                            }

                        }
                        break;
                    case 3:
                        while (mIterator.hasNext()) {
                            String info = mIterator.next().toString().toLowerCase();

                            if (info.contains("getmacaddress")) {
                                flag = 1;
                            }
                        }
                        break;
                    case 4:
                        while (mIterator.hasNext()) {
                            String info = mIterator.next().toString().toLowerCase();

                            if (info.contains("getdeviceid") || info.contains("getsubscriberid") || info.contains("gethardwareaddress")) {
                                flag = 1;
                            }
                        }
                        break;
                    default:
                        break;
                }


            } catch (Throwable e) {
            }
        }
        return flag;
    }

    /**
     * 获取hook的客户端方法名
     *
     * @return
     */
    public static String getHookMethod() {
        return mStringBuilder.toString();
    }

    private static boolean findHookStack() {
        try {
            throw new Exception("findhook");
        } catch (Exception e) {

            int zygoteInitCallCount = 0;
            for (StackTraceElement stackTraceElement : e.getStackTrace()) {
                if (stackTraceElement.getClassName().equals("com.android.internal.os.ZygoteInit")) {
                    zygoteInitCallCount++;
                    if (zygoteInitCallCount == 2) {
                        return true;
                    }
                }
                if (stackTraceElement.getClassName().equals("com.saurik.substrate.MS$2")
                        && stackTraceElement.getMethodName().equals("invoked")) {
                    return true;
                }
                if (stackTraceElement.getClassName().equals("de.robv.android.xposed.XposedBridge")
                        && stackTraceElement.getMethodName().equals("main")) {
                    return true;
                }
                if (stackTraceElement.getClassName().equals("de.robv.android.xposed.XposedBridge")
                        && stackTraceElement.getMethodName().equals("handleHookedMethod")) {
                    return true;
                }
            }
        }
        return false;
    }


}
