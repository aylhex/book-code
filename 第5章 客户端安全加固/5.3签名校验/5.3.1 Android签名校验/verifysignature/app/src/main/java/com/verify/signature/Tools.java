package com.verify.signature;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Build;
import android.util.Log;

import java.io.File;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.security.MessageDigest;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class Tools {

    static{
        try {
            System.loadLibrary("test");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static native String getSignByJni(Context mContext);

    public static boolean checkSignature(Context context){
        String sign = "8b34e97425e0e682e3a73bd55830fc28ce34a4e8";
        if (sign.equals(getReflectSignature(context))){
            return true;
        }else {
            return false;
        }
    }

    public static String getSignatureFileCrc(Context context){

        try {
            ZipFile zf;
            String path= context.getPackageManager().getApplicationInfo(context.getPackageName(), 0).publicSourceDir;
            zf = new ZipFile(path);
            ZipEntry ze = zf.getEntry("META-INF/MANIFEST.MF");
            String crcValue = String.valueOf(ze.getCrc());
            Log.i("Test MANIFEST Crc ====>",crcValue);

            return crcValue;
        }catch (Exception e){
            e.printStackTrace();
        }
        return null;
    }

    public static String getAppSignature(Context context) {
        try {
            return sha1(getSignature(context));
        } catch (Exception e) {
        }
        return null;
    }

    public static byte[] getSignature(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] signatures = packageInfo.signatures;
            if (signatures != null)
            {
                Log.i("Test ===>", signatures[0].toCharsString());
                return signatures[0].toByteArray();
            }
        } catch (Exception e) {
        }
        return null;
    }

    public static String getReflectSignature(Context context) {
        try {
            String path= context.getPackageManager().getApplicationInfo(context.getPackageName(), 0).publicSourceDir;
            return sha1(getApkSignatureByReflect(path));
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

    public static byte[] getApkSignatureByReflect(String apkPath) {
        String fullPackageParserPath = "android.content.pm.PackageParser";
        try {

            Class packageParserClass = Class.forName(fullPackageParserPath);
            Constructor pkgParserConstructor = null;
            Object pkgParserIns = null;

            if (Build.VERSION.SDK_INT > 20){

                pkgParserConstructor = packageParserClass.getConstructor();
                pkgParserIns = pkgParserConstructor.newInstance();
                Class[] args = {File.class, Integer.TYPE};;
                Method parsePackageMethod = packageParserClass.getDeclaredMethod("parsePackage", args);
                Object[] valueArgs  = new Object[2];
                valueArgs[0] = new File(apkPath);
                valueArgs[1] = PackageManager.GET_SIGNATURES;
                Object parserPackage = parsePackageMethod.invoke(pkgParserIns, valueArgs);

                if (Build.VERSION.SDK_INT>=28){
                    Class[] typeArgs = {parserPackage.getClass(),Boolean.TYPE};
                    Method collectCertificatesMethod = packageParserClass.getDeclaredMethod("collectCertificates", typeArgs);
                    Object[] valueArgs2 = {parserPackage, Build.VERSION.SDK_INT>28};
                    collectCertificatesMethod.invoke(pkgParserIns, valueArgs2);
                    Field mSignatures = null;

                    if(Build.VERSION.SDK_INT>=29){
                        mSignatures = parserPackage.getClass().getDeclaredField("mSigningDetails");
                    }else{
                        mSignatures = parserPackage.getClass().getDeclaredField("mSignatures");
                    }
                    mSignatures.setAccessible(true);
                    Object mSigningDetails =  mSignatures.get(parserPackage);
                    Field infoField = mSigningDetails.getClass().getDeclaredField("signatures");
                    infoField.setAccessible(true);
                    Signature[] info = (Signature[]) infoField.get(mSigningDetails);
                    Log.i("ReflectSignature ===>", info[0].toCharsString());
                    return info[0].toByteArray();
                }

                Class[] typeArgs = {parserPackage.getClass(),Integer.TYPE};
                Method collectCertificatesMethod = packageParserClass.getDeclaredMethod("collectCertificates", typeArgs);
                Object[] valueArgs2 = {parserPackage, PackageManager.GET_SIGNATURES};
                collectCertificatesMethod.invoke(pkgParserIns, valueArgs2);
                // 应用程序信息包, 这个公开的, 不过有些函数, 变量没公开
                Field packageInfoFld = parserPackage.getClass().getDeclaredField("mSignatures");
                Signature[] info = (Signature[]) packageInfoFld.get(parserPackage);
                Log.i("ReflectSignature ===>", info[0].toCharsString());
                return info[0].toByteArray();
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
}

