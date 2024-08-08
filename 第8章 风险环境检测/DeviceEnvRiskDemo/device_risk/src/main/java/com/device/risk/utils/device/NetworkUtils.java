package com.device.risk.utils.device;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import com.device.risk.utils.tools.PermUtils;

import org.json.JSONObject;

public class NetworkUtils {
    public static final int NETWORN_NONE = 0;
    public static final int NETWORN_WIFI = 1;
    public static final int NETWORN_2G = 2;
    public static final int NETWORN_3G = 3;
    public static final int NETWORN_4G = 4;
    public static final int NETWORN_5G = 5;
    public static final int NETWORN_MOBILE = 10;

    public static boolean isConnectInternet(Context context) {
        ConnectivityManager conManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        try {
            if (conManager != null) {
                try {
                    @SuppressLint("MissingPermission")
                    NetworkInfo networkInfo = conManager.getActiveNetworkInfo();
                    if (networkInfo != null) {
                        return networkInfo.isAvailable();
                    }
                } catch (Exception e) {
                }
            }
        } catch (Exception e) {

        }
        return false;
    }

    /**
     * 获取当前网络连接类型
     */
    @SuppressLint("MissingPermission")
    public static int getNetworkState(Context context) {
        //获取系统的网络服务
        ConnectivityManager connManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);

        try {
            //如果当前没有网络
            if (null == connManager) {
                return NETWORN_NONE;
            }
            //获取当前网络类型，如果为空，返回无网络
            NetworkInfo activeNetInfo = connManager.getActiveNetworkInfo();
            if (activeNetInfo == null || !activeNetInfo.isAvailable()) {
                return NETWORN_NONE;
            }

            // 判断是不是连接的是不是wifi
            NetworkInfo wifiInfo = connManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
            if (null != wifiInfo) {
                NetworkInfo.State state = wifiInfo.getState();
                if (null != state)
                    if (state == NetworkInfo.State.CONNECTED || state == NetworkInfo.State.CONNECTING) {
                        return NETWORN_WIFI;
                    }
            }
            // 如果不是wifi，则判断当前连接的是运营商的哪种网络2g、3g、4g等
            NetworkInfo networkInfo = connManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);

