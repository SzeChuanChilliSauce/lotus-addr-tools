#include "ecc_key.h"

// 1 - valid
// 0 - invalid
uint8_t verify_private_key(const secure_key* privkey)
{
    secp256k1_context* secp256k1_ctx = NULL; 
    return secp256k1_ec_seckey_verify(secp256k1_ctx, (const uint8_t*)privkey);
}

//
void private_key_init(secure_key* privkey)
{
    memset(privkey->privkey, 0, ECC_PRIVKEY_LEN);
}

//
void private_key_gen(secure_key* privkey)
{
    if (!privkey)
    {
        return ;
    }
    do {
        assert(random_bytes(privkey->privkey, ECC_PRIVKEY_LEN, 0));
    } while(verify_private_key(privkey->privkey) == 0);
}

//
uint8_t private_key_is_valid(const secure_key* privkey)
{
    if (!privkey) {
        return 0; // false
    }

    return verify_private_key(privkey->privkey);
}

//
uint8_t private_key_verify_pubkey(secure_key* privkey, secp256k1_pubkey* pubkey)
{

}

//
void private_key_cleanse(secure_key* privkey)
{
    memset(privkey->privkey, 0, ECC_PRIVKEY_LEN);
}