//
//  Created by AYL on 19/8/23.
//

#include <stddef.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <aes.h>
#include <base64.h>
#include <error.h>
#include <tools.h>


int generate_iv(unsigned char *key, unsigned char **iv) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    unsigned char digest[IV_LENGTH + 1];

    SHA1(key, strlen((const char *)key), hash);

    *iv = calloc(IV_LENGTH + 1, sizeof(char));

    for(int i = 0; i < 8; i++)
    {
         sprintf((char *)&digest[i*2], "%02x", (unsigned int)hash[i]);
    }    

    memcpy(*iv, digest, IV_LENGTH + 1);

    return 0;

}

int generate_aeskey(unsigned char *key, unsigned char **aeskey) {
    char chaos[] = "test";

    unsigned char hash[SHA512_DIGEST_LENGTH];
    unsigned char digest[AESKEY_LENGTH + 1];

    SHA512(key, strlen((const char *)key), hash);
    
    *aeskey = calloc(AESKEY_LENGTH + 1, sizeof(char));

    for(int i = 0; i < 16; i++) {
         sprintf((char *)&digest[i*2], "%02x", (unsigned int)hash[i]);
    }

    for(int i = 0; i < 3; i++) {
        digest[i] = chaos[i];
        digest[AESKEY_LENGTH - i] = chaos[2 + i];
    }

    memcpy(*aeskey, digest, AESKEY_LENGTH + 1);

    return 0;

}


int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *aeskey,
            unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx = NULL;

    int len;

    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) {
        return AES_ENCRYPT_CTX_ERROR;

    }

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aeskey, iv)) {
        return AES_ENCRYPT_INIT_ERROR;
    }

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
        return AES_ENCRYPT_UPDATE_ERROR;
    }
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        return AES_ENCRYPT_FINAL_ERROR;
    }

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}



int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *aeskey,
            unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx = NULL;

    int len;

    int plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) {
        return AES_DECRYPT_CTX_ERROR;
    }

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aeskey, iv)) {
        return AES_DECRYPT_INIT_ERROR;
    }

    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
        return AES_DECRYPT_DECRYPT_ERROR;
    }

    plaintext_len = len;

    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        return AES_DECRYPT_FINAL_ERROR;
    }

    //LOGI("decrypt_xxx=%s", plaintext);

    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}


int aes_encrypt(char *plaintext, unsigned char *key, char **encryptdata) {
    int plaintext_len;
    int real_ciphertext_len;
    unsigned char *iv;
    unsigned char *aeskey;
    unsigned char *ciphertext;
    unsigned char *real_ciphertext;
    unsigned char *chaos_data;
    unsigned char *b64_ciphertext;
    unsigned char hash[SHA_DIGEST_LENGTH];

    generate_aeskey(key, &aeskey);

    generate_iv(key, &iv);

    plaintext_len = strlen(plaintext);

    ciphertext = calloc((plaintext_len /16 + 1) * 16, sizeof(char));
    
    real_ciphertext_len = encrypt((unsigned char *)plaintext, plaintext_len, aeskey, iv, ciphertext);

    if(real_ciphertext_len < 0) {
        free(iv);
        free(aeskey);
        free(ciphertext);
        free(real_ciphertext);
        free(chaos_data);
        free(b64_ciphertext);
        return AES_ENCRYPT_ERROR;
    }

    real_ciphertext = calloc(real_ciphertext_len + 1 , sizeof(char));

    memcpy(real_ciphertext, ciphertext, real_ciphertext_len);
    
    SHA1(real_ciphertext, real_ciphertext_len, hash);

    chaos_data = calloc(real_ciphertext_len + SHA_DIGEST_LENGTH + 1, sizeof(char));

    memcpy(chaos_data, hash, SHA_DIGEST_LENGTH);
    memcpy(chaos_data + SHA_DIGEST_LENGTH, real_ciphertext, real_ciphertext_len);


    b64_ciphertext = base64_encode(chaos_data,  real_ciphertext_len + SHA_DIGEST_LENGTH, NULL);
    
    if(b64_ciphertext == NULL) {
        free(iv);
        free(aeskey);
        free(ciphertext);
        free(real_ciphertext);
        free(chaos_data);
        free(b64_ciphertext);
        return AES_ENCRYPT_B64ENCODE_ERROR;
    }

    *encryptdata = calloc(strlen((const char *)b64_ciphertext) + 1, sizeof(char));
    
    memcpy(*encryptdata, b64_ciphertext, strlen((const char *)b64_ciphertext));

    free(iv);
    free(aeskey);
    free(ciphertext);
    free(real_ciphertext);
    free(chaos_data);
    free(b64_ciphertext);

    return 0;

}

int aes_decrypt(char *ciphertext, unsigned char *key, char **decryptdata) {

    size_t b64decode_len;
    int decrypt_len;
    
    unsigned char *iv;
    unsigned char *aeskey;
    unsigned char *b64decode_text;
    unsigned char *encrypt_data;
    unsigned char *decrypt_data;

    generate_aeskey(key, &aeskey);

    generate_iv(key, &iv);

    b64decode_text = base64_decode((unsigned char *)ciphertext, strlen((const char *)ciphertext), &b64decode_len);

    if(b64decode_text == NULL) {
        free(iv);
        free(aeskey);
        free(b64decode_text);
        free(encrypt_data);
        free(decrypt_data);
    
        return AES_DECRYPT_B64DECODE_ERROR;
    }

    encrypt_data = calloc(b64decode_len - SHA_DIGEST_LENGTH + 1, sizeof(char));

    memcpy(encrypt_data, b64decode_text + SHA_DIGEST_LENGTH, b64decode_len - SHA_DIGEST_LENGTH);

    decrypt_data = calloc(b64decode_len + 1, sizeof(char));

    decrypt_len = decrypt(encrypt_data, b64decode_len - SHA_DIGEST_LENGTH, aeskey, iv, decrypt_data);
    if(decrypt_len < 0) {
        free(iv);
        free(aeskey);
        free(b64decode_text);
        free(encrypt_data);
        free(decrypt_data);
        return AES_DECRYPT_ERROR;
    }

    *decryptdata = calloc(decrypt_len + 1, sizeof(char));

    memcpy(*decryptdata, decrypt_data, decrypt_len);

    free(iv);
    free(aeskey);
    free(b64decode_text);
    free(encrypt_data);
    free(decrypt_data);

    return 0;

}

