#include <stdio.h>
#include "filecoin.h"
#include "addr.h"
#include "blake2-impl.h"
#include "blake2.h"
#include "encode.h"
#include "secp256k1.h"
#include "secp256k1_ecdh.h"
#include "secp256k1_preallocated.h"
#include "secp256k1_recovery.h"

int test_bls()
{
    // privkey: [19 218 187 104 103 83 240 243 129 236 51 92 31 143 142 194 111 213 96 38 12 30 16 25 145 117 91 200 228 159 89 62]
    // pubkey: [138 126 22 34 2 177 53 213 232 134 56 45 198 89 8 93 248 173 85 43 84 50 1 114 88 4 139 46 37 252 132 107 203 86 157 135 96 194 73 148 74 178 207 145 195 95 50 124]
    // protocol: 3
    // checksum: [164 233 232 160]
    // addr: t3rj7bmiqcwe25l2eghaw4mwiilx4k2vjlkqzac4syasfs4jp4qrv4wvu5q5qmesmujkzm7eodl4zhzjhj5cqa
    BlsAddress addr = {0};
    addr.protocol = 3;
    uint8_t pubkey[] = {138,126,22,34,2,177,53,213,232,
                        134,56,45,198,89,8,93,248,173,85,
                        43,84,50,1,114,88,4,139,46,37,
                        252,132,107,203,86,157,135,96,194,73, 
                        148,74,178,207,145,195,95,50,124};
    memcpy(addr.data, pubkey, PUBLIC_KEY_BYTES);
    address_error_t err;
    uint8_t* addr_str = encode_bls_address('t', &addr, &err);
    if (err != ADDRESS_OK)
    {
        printf("error:%d\n", err);
        return -1;
    }

    printf("bls_addr=%s\n", addr_str);
}

int test_secp256k1()
{
    // privkey: [156 64 40 231 151 216 184 66 150 245 194 72 143 239 178 254 19 139 1 225 163 192 169 4 224 193 177 155 186 28 57 138]
    // pubkey: [4 195 98 172 92 12 212 239 98 97 118 17 122 177 22 107 39 54 136 17 119 72 31 145 51 35 224 35 2 139 152 168 196 163 253 221 32 3 205 186 52 159 133 33 31 104 51 39 130 233 29 170 230 59 141 82 38 31 162 142 228 225 207 244 33]
    // protocol: 1
    // payload: [253 74 140 167 174 219 84 195 31 156 245 173 142 131 131 73 174 69 246 55]
    // checksum:[63 167 184 224]
    // addr: t17vfizj5o3nkmgh446wwy5a4djgxel5rxh6t3rya
    Secp256k1Address addr = { 0 };
    addr.protocol = 1;
    uint8_t pubkey[] = {4, 195 ,98, 172, 92, 12, 212 ,239 ,98 ,97 ,
                        118, 17, 122 ,177 ,22 ,107 ,39 ,54 ,136 ,17,
                        119 ,72 ,31 ,145, 51 ,35 ,224, 35 ,2 ,139 ,
                        152 ,168 ,196 ,163 ,253 ,221, 32, 3, 205, 186 ,
                        52, 159, 133 ,33, 31, 104, 51 ,39 ,130 ,233,
                        29 ,170 ,230 ,59, 141, 82, 38 ,31 ,162 ,142,
                        228, 225 ,207 ,244 ,33};
    memcpy(addr.data, pubkey, 65);
    address_error_t err;
    uint8_t* addr_str = encode_secp256k1_actor_address('t', &addr, &err);
    if (err != ADDRESS_OK)
    {
        printf("error:%d\n", err);
        return -1;
    }

    printf("secp256k1_addr=%s\n", addr_str);
}

int test_genereate_bls_keypair()
{
    // 生成私钥
    PrivateKeyGenerateResponse* res_priv_key = private_key_generate();
    printf("------------\n");
    printf("priv_key_len: %ld\n", sizeof(res_priv_key->private_key)/sizeof(res_priv_key->private_key[0]));

    // 由私钥生成公钥
    PrivateKeyPublicKeyResponse* res_pub_key = private_key_public_key(res_priv_key->private_key);
    printf("------------\n");
    printf("pub_key_len: %ld\n", sizeof(res_pub_key->public_key)/sizeof(res_pub_key->public_key[0]));
}

int test_generate_secp256k1_keypair()
{   
    // unsigned char nonce[32] = {0};
    // unsigned char msg[32] = {0};
    // unsigned char key[32] = {0};
    // unsigned char algo[16] = {0};
    // char* str = "cddscdy";
    // secp256k1_nonce_function pfn;
    // int r = pfn(nonce, msg, key, algo, str, 0);
    // printf("%d\n", r);
    secp256k1_pubkey pubkey = { 0 }; 
    unsigned char prikey[32] = { 0 }; 

    secp256k1_context* context = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
    int ret = secp256k1_ec_pubkey_create(context, &pubkey, prikey); 
    printf("res=%d\n", ret);
    printf("privkey:%s\n", bytes_to_hex_str(prikey, 32));
    printf("pubkey:%s\n", bytes_to_hex_str(pubkey.data, 65));
}

int main(int argc, char** argv) {
    //test_bls();
    // test_secp256k1();
    test_generate_secp256k1_keypair();
}



