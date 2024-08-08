//
//  Created by AYL on 19/8/23.
//

#include <main.h>
#include <tools.h>

/**
 * AES加密函数
*/
JNIEXPORT jstring EXPORT_AES_ENCRYPT(JNIEnv *env, jclass clazz, jstring _plaintext, jstring _key)
{
  char *plaintext = (char *)(*env)->GetStringUTFChars(env, _plaintext, 0);
  unsigned char *key = (unsigned char *)(*env)->GetStringUTFChars(env, _key, 0);
  char *ciphertext = NULL;

  int ret = aes_encrypt(plaintext, key, &ciphertext);
  if (0 != ret)
  {
    return NULL;
  }

  jstring result = (*env)->NewStringUTF(env, ciphertext);
  free(ciphertext);

  return result;
}

/**
 * AES解密函数
 */
JNIEXPORT jstring JNICALL EXPORT_AES_DECRYPT(JNIEnv *env, jclass clazz, jstring _ciphertext, jstring _key)
{
  char *ciphertext = (char *)(*env)->GetStringUTFChars(env, _ciphertext, 0);
  unsigned char *key = (unsigned char *)(*env)->GetStringUTFChars(env, _key, 0);
  char *plaintext = NULL;

  int ret = aes_decrypt(ciphertext, key, &plaintext);
  if (0 != ret)
  {
    return NULL;
  }
  jstring result = (*env)->NewStringUTF(env, plaintext);
  free(plaintext);

  return result;
}

/**
 * 客户端生成公私钥
 */
JNIEXPORT jstring JNICALL EXPORT_CREATE_KEY(JNIEnv *env, jclass clazz)
{
  unsigned char *prikey;
  unsigned char *pubkey;

  int ret = create_key(&prikey, &pubkey);
  if (0 != ret)
  {
    return NULL;
  }

  int prikey_len = strlen((const char *)prikey);
  int pubkey_len = strlen((const char *)pubkey);
  char *ecckey = calloc(prikey_len + pubkey_len + 1, sizeof(char));
  sprintf(ecckey, "%s;%s", prikey, pubkey);
  jstring result = (*env)->NewStringUTF(env, (const char *)ecckey);
  free(ecckey);
  return result;
}


/**
 * 生成密钥 
 * _pubkey 服务端公钥
 * _prikey 客户单私钥
 * _sign   服务端公钥签名
 * return NULL 生成失败
 * return 非NULL 生成成功
 */
JNIEXPORT jstring JNICALL EXPORT_CALC_SECRET(JNIEnv *env, jclass clazz, jstring _pubkey, jstring _prikey, jstring _sign)
{
  unsigned char *pubkey = (unsigned char *)(*env)->GetStringUTFChars(env, _pubkey, 0);
  unsigned char *prikey = (unsigned char *)(*env)->GetStringUTFChars(env, _prikey, 0);
  unsigned char *sign = (unsigned char *)(*env)->GetStringUTFChars(env, _sign, 0);

  unsigned char *sharedkey = NULL;

  // 校验服务端下发的公钥是否合法
  int ret_verify = ecdsa_verify(pubkey, sign);
  if (0 != ret_verify )
  {
    LOGI("ecdsa_verify error");
    return NULL;
  }

  // 生成AES key
  int ret = calc_secret(pubkey, prikey, &sharedkey);
  if (0 != ret)
  {
    LOGI("calc_secret error");
    return NULL;
  }

  jstring result = (*env)->NewStringUTF(env, (const char *)sharedkey);
  free(sharedkey);

  return result;
}


// JNIEXPORT jstring JNICALL EXPORT_ECDSA_SIGN(JNIEnv *env, jobject _jobject, jstring _msg) 
// {
//     unsigned char *msg = (unsigned char *)(*env)->GetStringUTFChars(env, _msg, 0);

//     unsigned char *sign = NULL;
//     int ret = ecdsa_sign(msg, &sign);
//     if(0 != ret) 
//     {

//       return NULL;
//     }

//     jstring result = (*env)->NewStringUTF(env, (const char *)sign);

//     free(sign);

//     return result;

//   }
