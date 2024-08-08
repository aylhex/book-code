package com.ecdh.crypto;

import android.content.Context;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class MContext {
    public static Context sContext = null;

    public static void init(Context context) {
        if (sContext == null){
            sContext = context;
        }

    }

    public static Context getContext() {
        return sContext;
    }

    /**
     * MD5 算法
     *
     * @param string
     * @return
     */
    public static String MD5(String string) {

        MessageDigest md5 = null;
        try {
            md5 = MessageDigest.getInstance("MD5");
            byte[] bytes = md5.digest(string.getBytes());
            StringBuilder result = new StringBuilder();
            for (byte b : bytes) {
                String temp = Integer.toHexString(b & 0xff);
                if (temp.length() == 1) {
                    temp = "0" + temp;
                }
                result.append(temp);
            }
            return result.toString();
        } catch (NoSuchAlgorithmException e) {
        }
        return null;
    }
}
