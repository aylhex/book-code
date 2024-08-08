package com.device.risk.utils.device;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Context;
import android.media.MediaDrm;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Environment;
import android.provider.Settings;
import android.telephony.TelephonyManager;

import com.device.risk.utils.tools.MBase64;
import com.device.risk.utils.tools.MLog;
import com.device.risk.utils.tools.PermUtils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.lang.reflect.Method;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Enumeration;
import java.util.UUID;


public class DeviceUtils {

    private static String invalid = "invalid";
    private static String padding = "6666";
    private static String Error = "Error";
    private static String exceptError = "except_error";

    /**
     * 1、匹配上无效值返回true
     * 2、未匹配上无效值返回false
     */
    private static boolean checkInvalue(String info) {

        if (info.equals(invalid)) {
            return true;
        }

        if (info.equals(Error)) {
            return true;
        }
        return false;
    }

    // 返回有效的MAC地址
    public static String getMACAddress(Context context) {

        String macbynetworkinter = getMACAddressByNetworkInterfaces(context);
        String macbyip = getMacAddressByIP();

        //如果都为invalid则返回invalid
        if (macbynetworkinter.equals(invalid) && macbyip.equals(invalid)) {
            String mac = getAddressMacByFile();
            if (mac.equals(invalid)) {
                return invalid;
            } else {
                return mac;
            }
        }

        //如果都为Error则返回Error
        if (macbynetworkinter.equals(Error) && macbyip.equals(Error)) {
            String mac = getAddressMacByFile();
            if (mac.equals(Error)) {
                return invalid;
            } else {
                return mac;
            }
        }

        //如果系统版本>=7.0 且值不为空 取第二种方式的mac地址
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            // 判断两种方式获取的mac地址是不是都是有效的地址
            if (!checkInvalue(macbynetworkinter) && !checkInvalue(macbyip)) {
                // 有效值但两个值不一样 取通过IP地址方式获取的mac地址
                if (!macbynetworkinter.equals(macbyip)) {
                    return macbyip;
                }
            }
            // 第一种方式的mac有效  第二种无效
            if (!checkInvalue(macbynetworkinter) && checkInvalue(macbyip)) {

                return macbynetworkinter;
            }

            return macbyip;
        }

        // 判断两种方式都获取到了有效mac地址
        if (!checkInvalue(macbynetworkinter) && !checkInvalue(macbyip)) {
            if (NetworkUtils.isConnectInternet(context)) {
                if (1 == NetworkUtils.getNetworkState(context)) {
                    //在wifi状态下获取的两个mac地址不同以第二个为准
                    if (!macbynetworkinter.equals(macbyip)) {
                        return macbyip;
                    }
                }
            }
        }
        // 第一种为有效值， 第二种方式无效值
        if (!checkInvalue(macbynetworkinter) && checkInvalue(macbyip)) {
            return macbynetworkinter;
        }

        // 第二种为有效值， 第一种方式无效值
        if (checkInvalue(macbynetworkinter) && !checkInvalue(macbyip)) {
            if (NetworkUtils.isConnectInternet(context)) {
                if (1 == NetworkUtils.getNetworkState(context)) {
                    return macbyip;
                }
            }
        }

