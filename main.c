#include <stdio.h>
#include "filecoin.h"
#include "addr.h"
#include "blake2-impl.h"
#include "blake2.h"
void test(uint8_t c)
{
    switch (c)
    {
    default:
        printf("default...\n");
    case 4:
        printf("4...\n");
    case 3:
         printf("3...\n");
    case 2:
        printf("2...\n");
    case 1: 
        printf("1...\n");
    }
}

void test2(uint8_t* in)
{
    printf("====%ld\n", sizeof(in));
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

    // uint64_t i = 0x9DC333C7FE149CCCULL;
    // char* res = uint64_2_byte(i);
    // //char* str = byte_2_hex_str();
    // printf("%s\n", res);

    uint8_t arr[] = {3,138,126,22,34,2,177,53,213,232,
                    134,56,45,198,89,8,93,248,173,85,
                    43,84,50,1,114,88,4,139,46,37,
                    252,132,107,203,86,157,135,96,194,73, 
                    148,74,178,207,145,195,95,50,124};
    blake2b_state stat = {0};
    uint8_t  out[4] = {0};
    // printf("%ld %ld\n", sizeof(out), sizeof(arr));
    // blake2b_init(&stat, 4);
    // blake2b_update(&stat, arr, sizeof(arr));
    // blake2b_final(&stat, out, 4);


    // char* checksum = bytes_to_hex_str(out);
    // printf("%s %d\n", checksum, '\0');
    // free(checksum);
    int res = checksum(arr, sizeof(arr), out, sizeof(out));
    printf("checksum:%s, res=%d\n", bytes_to_hex_str(out, sizeof(out)), res);
    
}


