package com.ecdh.crypto;

import android.content.Context;
import android.content.SharedPreferences;

import com.ecdh.crypto.jni.Native;


public class MCrypto {

    private static MCrypto instance;

    private static String privatekey = null;
    private static String publicKey = null;
    private static String sharedKey = null;

    private MCrypto() { }

    public static void init(Context mContext) {
        synchronized (Native.class) {
            MContext.init(mContext);

            String result = Native.getInstance().o();
            String[] keyArray = result.split(";");

//            MLog.i("private key==> " + keyArray[0]);
//            MLog.i("public key==> " + keyArray[1]);
            privatekey = keyArray[0];
            publicKey = keyArray[1];
        }
    }

    public static MCrypto getInstance() {
        synchronized (Native.class) {
            if (instance == null) {
                instance = new MCrypto();
            }
            return instance;
        }
    }

    /**
     * 存储 生成的AES Key
     * @param key
     */
    private void setSharedKey(Context mContext, String key) {
        sharedKey = key;
        //步骤1：创建一个SharedPreferences对象
        SharedPreferences sharedPreferences = mContext.getSharedPreferences("ssconfig", Context.MODE_PRIVATE);
        //步骤2： 实例化SharedPreferences.Editor对象
        SharedPreferences.Editor editor = sharedPreferences.edit();
        //步骤3：将获取过来的值放入文件
        editor.putString("uuid", key);
        //步骤4：提交
        editor.commit();
    }

    public String getSharedKey(Context mContext) {
        if (sharedKey != null) {
            return sharedKey;
        } else {
            SharedPreferences sharedPreferences = mContext.getSharedPreferences("ssconfig", Context.MODE_PRIVATE);
            String key = sharedPreferences.getString("uuid", "error");
            if (key.equals("error")) {
                return null;
            }
            return key;
        }
    }

    /**
     * 获取客户端公钥
     * @return
     */
    public String getPublicKey() {
        return publicKey;
    }

    /**
     * 生成AES密钥
     * @param publickey 服务端公钥
     * @param esdsasign 服务端公钥
     * @return false 生成失败
     * @return true  生成成功
     */
    public boolean createSharedSecretKey(String publickey, String esdsasign) {

        String privatekey = this.privatekey;

        if (privatekey == null || privatekey.equals("error")) {
            return false;
        }
        String sharedKey = Native.getInstance().a(publickey, privatekey, esdsasign);
        setSharedKey(MContext.getContext(), sharedKey);
        return true;
    }

    /**
     * 返回加密数据
     * @param plaintext 明文
     * @return null   加密失败
     * @return 非null 加密成功
     */
    public synchronized String getCryptoData(Context mContext, String plaintext) {
        try {
            String sharedkey = getSharedKey(mContext);
            if (sharedkey == null || plaintext == null){
                return null;
            }
            String result = Native.getInstance().m(plaintext, sharedkey);
//            Log.i("mmhex Crypto decrypt", "cryptokey====>"+sharedkey+"result===>"+result);
            return result;
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        return null;
    }


    /**
     * 返回加密数据
     * @param plaintext 明文
     * @return null   加密失败
     * @return 非null 加密成功
     */
    public synchronized String getInitCryptoData(String plaintext) {
        try {
            if (publicKey == null || plaintext == null){
                return null;
            }
            //MLog.i("publicKey==> " + publicKey);
            String value = MContext.MD5(publicKey).toLowerCase();
            //MLog.i("publicKey MD5==> " + value);
            String result = Native.getInstance().m(plaintext, value);
            return result;
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        return null;
    }

    /**
     * 返回解密数据
     * @param ciphertext 密文
     * @return null    解密失败
     * @return 非null  解密成功
     */
    public synchronized String getInitDecryptData(String ciphertext) {
        try {
            if (publicKey == null || ciphertext == null){
                return null;
            }
            String value = MContext.MD5(publicKey);
            String result = Native.getInstance().h(ciphertext, value);
            return result;
        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        return null;
    }
    /**
     * 返回解密数据
     * @param ciphertext 密文
     * @return null    解密失败
     * @return 非null  解密成功
     */
    public synchronized String getDecryptData(Context mContext, String ciphertext) {
        try {
            String sharedkey = getSharedKey(mContext);
            if (sharedkey == null || ciphertext == null) {
                return null;
            }

            int count = 0;
            String result = null;
            while (count < 2){
                result = Native.getInstance().h(ciphertext, sharedkey);
                count++;
                if (result != null){
                    break;
                }
            }

            return result;

        } catch (Exception e) {
            MLog.printStackTrace(e);
        }
        return null;
    }

    /**
     * 加解密函数
     * @param sharedkey
     * @param data
     * @param type   1标识加密，2标识解密
     * @return
     */
    public static synchronized String Crypto(String sharedkey, String data, int type) {

        if (sharedkey == null || data== null){
            return null;
        }

        switch (type) {
            case 1:
                try {
                    int count = 0;
                    String encode = null;
                    while (count < 2){
                        encode = Native.getInstance().m(data, sharedkey);
                        count++;
                        if (encode != null){
                            break;
                        }
                    }
                    return encode;
                } catch (Exception e) {
                    MLog.printStackTrace(e);
                }
                break;

            case 2:
                try {
                    int count = 0;
                    String decode = null;
                    while (count < 2){
                        decode = Native.getInstance().h(data, sharedkey);
                        count++;
                        if (decode != null){
                            break;
                        }
                    }
                    return decode;
                } catch (Exception e) {
                    MLog.printStackTrace(e);
                }
                break;

            default:
                break;
        }

        return null;
    }


}
