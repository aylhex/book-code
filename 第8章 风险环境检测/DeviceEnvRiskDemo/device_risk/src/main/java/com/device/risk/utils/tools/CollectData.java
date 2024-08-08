package com.device.risk.utils.tools;

import android.content.Context;
import android.location.Location;
import android.provider.Settings;
import com.device.risk.utils.device.DeviceUtils;
import com.device.risk.utils.device.MContext;
import com.device.risk.utils.device.NetworkUtils;
import com.device.risk.utils.device.ParamHolder;
import com.device.risk.utils.sensors.SensorData;
import com.device.risk.utils.sensors.SensorInfo;

import org.json.JSONObject;


public class CollectData {

    MJSON mJson = null;
    Context mContext;
    private String exceptError = "except_error";
    private String invalidError = "6666";

    public CollectData(Context context, MJSON mmJson) {
        mJson = mmJson;
        mContext = context;
    }

    public CollectData(Context context) {
        mContext = context;
    }

    public void getDataByJson() {
        SensorData mSensorData = new SensorData();

        try {
            mSensorData.startRegisterSensor(mContext);
        } catch (Exception e) {
            //e.printStackTrace();
        }


        try {
            JSONObject tmp = new JSONObject();
            Location mLocation = LocationUtils.getLocation();
            if (mLocation == null){
                tmp.put("lat",0.0);
                tmp.put("lon",0.0);
            }else {
                tmp.put("lat",mLocation.getLatitude());
                tmp.put("lon",mLocation.getLongitude());
            }
            mJson.setLocation(tmp);
        } catch (Exception e) {
            //e.printStackTrace();
        }


        try {
            boolean enableAdb = false;
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.CUPCAKE) {
                enableAdb = (Settings.Secure.getInt(mContext.getContentResolver(), Settings.Secure.ADB_ENABLED, 0) > 0);
            }
            String screenInfo = DeviceUtils.getScreenInfo();
            if (enableAdb) {
                mJson.setEnableAdb("true");
            } else {
                mJson.setEnableAdb("false");
            }
            mJson.setScreen(screenInfo);
        } catch (Exception e) {
            mJson.setEnableAdb(exceptError);
        }

        try {
            mJson.setSdCardCid(DeviceUtils.getSDcardCid(mContext));
        } catch (Exception e) {

        }

        try {
            mJson.setCurrentWifi(NetworkUtils.getConnectWifi(mContext));
        } catch (Exception e) {

        }


        try {
            mJson.setOaid(ParamHolder.oaid);
        } catch (Exception e) {
            mJson.setOaid(exceptError);
        }



        try {
            mJson.setNetType(NetworkUtils.getNetworkState(mContext) + "");
        } catch (Exception e) {
            mJson.setNetType(exceptError);
        }

        try {
            if (NetworkUtils.isWifiProxy(mContext)) {
                mJson.setProxy(1);
            } else {
                mJson.setProxy(0);
            }
        } catch (Exception e) {
        }

        try {
            mJson.setVersion(DeviceUtils.getOSVersion());
            mJson.setDeviceIp(IPUtils.getIPAddress(mContext));
        } catch (Exception e) {
        }

        try {
            int flag = HookUtils.checkXpoesd(ClassLoader.getSystemClassLoader(), mContext);
            if (flag == 1) {
                mJson.setHook(flag);
                mJson.setHookMethod(HookUtils.getHookMethod());
            } else {
                mJson.setHook(0);
                mJson.setHookMethod(invalidError);
            }

        } catch (Exception e) {

        }

        try {
            mJson.setModle(DeviceUtils.getPhoneModel());
        } catch (Exception e) {

        }

        try {
            mJson.setInstallationId(Instation.getInstallationId(mContext));
        } catch (Exception e) {

        }

        try {
            mJson.setBuildNumber(DeviceUtils.getBuildNumberVer());
        } catch (Exception e) {

        }

        try {
            if (PermUtils.RW_EXTERNAL_STORAGE){
                mJson.setSDcardPerm(1);
            }else {
                mJson.setSDcardPerm(0);
            }

        } catch (Exception e) {

        }

        try {
            if (PermUtils.READ_PHONE_STATE){
                mJson.setPhonePerm(1);
            }else {
                mJson.setPhonePerm(0);
            }
        } catch (Exception e) {

        }

        try {
            mJson.setDrmUid(DeviceUtils.getDrmUid());
        } catch (Exception e) {
            mJson.setDrmUid(exceptError);
        }

        try {
            SensorInfo mSensorInfo = new SensorInfo(mContext);
            String result = mSensorInfo.getGYROSCOPEInfo();
            int usbstate = mSensorInfo.getUSBState();

            if (!result.equals("None")) {
                mJson.setGyroscope(result);
            } else {
                mJson.setGyroscope(invalidError);
            }
            mJson.setUSBState(usbstate);
            mJson.setSensorInfo(mSensorInfo.getInfo());

        } catch (Exception e) {

        }

