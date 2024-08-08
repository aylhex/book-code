//
//  Created by AYL on 19/8/23.
//

#include <stddef.h>
#include <string.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <ecdsa.h>
#include <base64.h>
#include <error.h>

int get_ec_key_via_pubkey(EC_KEY **ec_key) {
	unsigned char *b64decode_pubkey;
	size_t b64decode_pubkey_len;

    char* ecdsa_pubkey = "testkey";
	*ec_key = EC_KEY_new_by_curve_name(NID_secp384r1);

	b64decode_pubkey = base64_decode((unsigned char*)ecdsa_pubkey, strlen((const char *)ecdsa_pubkey), &b64decode_pubkey_len);
	if(b64decode_pubkey == NULL) {
		return ECDSA_PUBKEY_B64DECODE_ERROR;
	}

	*ec_key = o2i_ECPublicKey(ec_key, (const unsigned char **)&b64decode_pubkey, b64decode_pubkey_len);

	if(*ec_key == NULL) {
        free(b64decode_pubkey);
		return ECDSA_PUBKEY_ECKEY_GENERATE_ERROR;
	}
	
	return 0;
}

int do_sign(unsigned char *msg, EC_KEY *ec_key, unsigned char **sign) {
    ECDSA_SIG *ecdsa_sig;
    int ecdsa_sign_len;

    ecdsa_sig = ECDSA_do_sign(msg, strlen((const char *)msg), ec_key);
    if(ecdsa_sig == NULL) {
        return ECDSA_DO_SIGN_ERROR;
    }

    ecdsa_sign_len = i2d_ECDSA_SIG(ecdsa_sig, NULL);
    if(ecdsa_sign_len <0) {
        ECDSA_SIG_free(ecdsa_sig);
        return ECDSA_SIGN_LEN_GENERATE_ERROR;

    }

    *sign = calloc(ecdsa_sign_len + 1, sizeof(char));

    unsigned char *sign_buf = *sign;

    ecdsa_sign_len = i2d_ECDSA_SIG(ecdsa_sig, &sign_buf);


    if(ecdsa_sign_len <0) {
        ECDSA_SIG_free(ecdsa_sig);
        return ECDSA_SIGN_GENERATE_ERROR;

    }
    ECDSA_SIG_free(ecdsa_sig);

    return ecdsa_sign_len;
}

int ecdsa_verify(unsigned char *msg, unsigned char *signature) {
    EC_KEY *ec_key;
    ECDSA_SIG *ecdsa_sig;

    unsigned char *sign;
    unsigned char *b64decode_sign;

    size_t b64decode_sign_len;

    int ret;

    ret = get_ec_key_via_pubkey(&ec_key);
    if(0 != ret) {
        EC_KEY_free(ec_key);

        return ret;
    }

    b64decode_sign = base64_decode(signature, strlen((const char *)signature), &b64decode_sign_len);
    if(b64decode_sign == NULL) {
        return ECDSA_SIGN_B64DECODE_ERROR;
    }

    unsigned char *sign_buf = b64decode_sign;

    ecdsa_sig = d2i_ECDSA_SIG(NULL, (const unsigned char **)&sign_buf, b64decode_sign_len);
    if(ecdsa_sig == NULL) {
        free(b64decode_sign);
        EC_KEY_free(ec_key);

        return ECDSA_SIGN_RECOVER_ERROR;
    }

    ret = ECDSA_do_verify(msg, strlen((const char *)msg), ecdsa_sig, ec_key);
    if(1 != ret) {
        EC_KEY_free(ec_key);
        ECDSA_SIG_free(ecdsa_sig);
        free(b64decode_sign);

        return ECDSA_DO_VERIFY_ERROR;
    }
    
    free(b64decode_sign);
    ECDSA_SIG_free(ecdsa_sig);
    EC_KEY_free(ec_key);

    return 0;
}