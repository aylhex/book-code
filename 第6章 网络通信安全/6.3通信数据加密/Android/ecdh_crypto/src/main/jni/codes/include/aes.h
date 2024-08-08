//
//  Created by AYL on 19/8/23.
//

#ifndef _AES_
#define _AES_



#define IV_LENGTH 16
#define AESKEY_LENGTH 32

/** Encrypt data via aes-256-cbc
 *  @param  plaintext    the data which is perpared to encrypt
 *  @param  key          aes key        
 *  @param  encryptdata  a pointer to encrypted data without malloc
 *  @return 0 if success otherwise error code
 */
int aes_encrypt(char *plaintext, unsigned char *key, char **encryptdata);



/** Decrypt data via aes-256-cbc
 *  @param  ciphertext    the data which is perpared to decrypt
 *  @param  key          aes key        
 *  @param  decryptdata  a pointer to decrypted data without malloc
 *  @return 0 if success otherwise error code
 */
int aes_decrypt(char *ciphertext, unsigned char *key, char **decryptdata);

#endif