        // 检测是不是伪造了地理位置
        try {
            int fakegps = HookUtils.checkXpoesdFake(ClassLoader.getSystemClassLoader(), 1, mContext);
            mJson.setFakeGps(fakegps+"");
        } catch (Exception e) {
            mJson.setFakeGps(exceptError);
        }

        // 检测是不是伪造了地理位置
        try {
            int fakeBssid = HookUtils.checkXpoesdFake(ClassLoader.getSystemClassLoader(), 2, mContext);
            mJson.setFakeBssid(fakeBssid+"");
        } catch (Exception e) {
            mJson.setFakeBssid(exceptError);
        }

        // 检测是不是伪造了地理位置
        try {
            int fakeMacAdr = HookUtils.checkXpoesdFake(ClassLoader.getSystemClassLoader(), 3, mContext);
            mJson.setFakeMacAdr(fakeMacAdr+"");
        } catch (Exception e) {
            mJson.setFakeMacAdr(exceptError);
        }

        // 通过java层获取mac地址
        try {
            String jmac = DeviceUtils.getMacAddressByJava(mContext);
            mJson.setJavaMac(jmac);
        } catch (Exception e) {
            mJson.setJavaMac(exceptError);
        }

        // 检测是不是伪造了deviceid
        try {
            int fakeapp = HookUtils.checkXpoesdFake(ClassLoader.getSystemClassLoader(), 4, mContext);
            mJson.setFakeApp(fakeapp+"");
        } catch (Exception e) {
            mJson.setFakeGps(exceptError);
        }
        // 检测是不是开启vpn
        try {
            int vpnstatus = DeviceUtils.isVpnUsed();
            mJson.setVpnStatus(vpnstatus+"");
        } catch (Exception e) {
            mJson.setFakeGps(exceptError);
        }
        // 检测是否安装sim卡
        try {
            int simstatuse = DeviceUtils.hasSimCard(mContext);
            mJson.setSimStatus(simstatuse+"");
        } catch (Exception e) {
            mJson.setFakeGps(exceptError);
        }

        // 获取
        try {
            String virtualpath = MContext.getVirtualAppFiles(mContext);
            mJson.setVirtualPath(virtualpath);
        } catch (Exception e) {
            mJson.setFakeGps(exceptError);
        }

        // 获取IMEI
        try {
            String imei = DeviceUtils.getIMEI(mContext);
            mJson.setIMEI(imei);
        } catch (Exception e) {
            mJson.setFakeGps(exceptError);
        }

        // 获取IMSI
        try {
            String imsi = DeviceUtils.getIMSI(mContext);
            mJson.setIMSI(imsi);
        } catch (Exception e) {
            mJson.setFakeGps(exceptError);
        }

        // 获取AndroidId
        try {
            String androidId = DeviceUtils.getAndroidId(mContext);
            mJson.setAndroidID(androidId);
        } catch (Exception e) {
            mJson.setFakeGps(exceptError);
        }

        try {
            mJson.setSignatureData(SignUtils.getSignaturesMsg(mContext).toString());

        } catch (Exception e) {
            //e.printStackTrace();
        }
        try {
            Thread.sleep(3000);  //1000毫秒就是1秒
            mJson.setSensorData(mSensorData.getSensorResult());
            mSensorData.stopGetSensorData();
        } catch (Exception e) {
            //e.printStackTrace();
        }
    }

    public void getDataByJson(final JSONObject mJSONObject) {
        // 7.0及以上通过非常规方法获取的mac地址
        try {
            mJSONObject.put("mac", DeviceUtils.getMACAddress(mContext));
        } catch (Exception e) {
            // 异常后写入默认值便于后面分析
            try {
                mJSONObject.put("mac", exceptError);
            } catch (Exception e1) {

            }
        }

        // java层常规方法获取的mac地址
        try {
            String tmp= "jmacaddress";
            mJSONObject.put(tmp, DeviceUtils.getMacAddressByJava(mContext));
        } catch (Exception e) {
            // 异常后写入默认值便于后面分析
            try {
                String tmp= "jmacaddress";
                mJSONObject.put(tmp, exceptError);
            } catch (Exception e1) {

            }
        }
        // 获取sdcard路径
        try {
            String tmp="path";
            mJSONObject.put(tmp, DeviceUtils.getSDPath());
        } catch (Exception e) {
            // 异常后写入默认值便于后面分析
            try {
                String tmp="path";
                mJSONObject.put(tmp, "/sdcard");
            } catch (Exception e1) {
            }
        }
    }
}
