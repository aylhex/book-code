package com.ecdh.crypto.jni;


public class Native {
    static {
        System.loadLibrary("ecdh");
    }

    private static Native instance;

    private Native(){}

    public static Native getInstance(){
        synchronized (Native.class){
            if(instance == null){
                instance = new Native();
            }
            return instance;
        }
    }

    // 密钥生成算法create_key
    public native String o();
    // aes_encrypt
    public native String m(String plaintext, String key);
    // aes_decrypt
    public native String h(String ciphertext, String key);
    // calc_secret
    public native String a(String clientPrivateKey, String serverPublickey, String esdsaSign);

    // 生成签名
    public native String ecdsaSign(String msg);

}
