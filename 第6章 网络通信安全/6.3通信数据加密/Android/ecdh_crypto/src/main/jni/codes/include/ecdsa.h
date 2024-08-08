//
//  Created by AYL on 19/8/23.
//

#ifndef _ECDSA_
#define _ECDSA_

/** ecdsa sign data
 *  @param  the msg which needs to sign
 *  @param  a pointer to signature without malloc
 *  @return 0 if success otherwise error code
 */ 
int ecdsa_sign(unsigned char* msg, unsigned char** signature);


/** ecdsa verify data
 *  @param  the msg which is signed
 *  @param  the ecdsa signature
 *  @return 0 if success otherwise error code
 */ 
int ecdsa_verify(unsigned char* msg, unsigned char* signature);

#endif