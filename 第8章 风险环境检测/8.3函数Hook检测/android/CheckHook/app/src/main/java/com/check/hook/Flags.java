package com.check.hook;

import android.os.Build;


import com.check.utils.reflect.MReflection;
import com.check.utils.tools.MContext;
import com.check.utils.tools.MLog;

import org.json.JSONException;
import org.json.JSONObject;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Map;

public class Flags {
    private static final String TAG = "AccessFlags";
    private static Field accessFlags = null;
    private static Method artMethod = null;
    static {
        try {
            MReflection.unseal(MContext.getContext());
            Field[] fileds = new Field[0];

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P){
                Class tempClass = Class.forName("java.lang.reflect.Executable");
                fileds = tempClass.getDeclaredFields();
            }
            else if (Build.VERSION_CODES.O <= Build.VERSION.SDK_INT &&  Build.VERSION.SDK_INT <  Build.VERSION_CODES.P) {
                Class tempClass = Class.forName("java.lang.reflect.Executable");
                fileds = tempClass.getDeclaredFields();
            }
            else if (Build.VERSION_CODES.M <= Build.VERSION.SDK_INT && Build.VERSION.SDK_INT < Build.VERSION_CODES.O){
                Class cl = Class.forName("java.lang.reflect.AbstractMethod");
                fileds = cl.getDeclaredFields();
            }
            else if ( Build.VERSION.SDK_INT < Build.VERSION_CODES.M ){
                Class cl = Class.forName("java.lang.reflect.AbstractMethod");
                artMethod = cl.getDeclaredMethod("getAccessFlags");
                artMethod.setAccessible(true);

            }
            for(Field field : fileds){
                if(field.getName().equals("accessFlags")){
                    accessFlags = field;
                    accessFlags.setAccessible(true);
                    MLog.i("ACCESSFLAG", "get accessFlags");
                    break;
                }
            }
        } catch (Exception e) {

            MLog.i("ACCESSFLAG  Exception ", e.getMessage());
            MLog.printStackTrace(TAG, e);
        }
    }

    public static JSONObject showFlags(String cls, Map map){

        JSONObject methodDetail = new JSONObject();

        try {
            Method[] methods = Class.forName(cls).getDeclaredMethods();
            for(Method method : methods){

                if (map.containsKey(method.getName())){
                    if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M){
                        int flag = (int) artMethod.invoke(method);
                        methodDetail.put((String) map.get(method.getName()), "0x"+String.format("%08x", flag));
                    }else {
                        int flag = (int) accessFlags.get(method);
                        methodDetail.put((String) map.get(method.getName()), "0x"+String.format("%08x", flag));
                    }
                }
            }
        }catch (Exception e){
            for(Object key : map.keySet()){
                String value = (String) map.get(key);
                try {
                    methodDetail.put((String) map.get(value), "nosupported version");
                } catch (JSONException ex) {
                    MLog.printStackTrace(TAG, ex);
                }
            }
            MLog.printStackTrace(TAG, e);
        }
        return methodDetail;
    }

    public static String showFlags3(Class cls, String methodName, Class<?>... parameterTypes){

        try {

            Method method = cls.getDeclaredMethod(methodName, parameterTypes);

            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M){
                int flag = (int) artMethod.invoke(method);
                return "0x"+String.format("%08x", flag);
            }else {
                int flag = (int) accessFlags.get(method);
                return "0x"+String.format("%08x", flag);
            }

        }catch (Exception e){
            MLog.printStackTrace(TAG, e);
        }
        return "nosupported version";
    }

    public static String showFlags2(Class cls, String methodName){

        try {
            Method method = cls.getDeclaredMethod(methodName);
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M){
                int flag = (int) artMethod.invoke(method);
                return "0x"+String.format("%08x", flag);
            }else {
                int flag = (int) accessFlags.get(method);
                return "0x"+String.format("%08x", flag);
            }
        }catch (Exception e){
            MLog.printStackTrace(TAG, e);
        }
        return "nosupported version";
    }

    public static JSONObject showFlags2(String cls){

        JSONObject methodDetail = new JSONObject();

        try {
            Method[] methods = Class.forName(cls).getDeclaredMethods();
            for(Method method : methods){
                int flag = (int) accessFlags.get(method);
                MLog.i("ACCESSFLAG", method.getName() + " jopcode=0x" + String.format("%08x", flag));
            }
        }catch (Exception e){
            MLog.printStackTrace(TAG, e);
        }
        return methodDetail;
    }
}
