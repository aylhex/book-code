package com.check.hook;

public class HookUtils {

    // Java层监控的字段对应Key
    public static String j_WifiManager_FLAGOPCODE = "mSubscriptionManager";
    public static final String[] jmethodList = {
            "android.net.wifi.WifiInfo|getMacAddress()",
            "android.net.wifi.WifiInfo|getSSID()",
            "android.net.wifi.WifiInfo|getBSSID()",
    };
}
