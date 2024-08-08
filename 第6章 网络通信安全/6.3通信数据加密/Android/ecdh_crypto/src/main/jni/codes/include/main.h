//
//  Created by AYL on 19/8/23.
//

#ifndef _MAIN_
#define _MAIN_
#include <onload.h>
#include <aes.h>
#include <base64.h>
#include <ecdh.h>
#include <ecdsa.h>
#include <error.h>

// AES加密
jstring EXPORT_AES_ENCRYPT(JNIEnv *env, jclass clazz, jstring _plaintext, jstring _key);
// AES解密
jstring EXPORT_AES_DECRYPT(JNIEnv *env, jclass clazz, jstring _ciphertext, jstring _key);
// 客户端生成公私钥
jstring EXPORT_CREATE_KEY(JNIEnv *env, jclass clazz);

// 生成签名
jstring EXPORT_ECDSA_SIGN(JNIEnv *env, jclass clazz, jstring _msg);

// 生成AESKEY
jstring EXPORT_CALC_SECRET(JNIEnv *env , jclass clazz, jstring _serverpubkey, jstring _clientpubkey,  jstring _sign);

#endif