            if (null != networkInfo) {
                NetworkInfo.State state = networkInfo.getState();
                String strSubTypeName = networkInfo.getSubtypeName();
                if (null != state)
                    if (state == NetworkInfo.State.CONNECTED || state == NetworkInfo.State.CONNECTING)
                    {
                        switch (activeNetInfo.getSubtype())
                        {
                            //如果是2g类型
                            case TelephonyManager.NETWORK_TYPE_GPRS: // 联通2g
                            case TelephonyManager.NETWORK_TYPE_CDMA: // 电信2g
                            case TelephonyManager.NETWORK_TYPE_EDGE: // 移动2g
                            case TelephonyManager.NETWORK_TYPE_1xRTT:
                            case TelephonyManager.NETWORK_TYPE_IDEN:
                                return NETWORN_2G;
                            //如果是3g类型
                            case TelephonyManager.NETWORK_TYPE_EVDO_A: // 电信3g
                            case TelephonyManager.NETWORK_TYPE_UMTS:
                            case TelephonyManager.NETWORK_TYPE_EVDO_0:
                            case TelephonyManager.NETWORK_TYPE_HSDPA:
                            case TelephonyManager.NETWORK_TYPE_HSUPA:
                            case TelephonyManager.NETWORK_TYPE_HSPA:
                            case TelephonyManager.NETWORK_TYPE_EVDO_B:
                            case TelephonyManager.NETWORK_TYPE_EHRPD:
                            case TelephonyManager.NETWORK_TYPE_HSPAP:
                                return NETWORN_3G;
                            //如果是4g类型
                            case TelephonyManager.NETWORK_TYPE_LTE:
                                return NETWORN_4G;
                            case TelephonyManager.NETWORK_TYPE_NR:
                                return NETWORN_5G;
                            default:
                                //中国移动 联通 电信 三种3G制式
                                if (strSubTypeName.equalsIgnoreCase("TD-SCDMA") ||
                                        strSubTypeName.equalsIgnoreCase("WCDMA") ||
                                        strSubTypeName.equalsIgnoreCase("CDMA2000"))
                                {
                                    return NETWORN_3G;
                                } else {
                                    return NETWORN_MOBILE;
                                }
                        }
                    }
            }
        } catch (Exception e) {
            return -1;
        }
        return NETWORN_NONE;
    }

    @SuppressLint("MissingPermission")
    public static boolean isNetworkAvailable() {

        if (MContext.getContext() == null) {
            return false;
        }
        ConnectivityManager connectivityManager = (ConnectivityManager) MContext.getContext() .getSystemService(Context.CONNECTIVITY_SERVICE);
        if (connectivityManager == null) {
            return false;
        }
        if (Build.VERSION.SDK_INT >= 21) {
            try {
                Network[] networks = connectivityManager.getAllNetworks();
                NetworkInfo networkInfo = null;
                if (networks != null) {
                    for (Network mNetwork : networks) {
                        try {
                            networkInfo = connectivityManager.getNetworkInfo(mNetwork);
                        } catch (Exception e) {
                        }
                        if (networkInfo != null) {
                            if (NetworkInfo.State.CONNECTED.equals(networkInfo.getState())) {
                                return true;
                            }
                        }
                    }
                }
            } catch (NoSuchMethodError e) {
                return isNetworkAvailableApiBelow21(connectivityManager);
            }
        } else {
            return isNetworkAvailableApiBelow21(connectivityManager);
        }

        return false;
    }

    @SuppressLint("MissingPermission")
    private static boolean isNetworkAvailableApiBelow21(ConnectivityManager connectivityManager) {
        NetworkInfo[] info = null;
        try {
            info = connectivityManager.getAllNetworkInfo();
        } catch (Exception ex) {
            return false;
        }
        if (info != null) {
            for (int i = 0; i < info.length; i++) {
                if (info[i].getState() == NetworkInfo.State.CONNECTED) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * 获取当前连接的路由器信息
     *
     * @param context
     * @return
     */
    @SuppressLint("MissingPermission")
    public static JSONObject getConnectWifi(Context context) {
        String Error = "Error";
        JSONObject json = new JSONObject();
        try {
            if (!PermUtils.checkPermission(context, Manifest.permission.ACCESS_WIFI_STATE)) {
                return null;
            }

            WifiManager mWifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
            if (mWifiManager == null) {
                return null;
            }
            if (!mWifiManager.isWifiEnabled()) {
                return null;
            }
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.O){
                json.put("SSID", mWifiManager.getConnectionInfo().getSSID().trim().replace("\"", ""));
            }else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O && Build.VERSION.SDK_INT <Build.VERSION_CODES.P){
                ConnectivityManager mConnectivityManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
                assert mConnectivityManager != null;
                NetworkInfo info = mConnectivityManager.getActiveNetworkInfo();
                if (info != null && info.isConnected()) {
                    String ssid = info.getExtraInfo();
                    json.put("SSID", ssid.trim().replace("\"", ""));
                }
            } else {
                json.put("SSID", mWifiManager.getConnectionInfo().getSSID().trim().replace("\"", ""));
            }

            json.put("Signal", mWifiManager.calculateSignalLevel(mWifiManager.getConnectionInfo().getRssi(), 1001));
            json.put("BSSID", mWifiManager.getConnectionInfo().getBSSID());

        } catch (Exception e) {
            e.printStackTrace();
        }
        return json;
    }


    /**
     * 检测手机是否开启代理
     *
     * @param context
     * @return
     */
    public static boolean isWifiProxy(Context context) {

        final boolean IS_ICS_OR_LATER = Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH;
        String proxyAddress;
        int proxyPort;

        if (IS_ICS_OR_LATER) {
            proxyAddress = System.getProperty("http.proxyHost");
            String portStr = System.getProperty("http.proxyPort");
            proxyPort = Integer.parseInt((portStr != null ? portStr : "-1"));
        } else {
            proxyAddress = android.net.Proxy.getHost(context);
            proxyPort = android.net.Proxy.getPort(context);
        }
        return (!TextUtils.isEmpty(proxyAddress)) && (proxyPort != -1);
    }

}
