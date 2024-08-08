package com.device.risk.utils.tools;

import java.io.ByteArrayInputStream;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import javax.security.auth.x500.X500Principal;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;

import org.json.JSONException;
import org.json.JSONObject;
/**
 * 签名信息
 */
public class SignUtils {

    public static JSONObject getSignaturesMsg(Context  mContext){
        JSONObject mJson = new JSONObject();
        try {
            if (isDebuggable(mContext)){
                mJson.put("signDebug", 1);
            }else {
                mJson.put("signDebug", 0);
            }
            JSONObject json = signatureName(mContext);
            mJson.put("signName", json.get("signName"));
            mJson.put("signNumber", json.get("signNumber"));
            mJson.put("signDN", json.get("signDN"));
        } catch (JSONException e) {
            MLog.printStackTrace(e);
        }
        return mJson;
    }


    /**
     * 检测应用程序是否是用"CN=Android Debug,O=Android,C=US"的debug信息来签名的
     * 判断签名是debug签名还是release签名
     */
    private final static X500Principal DEBUG_DN = new X500Principal(
            "CN=Android Debug,O=Android,C=US");

    /**
     * 判断签名是debug签名还是release签名
     * @return true = 开发(debug.keystore)，false = 上线发布（非.android默认debug.keystore）
     */
    private static boolean isDebuggable(Context  mContext) {
        // 判断是否默认key(默认是)
        boolean debuggable = false;

        try {
            PackageInfo packageInfo = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] signatures = packageInfo.signatures;
            for (int i = 0, c = signatures.length; i < c; i++) {
                CertificateFactory cf = CertificateFactory.getInstance("X.509");
                ByteArrayInputStream stream = new ByteArrayInputStream(signatures[i].toByteArray());
                X509Certificate cert = (X509Certificate) cf.generateCertificate(stream);
                debuggable = cert.getSubjectX500Principal().equals(DEBUG_DN);
                if (debuggable) {
                    break;
                }
            }
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        return debuggable;
    }

    private static JSONObject signatureName(Context  mContext){
        JSONObject mJson = new JSONObject();
        try {
            PackageInfo packageInfo = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] signatures = packageInfo.signatures;

            for (int i = 0, c = signatures.length; i < c; i++) {
                CertificateFactory cf = CertificateFactory.getInstance("X.509");
                ByteArrayInputStream stream = new ByteArrayInputStream(signatures[i].toByteArray());
                X509Certificate cert = (X509Certificate) cf.generateCertificate(stream);
                String pubKey = cert.getPublicKey().toString();   //公钥
                String signNumber = cert.getSerialNumber().toString();
                mJson.put("signName", cert.getSigAlgName());
                mJson.put("signNumber", signNumber);
                mJson.put("signDN", cert.getSubjectDN().toString());
            }
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        return mJson;
    }
}