        return macbynetworkinter;
    }

    private static String getMACAddressByNetworkInterfaces(Context context) {
        String macaddr = invalid;
        try {
            Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
            while (interfaces.hasMoreElements()) {
                NetworkInterface net = interfaces.nextElement();
                byte[] addr = net.getHardwareAddress();
                if (addr == null || addr.length == 0) {
                    continue;
                }
                StringBuilder buf = new StringBuilder();
                for (byte b : addr) {
                    buf.append(String.format("%02x:", b));
                }
                if (buf.length() > 0) {
                    buf.deleteCharAt(buf.length() - 1);
                }
                if (net.getName().equals("wlan0")) {
                    macaddr = buf.toString();
                    break;
                }
            }

            if (macaddr.equals("02:00:00:00:00:00") || macaddr.equals("00:00:00:00:00:00")) {
                return invalid;
            } else {
                return macaddr;
            }
        } catch (Exception e) {
            macaddr = Error;
        }
        return macaddr;
    }


    public static String getMacAddressByJava(Context context) {
        try {
            if (PermUtils.checkPermission(context, Manifest.permission.ACCESS_WIFI_STATE)) {
                WifiManager wifi = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
                @SuppressLint("MissingPermission")
                WifiInfo info = wifi.getConnectionInfo();
                String mac = info.getMacAddress();
                // mac地址不问空
                if (!mac.isEmpty()) {
                    if (mac.equals("02:00:00:00:00:00") || mac.equals("00:00:00:00:00:00") || mac.equals("")) {
                        return invalid;
                    }
                }
                return mac;
            }
        } catch (Exception e) {
            // e.printStackTrace();
        }
        return invalid;
    }

    /**
     * 获取移动设备本地IP
     *
     * @return
     */
    private static InetAddress getLocalInetAddress() {
        InetAddress ip = null;
        try {
            //列举
            Enumeration<NetworkInterface> en_netInterface = NetworkInterface.getNetworkInterfaces();
            while (en_netInterface.hasMoreElements()) {//是否还有元素
                NetworkInterface ni = (NetworkInterface) en_netInterface.nextElement();//得到下一个元素
                Enumeration<InetAddress> en_ip = ni.getInetAddresses();//得到一个ip地址的列举
                while (en_ip.hasMoreElements()) {
                    ip = en_ip.nextElement();
                    if (!ip.isLoopbackAddress() && ip.getHostAddress().indexOf(":") == -1) {
                        break;
                    } else {
                        ip = null;
                    }
                }

                if (ip != null) {
                    break;
                }
            }
        } catch (Exception e) {

            //e.printStackTrace();
        }
        return ip;
    }

    /**
     * 根据IP地址获取MAC地址
     *
     * @return
     */
    private static String getMacAddressByIP() {
        String macAddr = "invalid";
        try {
            //获得IpD地址
            InetAddress ip = getLocalInetAddress();
            byte[] b = NetworkInterface.getByInetAddress(ip).getHardwareAddress();
            StringBuffer buffer = new StringBuffer();
            int count = 0;
            for (int i = 0; i < b.length; i++) {
                if (i != 0) {
                    buffer.append(':');
                    count++;
                }
                String str = Integer.toHexString(b[i] & 0xFF);
                buffer.append(str.length() == 1 ? 0 + str : str);
            }
            if (count != 0) {
                macAddr = buffer.toString().toUpperCase();
            } else {
                macAddr = invalid;
            }

        } catch (Exception e) {
            macAddr = Error;
        }

        return macAddr;
    }

    private static String getAddressMacByFile() {
        String fileAddressMac = "/sys/class/net/wlan0/address";
        try {
            File mFile = new File(fileAddressMac);
            if (mFile.exists()) {
                FileInputStream fileInputStream = new FileInputStream(mFile);
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(fileInputStream));
                String mac = bufferedReader.readLine().trim();
                return mac;
            } else {
                return invalid;
            }
        } catch (Exception e) {
            //e.printStackTrace();
        }
        return Error;
    }

    public static String getScreenInfo() {
        StringBuilder mStringBuilder = new StringBuilder();
        mStringBuilder.append(ScreenUtils.getRealScreenHeigh() + "x");
        mStringBuilder.append(ScreenUtils.getRealScreenWidth());
        return mStringBuilder.toString();
    }


    public static String getPhoneModel() {
        String model = Build.MODEL;
        return model;
    }

    /**
     * BASEBAND-VER
     * 基带版本
     * return String
     */
    public static String getBuildNumberVer() {

        String version = Build.FINGERPRINT;
        int index = version.lastIndexOf(":");
        if (index > 0) {
            String subString = version.substring(0, index);
            String[] strs = subString.split("/");
            if (strs.length > 2) {
                version = "";
                version += strs[strs.length - 2] + "/";
                version += strs[strs.length - 1];
            }
        }
        return version;
    }

    /**
     * 获取SDCard信息
     */
    public static String getSDcardCid(Context context) {

        StringBuilder cid = new StringBuilder();
        String path1 = "/sys/block/mmcblk0/source.device/type";
        String path2 = "/sys/block/mmcblk1/source.device/type";
        try {
            String path_cid = null;
            File mFile = new File(path1);
            if (mFile.exists()) {
                FileReader mFileReader = new FileReader(path1);
                BufferedReader mBufferedReader = new BufferedReader( mFileReader);
                if (mBufferedReader.readLine().toLowerCase().contentEquals("mmc")) {
                    path_cid = "/sys/block/mmcblk0/source.device/";
                    FileReader mFileReader2 = new FileReader(path_cid + "cid"); // nand ID
                    BufferedReader mBufferedReader2 = new BufferedReader(mFileReader2);
                    cid.append(mBufferedReader2.readLine());
                    mFileReader2.close();
                    mBufferedReader2.close();
                    return cid.toString();
                }
                mFileReader.close();
                mBufferedReader.close();
            }

            mFile = new File(path2);
            if (mFile.exists()) {

                FileReader mFileReader = new FileReader(path2);
                BufferedReader mBufferedReader = new BufferedReader( mFileReader);
                if (mBufferedReader.readLine().toLowerCase().contentEquals("sd")){
                    path_cid = "/sys/block/mmcblk1/source.device/";
                    FileReader mFileReader2 = new FileReader(path_cid + "cid");// SD Card ID

                    BufferedReader mBufferedReader2 = new BufferedReader(mFileReader2);
                    cid.append(mBufferedReader2.readLine());
                    mFileReader2.close();
                    mBufferedReader2.close();

                }
                mFileReader.close();
                mBufferedReader.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return cid.toString();
    }

    public static String getAndroidId(Context context) {
        return Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
    }

    /**
     * 获得手机IMEI号
     */
    @SuppressLint("MissingPermission")
    public static String getIMEI(Context context) {
        String deviceId = null;

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q){
            return deviceId;
        }
        try {
            if (PermUtils.READ_PHONE_STATE) {
                TelephonyManager telephonyManager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
                deviceId = telephonyManager.getDeviceId();
            }
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        return deviceId;
    }

    /**
     * 获取手机IMSI
     */
    @SuppressLint("MissingPermission")
    public static String getIMSI(Context context) {
        try {
            if (PermUtils.READ_PHONE_STATE) {
                TelephonyManager telephonyManager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
                //获取IMSI号
                String imsi = telephonyManager.getSubscriberId();
                if (null == imsi) {
                    imsi = "6666";
                }
                return imsi;
            }
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        return "6666";
    }

    /**
     * 获得手机操作系统版本号
     */
    public static String getOSVersion() {
        return Build.VERSION.RELEASE;
    }

    /**
     * 是否正在使用VPN
     */
    public static int isVpnUsed() {
        int result = 0;
        try {
            Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
            while (interfaces.hasMoreElements()) {
                NetworkInterface mNetworkInterface = (NetworkInterface) interfaces.nextElement();
                if (!mNetworkInterface.isUp() || mNetworkInterface.getInterfaceAddresses().size() == 0) {
                    continue;
                }
                // 如果满足条件则是开启了vpn
                if ("tun0".equals(mNetworkInterface.getName()) || "ppp0".equals(mNetworkInterface.getName())) {
                    result = 1;
                    return result;
                }
            }

        } catch (Throwable e) {
            //e.printStackTrace();
        }
        return result;
    }

    /**
     * 判断是否包含SIM卡
     * ret 1 有SIM卡
     * ret 0 没有SIM卡
     *
     * @return 状态
     */
    public static int hasSimCard(Context context) {
        TelephonyManager telMgr = (TelephonyManager)
                context.getSystemService(Context.TELEPHONY_SERVICE);
        int simState = telMgr.getSimState();
        int result = 1;
        switch (simState) {
            case TelephonyManager.SIM_STATE_ABSENT:
                result = 0; // 没有SIM卡
                break;
            case TelephonyManager.SIM_STATE_UNKNOWN:
                result = 0;
                break;
            default:
                break;
        }
        return result;
    }

    public static String getDrmUid() {
        StringBuilder deviceUniqueId = new StringBuilder();
        try {
            if(Build.VERSION.SDK_INT >= 18){
                MediaDrm mediaDrm = new MediaDrm(new UUID(-1301668207276963122L, -6645017420763422227L));
                if (mediaDrm != null){
                    @SuppressLint("WrongConstant")
                    byte[]  ret = mediaDrm.getPropertyByteArray("deviceUniqueId");
                    if(ret != null) {
                        deviceUniqueId.append(MBase64.encode(ret));
                    }
                    mediaDrm.release();
                }
            }
        } catch (Exception e) {
            deviceUniqueId.append(exceptError);
        }
        return deviceUniqueId.toString();
    }

    public static String getSerialNumber(){
        StringBuilder serial = new StringBuilder();

        try {
            if (Build.VERSION.SDK_INT > Build.VERSION_CODES.N){
                serial.append(Build.SERIAL);
            }else {
                Class<?> c = Class.forName("android.os.SystemProperties");
                Method get = c.getMethod("get", String.class);
                serial.append((String) get.invoke(c, "ro.serialno"));
            }
        } catch (Exception e) {
            serial.append(padding);
        }
        return serial.toString();
    }

    /**
     * 获取SDcard路径
     *
     * @return
     */
    public static String getSDPath() {
        String sdpath = null;
        try {
            File sdDir = null;
            boolean sdCardExist = Environment.getExternalStorageState()
                    .equals(Environment.MEDIA_MOUNTED);//判断sd卡是否存在
            if (sdCardExist) {
                sdDir = Environment.getExternalStorageDirectory();//获取跟目录
                sdpath = sdDir.toString();
            }
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        if (sdpath == null) {
            sdpath = "/sdcard";
        }
        return sdpath;
    }
}


