package com.device.risk.utils.tools;

/**
 * Created by an on 2023/5/8.
 */

public class StringUtils
{
    public static boolean isEmpty(String str)
    {
        if ((str != null) && (str.length() > 0)) {
            return false;
        }
        return true;
    }

    public static boolean isEmpty(byte[] str)
    {
        if ((str != null) && (str.length > 0)) {
            return false;
        }
        return true;
    }

    public static String convertObjectToString(Object obj)
    {

        if (obj != null)
        {
            if ((obj instanceof String)) {
                return ((String)obj).toString();
            }
            if ((obj instanceof Integer)) {
                return "" + ((Integer)obj).intValue();
            }
            if ((obj instanceof Long)) {
                return "" + ((Long)obj).longValue();
            }
            if ((obj instanceof Double)) {
                return "" + ((Double)obj).doubleValue();
            }
            if ((obj instanceof Float)) {
                return "" + ((Float)obj).floatValue();
            }
            if ((obj instanceof Short)) {
                return "" + ((Short)obj).shortValue();
            }
            if ((obj instanceof Byte)) {
                return "" + ((Byte)obj).byteValue();
            }
            if ((obj instanceof Boolean)) {
                return ((Boolean)obj).toString();
            }
            if ((obj instanceof Character)) {
                return ((Character)obj).toString();
            }
            return obj.toString();
        }
        return "";
    }

    public static int hashCode(String value)
    {
        int h = 0;
        if ((h == 0) && (value.length() > 0))
        {
            char[] val = value.toCharArray();
            for (int i = 0; i < val.length; i++) {
                h = 31 * h + val[i];
            }
        }
        return h;
    }
    public static String getString(byte[] data){
        if (data != null){
            return new String(data);
        }
        return "";
    }


}
