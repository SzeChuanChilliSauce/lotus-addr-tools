#include <stdio.h>
#include "filecoin.h"
#include "addr.h"
#include "blake2-impl.h"
#include "blake2.h"
#include "encode.h"

int test_bls()
{
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

int test_secp256k1()
{
    /* 
    privkey--> b1ea463b6ab3a9814bced39c3a62b25271f50ff1164c91c121f35bab63100697
    privkey--> [177 234 70 59 106 179 169 129 75 206 211 156 58 98 178 82 113 245 15 241 22 76 145 193 33 243 91 171 99 16 6 151]
    pubkey--> 04a66a6607db35487234e8a7b4beb3d0f7b9b477984a4f8418f99613980b6b739721c6a342ac8cc77f3aa823b828c9a3c4c1a4303c32cf7b254a7aa62a874e5ecb
    pubkey--> [4 166 106 102 7 219 53 72 114 52 232 167 180 190 179 208 247 185 180 119 152 74 79 132 24 249 150 19 152 11 107 115 151 33 198 163 66 172 140 199 127 58 168 35 184 40 201 163 196 193 164 48 60 50 207 123 37 74 122 166 42 135 78 94 203]
    checksum-->:  [230 214 196 169] e6d6c4a9
    addr: t1uwtasmeoadscsfudqqvpqnh5mqy4bjevi6iybji
    uwtasmeoadscsfudqqvpqnh5mqy4bjev i6iybji
    uwtasmeoadscsfudqqvpqnh5mqy4bjev aaaaaaa
    payload: [165 166 9 48 142 0 228 41 22 131 132 42 248 52 253 100 49 192 164 149]
    */
    // 第一个字节: 协议号
    // 其他: 公钥
    uint8_t arr[] = { 1, // protocol
                    4, 166, 106, 102, 7, 219, 53, 72, 114, 52,
                    232, 167, 180, 190, 179, 208, 247, 185, 180, 119,
                    152, 74, 79, 132, 24, 249, 150, 19, 152, 11, 
                    107, 115, 151, 33, 198, 163, 66, 172, 140, 199, 
                    127, 58, 168, 35, 184, 40, 201, 163, 196, 193, 
                    164, 48, 60, 50, 207, 123, 37, 74, 122, 166, 
                    42, 135, 78, 94, 203};

    blake2b_state stat = { 0 };
    uint8_t  out[4] = { 0 };

    blake2b_init(&stat, sizeof(out));
    blake2b_update(&stat, arr, sizeof(arr));
    blake2b_final(&stat, out, sizeof(out));

    char* checksum = bytes_to_hex_str(out, sizeof(out));
    printf("checksum:%s\n", checksum);
    free(checksum);
    

    uint8_t pubkey[] = {4, 166, 106, 102, 7, 219, 53, 72, 114, 52,
                        232, 167, 180, 190, 179, 208, 247, 185, 180, 119,
                        152, 74, 79, 132, 24, 249, 150, 19, 152, 11, 
                        107, 115, 151, 33, 198, 163, 66, 172, 140, 199, 
                        127, 58, 168, 35, 184, 40, 201, 163, 196, 193, 
                        164, 48, 60, 50, 207, 123, 37, 74, 122, 166, 
                        42, 135, 78, 94, 203};
    uint8_t pubkey_hash[20] = { 0 };
    blake2b_init(&stat, sizeof(pubkey_hash));
    blake2b_update(&stat, pubkey, sizeof(pubkey));
    blake2b_final(&stat, pubkey_hash, sizeof(pubkey_hash));

    for (int i = 0; i < sizeof(pubkey_hash); i++)
    {
        printf("%d ", pubkey_hash[i]);
    }
    printf("\n");

    // pubkey_hash(20) + checksum(4) = 24字节
    uint8_t payload[] = {165, 166, 9, 48, 142, 0, 228, 41, 22, 131, 132, 42, 248, 52, 253, 100, 49, 192, 164, 149, 230, 214, 196, 169};
    printf("payload_len=%d\n", sizeof(payload));
    encode_error_t err;
    uint8_t* raw_addr = base32_encode(payload, sizeof(payload), &err);
    if (err != OK)
    {
        printf("decode error:%d\n", err);
        return -1;
    }
    printf("raw_addr=%s\n", (char*)raw_addr);

    char* str = "vi6iybji";
    uint8_t* raw_bytes = base32_decode(str, strlen(str), &err);
    if (err != OK)
    {
        printf("decode error:%d\n", err);
        return -1;
    }

    for (int i = 0; i < sizeof(payload); i++)
    {   
        printf("%d ", raw_bytes[i]);
    }
    printf("\n");
}


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

    test_secp256k1();

}



