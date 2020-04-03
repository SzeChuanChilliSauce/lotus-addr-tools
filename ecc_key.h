#ifndef LOTUS_ADDR_TOOLS_ECC_KEY_H
#define LOTUS_ADDR_TOOLS_ECC_KEY_H

#include <stdlib.h>
#include <string.h>
#include <bits/stdint-uintn.h>
#include <assert.h>
#include "secp256k1.h"
#include "random.h"

#define ECC_PRIVKEY_LEN 32
#define SECP256K1_PUBKEY_LEN 65

// 私钥
typedef struct ecc_key__ {
    uint8_t privkey[ECC_PRIVKEY_LEN];
} secure_key;

// 初始化私钥
void private_key_init(secure_key* privkey);

// 生成私钥
void private_key_gen(secure_key* privkey);

// 私钥是否正确
uint8_t private_key_is_valid(const secure_key* privkey);

// 私钥对应的公钥是否正确
uint8_t private_key_verify_pubkey(secure_key* privkey, secp256k1_pubkey* pubkey);

// 清空私钥
void private_key_cleanse(secure_key* privkey);


#endif 