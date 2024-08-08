package com.demo.tools;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.util.Log;

import java.security.MessageDigest;

public class ToolUtils {

    static String sign = "8b34e97425e0e682e3a73bd55830fc28ce34a4e8";

    public static boolean checkSignature(Context context){
        if (sign.equals(getAppSignature(context))){
            return true;
        }else {
            return false;
        }
    }


    public static String getAppSignature(Context context) {
        try {
            return sha1(getSignature(context));
        } catch (Exception e) {
        }
        return null;
    }

    public static String sha1(byte[] bytes) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA1");
            md.update(bytes);
            byte[] b = md.digest();
            int i;
            StringBuilder sb = new StringBuilder();
            for (byte value : b) {
                i = value;
                if (i < 0)
                    i += 256;
                if (i < 16)
                    sb.append("0");
                sb.append(Integer.toHexString(i));
            }
            return sb.toString();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    public static void getSignature2(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] signatures = packageInfo.signatures;
            if (signatures != null)
            {
                Log.i("signatures===>",signatures[0].toCharsString());
            }
        } catch (Exception e) {
        }
    }

    public static byte[] getSignature(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] signatures = packageInfo.signatures;
            if (signatures != null)
            {
                return signatures[0].toByteArray();
            }
        } catch (Exception e) {
        }
        return null;
    }

}
