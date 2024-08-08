package com.device.risk.utils.device;

import android.content.Context;

import com.device.risk.android.Native;
import com.device.risk.utils.tools.CollectData;
import com.device.risk.utils.tools.MLog;
import com.device.risk.utils.tools.MJSON;
import com.device.risk.utils.tools.PermUtils;

import org.json.JSONObject;
/**
 * Created by an on 2023/4/13.
 */
public class Utils {

    public static String SDK_VERSION = "1.0.0";
    /**
     * 拼接java层获取的设备信息
     * @param mContext
     * @return
     */
    private static String getBasicDeviceInfos(final Context mContext) {
        final MJSON mJson = new MJSON();
        CollectData mSaveDeviceDataToJson = new CollectData(mContext, mJson);
        mSaveDeviceDataToJson.getDataByJson();
        return mJson.toString();
    }

    /**
     * 返回信息 SDK的信息
     * @param context
     * @return
     */
    public static String getDeviceInfos(final Context context) {
        MContext.init(context);
        PermUtils.checkCurrentPermission(context);

        String result = getCollectData();
        MLog.i("result==>"+result);

        return result;
    }
    /**
     * NDK层  返回上报所需的消息体
     * @param context
     * @return
     */
    private static String getDeviceEnvironmentMsg(Context context) {
        StringBuilder msg = new StringBuilder();
        JSONObject json = new JSONObject();
        try {
            CollectData mUtilsService = new CollectData(context);
            mUtilsService.getDataByJson(json);
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        try {
            JSONObject dataJson = new JSONObject();
            dataJson.put("client_info", json.toString());
            msg.append(new String(Native.doCommand(json.toString().getBytes())));
            return msg.toString();
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        return msg.toString();
    }

    /**
     * 返回信息 SDK获取的信息
     * @return
     */
    public static String getCollectData() {

        try {
            JSONObject json = new JSONObject();
            // Java层获取设备信息
            String javainfo = getBasicDeviceInfos(MContext.getContext());
            MLog.i("javainfo====>"+javainfo);
            // NDK获取设备信息
            String nativeinfo = getDeviceEnvironmentMsg(MContext.getContext());
            MLog.i("nativeinfo====>"+nativeinfo);

            json.put("javainfo", javainfo);
            json.put("nativeinfo", nativeinfo);

            return json.toString();
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        return "";
    }
}





