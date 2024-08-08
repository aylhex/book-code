package com.test.preventmitm;

import android.content.Context;
import android.os.Build;
import android.text.TextUtils;

import java.io.InputStream;
import java.net.URL;
import java.security.SecureRandom;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;

import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

import okhttp3.CertificatePinner;
import okhttp3.OkHttpClient;

public class PreventMitm {

    /**
     * 检测手机是否开启代理
     * @param context
     * @return
     */
    public static boolean isWifiProxy(Context context) {

        final boolean IS_ICS_OR_LATER = Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH;
        String proxyAddress;
        int proxyPort;

        if (IS_ICS_OR_LATER) {
            proxyAddress = System.getProperty("http.proxyHost");
            String portStr = System.getProperty("http.proxyPort");
            proxyPort = Integer.parseInt((portStr != null ? portStr : "-1"));
        } else {
            proxyAddress = android.net.Proxy.getHost(context);
            proxyPort = android.net.Proxy.getPort(context);
        }
        return (!TextUtils.isEmpty(proxyAddress)) && (proxyPort != -1);
    }

    public OkHttpClient getOkhttpClient(Context mContext){
        return new OkHttpClient.Builder()
                .certificatePinner(getCertPinner(mContext))
                .build();
    }
    // 获取证书
    public static CertificatePinner getCertPinner(Context mContext) {
        Certificate cert = null;
        try {
            CertificateFactory cf = CertificateFactory.getInstance("X.509");
            InputStream input = mContext.getAssets().open("test.cer");
            cert = cf.generateCertificate(input);
            input.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        String pinning = "";
        if (cert != null) {
            pinning = CertificatePinner.pin(cert);
        }
        return new CertificatePinner.Builder()
                .add("www.test.com", pinning)
                .build();
    }


    public HttpsURLConnection getTrustConnection(String url){
        HttpsURLConnection conn = null;
        try{
            SSLContext mSSLContext = SSLContext.getInstance("TLS");
            mSSLContext.init(null, new TrustManager[]{new ClientTrustManager()}, new SecureRandom());
            conn = (HttpsURLConnection) new URL(url).openConnection();
            conn.setDoOutput(true);
            conn.setDoInput(true);
        }catch (Exception e){
            e.printStackTrace();
        }
        return conn;
    }
    class ClientTrustManager implements X509TrustManager{
        @Override
        public void checkClientTrusted(X509Certificate[] x509Certificates, String s) throws
                CertificateException {
        }
        @Override
        public void checkServerTrusted(X509Certificate[] x509Certificates, String s) throws
                CertificateException {
            // 校验服务端证书是否被替换
        }
        @Override
        public X509Certificate[] getAcceptedIssuers() {
            return new X509Certificate[0];
        }
    }
}
