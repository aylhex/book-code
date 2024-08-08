//
//  Created by AYL on 19/8/23.
//

#ifndef _ECDH_
#define _ECDH_

#define SECRET_LENGTH 48

/** create a new pair of ecc public key & private key
 *  @param  prikey  a pointer to private key without malloc
 *  @param  pubkey  a pointer to public key without malloc
 *  @return 0 if success otherwise error code
 */
int create_key(unsigned char **prikey, unsigned char **pubkey);


/** calculate shared secret via ecc public key A & private key B
 *  @param  pubkey  ecc public key of A side
 *  @param  prikey  ecc private key of B side
 *  @param  secret  a pointer to shared secret without malloc
 *  @return 0 if success otherwise error code
 */
int calc_secret(unsigned char *pubkey, unsigned char *prikey, unsigned char** secret);

#endif