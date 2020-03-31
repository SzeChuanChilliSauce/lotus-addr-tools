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
    // pubkey---> [4 195 98 172 92 12 212 239 98 97 118 17 122 177 22 107 39 54 136 17 119 72 31 145 51 35 224 35 2 139 152 168 196 163 253 221 32 3 205 186 52 159 133 33 31 104 51 39 130 233 29 170 230 59 141 82 38 31 162 142 228 225 207 244 33]
    // protocol---> 1
    // payload---> [253 74 140 167 174 219 84 195 31 156 245 173 142 131 131 73 174 69 246 55]
    // checksum---> [63 167 184 224]
    // t17vfizj5o3nkmgh446wwy5a4djgxel5rxh6t3rya

    uint8_t pubkey[] = {4, 195 ,98, 172, 92, 12, 212 ,239 ,98 ,97 ,
                        118, 17, 122 ,177 ,22 ,107 ,39 ,54 ,136 ,17,
                        119 ,72 ,31 ,145, 51 ,35 ,224, 35 ,2 ,139 ,
                        152 ,168 ,196 ,163 ,253 ,221, 32, 3, 205, 186 ,
                        52, 159, 133 ,33, 31, 104, 51 ,39 ,130 ,233,
                        29 ,170 ,230 ,59, 141, 82, 38 ,31 ,162 ,142,
                        228, 225 ,207 ,244 ,33};

    uint8_t payload[20] = { 0 };

    blake2b_state stat = {0};
    blake2b_init(&stat ,sizeof(payload));
    blake2b_update(&stat, pubkey, sizeof(pubkey));
    blake2b_final(&stat, payload, sizeof(payload));

    for (int i = 0; i < sizeof(payload); i++)
    {
        printf("%d ", payload[i]);
    }
    printf("\n");

    uint8_t checksum[4] = {0};
    uint8_t proto_payload[] = {1, 253, 74, 140, 167, 174 ,219 ,84, 195 ,31 ,156 ,245 ,173 ,142 ,131 ,131, 73, 174, 69 ,246 ,55};
    blake2b_init(&stat, sizeof(checksum));
    blake2b_update(&stat, proto_payload, sizeof(proto_payload));
    blake2b_final(&stat, checksum, sizeof(checksum));

    for (int i = 0; i < sizeof(checksum); i++)
    {
        printf("%d ", checksum[i]);
    }
    printf("\n");

    uint8_t payload_checksum[] = {253, 74, 140, 167, 174 ,219 ,84, 195 ,31 ,156 ,245 ,173 ,142 ,131 ,131, 73, 174, 69 ,246 ,55,63 ,167 ,184 ,224};

    encode_error_t err;
    uint8_t* raw_addr = base32_encode(payload_checksum, sizeof(payload_checksum), &err);
    if (err != OK)
    {
        printf("decode error:%d\n", err);
        return -1;
    }
    printf("raw_addr=%s\n", (char*)raw_addr);
    uint8_t* raw_addr_bytes = base32_decode(raw_addr, 24, &err);
    if (err != OK)
    {
        printf("decode error:%d\n", err);
        return -1;
    }

    for (int i = 0; i < 24; i++)
    {
        printf("%d ", raw_addr_bytes[i]);
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
    //test_bls();
}



