//
//  Created by AYL on 19/8/23.
//

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/obj_mac.h>
#include <ecdh.h>
#include <base64.h>
#include <error.h>

int init_key(EC_KEY **ec_key)
 {
	int ret;

	*ec_key = EC_KEY_new_by_curve_name(NID_secp384r1);
	
	if(*ec_key == NULL)
	{
		return EC_KEY_INIT_ERROR;
	}

	if(1 != EC_KEY_generate_key(*ec_key))
	{
		return EC_KEY_GENERATE_ERROR;
	}

	return 0;
}

int create_prikey(EC_KEY *ec_key, unsigned char *prikey)
{
	int ret = i2d_ECPrivateKey(ec_key, &prikey);
	if(0 == ret)
	{
		return EC_PRIKEY_GENERATE_ERROR;
	}
	return 0;
}

int create_pubkey(EC_KEY *ec_key, unsigned char *pubkey)
{
	int ret = i2o_ECPublicKey(ec_key, &pubkey);
	if(0 == ret)
	{
		return EC_PUBKEY_GENERATE_ERROR;
	}
	return 0;
}

int create_key(unsigned char **prikey, unsigned char **pubkey)
{
	EC_KEY *ec_key;

	int ret;
	int ec_prikey_len;
	int ec_pubkey_len;

	unsigned char *ec_prikey;
	unsigned char *ec_pubkey;

	ret = init_key(&ec_key);
	if(0 != ret)
	{
		return ret;
	}

	ec_prikey_len = i2d_ECPrivateKey(ec_key, NULL);
	if(0 == ec_prikey_len)
	{
		EC_KEY_free(ec_key);

		return EC_KEY_PRIKEY_GENERATE_ERROR; 
	}
	
	ec_prikey = calloc(ec_prikey_len + 1, sizeof(char));

	ret = create_prikey(ec_key, ec_prikey);
	if(0 != ret)
	{
		EC_KEY_free(ec_key);
		return ret;
	}

	*prikey = base64_encode(ec_prikey, ec_prikey_len, NULL);

	if(*prikey == NULL)
	{
		free(ec_prikey);
		EC_KEY_free(ec_key);

		return EC_PRIKEY_B64ENCDOE_ERROR;
	}

	ec_pubkey_len = i2o_ECPublicKey(ec_key, NULL);
	if(0 == ec_pubkey_len)
	{
		free(ec_prikey);
		EC_KEY_free(ec_key);

		return EC_KEY_PUBKEY_GENERATE_ERROR; 
	}
	ec_pubkey = calloc(ec_pubkey_len + 1, sizeof(char));
	ret = create_pubkey(ec_key, ec_pubkey);

	if(0 != ret)
	{
		free(ec_prikey);
		free(ec_pubkey);
		EC_KEY_free(ec_key);
		return ret;
	}

	*pubkey = base64_encode(ec_pubkey, ec_pubkey_len, NULL);
	if(*pubkey == NULL)
	{
		free(ec_prikey);
		free(ec_pubkey);
		EC_KEY_free(ec_key);
		return EC_PUBKEY_B64ENCDOE_ERROR;
	}


	free(ec_prikey);
	free(ec_pubkey);
	EC_KEY_free(ec_key);

	return 0;

}

int get_ss_ec_key(unsigned char* prikey, EC_KEY **ec_key_pri)
{
	unsigned char *b64decode_prikey;
	size_t b64decode_prikey_len;

	b64decode_prikey = base64_decode(prikey, strlen((const char *)prikey), &b64decode_prikey_len);
	if(b64decode_prikey == NULL)
	{
		return EC_SECRET_PRIKEY_B64DECODE_ERROR;
	}

	*ec_key_pri = d2i_ECPrivateKey(NULL, (const unsigned char **)&b64decode_prikey, b64decode_prikey_len);
	if(*ec_key_pri == NULL)
	{
		return EC_SECRET_PRIKEY_GENERATE_ERROR;
	}

	return 0;

}

int get_ss_ec_point(unsigned char* pubkey, 	EC_KEY **ec_key_pub, EC_POINT **ec_point)
{
	unsigned char *b64decode_pubkey;
	size_t b64decode_pubkey_len;
	*ec_key_pub = EC_KEY_new_by_curve_name(NID_secp384r1);

	b64decode_pubkey = base64_decode(pubkey, strlen((const char *)pubkey), &b64decode_pubkey_len);
	if(b64decode_pubkey == NULL)
	{
		return EC_SECRET_PUBKEY_B64DECODE_ERROR;
	}

	*ec_key_pub = o2i_ECPublicKey(ec_key_pub, (const unsigned char **)&b64decode_pubkey, b64decode_pubkey_len);
	if(*ec_key_pub == NULL)
	{
		return EC_SECRET_PUBKEY_GENERATE_ERROR;
	}

	*ec_point = (EC_POINT *)EC_KEY_get0_public_key(*ec_key_pub);
	if(*ec_point == NULL)
	{
		return EC_SECRET_ECPOINT_GENERATE_ERROR;

	}
	return 0;
}


int calc_secret(unsigned char *pubkey, unsigned char *prikey, unsigned char** secret)
{
	EC_KEY *ec_key_pri;
	EC_KEY *ec_key_pub;
	EC_POINT *ec_point;

	unsigned char *ss;
	int ss_len;
	int ret;

	ret = get_ss_ec_key(prikey, &ec_key_pri);
	if(0 != ret)
	{
		EC_KEY_free(ec_key_pri);
		return ret;
	}

	ret = get_ss_ec_point(pubkey, &ec_key_pub, &ec_point);
	if(0 != ret)
	{
		EC_KEY_free(ec_key_pri);
		EC_KEY_free(ec_key_pub);
		return ret;
	}

	ss = OPENSSL_malloc(SECRET_LENGTH);
	ss_len = ECDH_compute_key(ss, SECRET_LENGTH, ec_point, ec_key_pri, NULL);
	if(ss_len < 0) {
		free(ss);
		EC_KEY_free(ec_key_pri);
		EC_KEY_free(ec_key_pub);
		return EC_SECRET_GENERATE_ERROR;
	}
	*secret = base64_encode(ss, ss_len, NULL);
	OPENSSL_free(ss);
	EC_KEY_free(ec_key_pri);
	EC_KEY_free(ec_key_pub);
	return 0;
}