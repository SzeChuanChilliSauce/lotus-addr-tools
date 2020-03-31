#include <stdio.h>
#include "filecoin.h"
#include "addr.h"
#include "blake2-impl.h"
#include "blake2.h"
#include "encode.h"


int main(int argc, char** argv) {
    // // 生成私钥
    // PrivateKeyGenerateResponse* res_priv_key = private_key_generate();
    // printf("------------\n");
    // printf("priv_key_len: %ld\n", sizeof(res_priv_key->private_key)/sizeof(res_priv_key->private_key[0]));

    // // 由私钥生成公钥
    // PrivateKeyPublicKeyResponse* res_pub_key = private_key_public_key(res_priv_key->private_key);
    // printf("------------\n");
    // printf("pub_key_len: %ld\n", sizeof(res_pub_key->public_key)/sizeof(res_pub_key->public_key[0]));
   
    // // 公钥字符串
    //  printf("------------\n");
    // char* pub_key_str = byte_2_hex_str(res_pub_key->public_key);
    // printf("pubkey: %s\n", pub_key_str);

    // // 由公钥生成地址
    // free(pub_key_str);


    uint8_t arr[] = {3,138,126,22,34,2,177,53,213,232,
                    134,56,45,198,89,8,93,248,173,85,
                    43,84,50,1,114,88,4,139,46,37,
                    252,132,107,203,86,157,135,96,194,73, 
                    148,74,178,207,145,195,95,50,124};
    blake2b_state stat = {0};
    uint8_t  out[4] = {0};

    blake2b_init(&stat, 4);
    blake2b_update(&stat, arr, sizeof(arr));
    blake2b_final(&stat, out, 4);
    char* checksum = bytes_to_hex_str(out, 4);
    printf("checksum:%s\n", checksum);
    free(checksum);

    uint8_t pubkey_with_checksum[] = {138,126,22,34,2,177,53,213,232,
                               134,56,45,198,89,8,93,248,173,85,
                               43,84,50,1,114,88,4,139,46,37,
                               252,132,107,203,86,157,135,96,194,73,
                               148,74,178,207,145,195,95,50,124,
                               164 ,233 ,232 ,160};
    encode_error_t err;
    uint8_t* raw_addr = base32_encode(pubkey_with_checksum, sizeof(pubkey_with_checksum), &err);
    if (err != OK)
    {
        printf("decode error:%d\n", err);
        return -1;
    }
    printf("raw_addr=%s\n", (char*)raw_addr);

    uint8_t* str = "rj7bmiqcwe25l2eghaw4mwiilx4k2vjlkqzac4syasfs4jp4qrv4wvu5q5qmesmujkzm7eodl4zhzjhj5cqa";
    uint8_t* raw_bytes = base32_decode(str, strlen(str), &err);
    if (err != OK)
    {
        printf("decode error:%d\n", err);
        return -1;
    }

    while (*raw_bytes)
    {
        printf("%d ", *raw_bytes);
        raw_bytes++;
    }
    printf("\n");
}



