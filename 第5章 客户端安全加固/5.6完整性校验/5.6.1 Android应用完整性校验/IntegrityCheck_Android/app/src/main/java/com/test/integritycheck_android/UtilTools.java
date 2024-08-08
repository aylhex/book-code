package com.test.integritycheck_android;

import android.content.Context;
import android.util.Log;

import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class UtilTools {

    private static final String[] fileArray = {"META-INF/MANIFEST.MF","classes.dex","resources.arsc","AndroidManifest.xml"};

    public static Map getFileCrc(Context context){
        Map result = new HashMap();
        try {
            String path= context.getPackageManager().getApplicationInfo(context.getPackageName(), 0).publicSourceDir;
            ZipFile zf = new ZipFile(path);
            ZipEntry ze;
            for(String item:fileArray){
                ze = zf.getEntry(item);
                if (ze != null){
                    String crcValue = String.valueOf(ze.getCrc());
                    result.put(item, crcValue);
                    Log.i(item+ " CRC ======>",crcValue);
                }
            }
            return result;
        }catch (Exception e){
            e.printStackTrace();
        }
        return null;
    }


    public static Map getAllFileCrc(Context context){
        Map result = new HashMap();
        try {
            String path= context.getPackageManager().getApplicationInfo(context.getPackageName(), 0).publicSourceDir;
            ZipFile zf = new ZipFile(path);
            Enumeration<? extends ZipEntry> entries = zf.entries();

            while (entries.hasMoreElements()) {
                // get the zip entry
                ZipEntry entry = entries.nextElement();
                String crcValue = String.valueOf(entry.getCrc());
                result.put(entry.getName(), crcValue);
                Log.i(entry.getName()+ " CRC ======>",crcValue);
            }
            return result;
        }catch (Exception e){
            e.printStackTrace();
        }
        return null;
    }
}
