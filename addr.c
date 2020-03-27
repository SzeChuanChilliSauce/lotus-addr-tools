#include "addr.h"

const Address UndefAddr;


Address new_id_address(const uint8_t protocol, uint64_t id)
{ 
    // char* bytes = uint64_to_bytes(id);
    // int len = strlen(bytes);

    // char* data = (char*)malloc(len+2);
    // data[0] = protocol;
    // memcpy(data+1, bytes, len);
    // data[len+1] = '\0';
    // Address addr = { protocol, *data };
    
    // free(bytes);

    // return addr;
}

Address new_actor_address(const uint8_t protocol, const uint8_t* data)
{
    // TODO need to hash data
    // hash(pubkey);
}

Address new_secp256k1_address(const uint8_t protocol, const uint8_t* pubkey)
{
    // TODO need to hash pubkey
    // hash(pubkey);
}

Address new_bls_address(const uint8_t protocol, const uint8_t* pubkey)
{
    // int len = strlen(pubkey);
    // if (len != PUBLIC_KEY_BYTES) return UndefAddr;
    
    // char* data = (char*)malloc(len+2);
    // data[0] = protocol;
    // memcpy(data+1, pubkey, len);
    // data[len+1] = '\0';

    // Address addr = { protocol, *data };

    // return addr;
}

uint8_t* encode_address_to_str(uint8_t network, Address* addr)
{
    uint8_t protocol = int_to_hex_char(addr->protocol);
    uint8_t* ptr = addr->data;
    // checksum
    uint8_t out[CHECKSUM_HASH_LENGTH] = { 0 };
    // checksum + data
    size_t len = 4+sizeof(addr->data);
    uint8_t payload[CHECKSUM_HASH_LENGTH] = { 0 };

    if (checksum(addr->data, sizeof(addr->data), out, sizeof(out))) 
        return NULL;

    size_t quotient = 0; // 商
    size_t remainder = 0; // 余

    quotient = len / 5;
    remainder = len % 5;

    if (remainder == 0) 
    {
        uint8_t* res = (uint8_t*)malloc(quotient*8+1);

    }
    
    uint64_t fist_byte=0, second_byte = 0, third_byte=0, fourth_byte = 0, fifth_byte = 0;
   

    return 0;
}

int checksum(const uint8_t* in, size_t in_len, uint8_t* out, size_t out_len)
{
    // 检查参数
    if (in == NULL || in_len == 0 || (in[0] == '\0' && in_len!=0)) return -1;
    if (out == NULL || out_len == 0) return -1;

    // 计算校验和
    blake2b_state stat = {0};
    if (blake2b_init(&stat, out_len)) return -1;
    if (blake2b_update(&stat, in, in_len)) return -1;
    if (blake2b_final(&stat, out, out_len)) return -1;

    return 0;
}

// 9DC333C7FE149CCC
uint8_t* uint64_to_bytes(uint64_t x)
{
    if (x < 0) return NULL;
    uint8_t* res = (uint8_t*)malloc(2*8+1);
    uint64_t tmp = x;
    for (int i = 15; i >= 0; i--)
    {
        res[i] = int_to_hex_char(tmp & 0xf);
        tmp = tmp >> 4;
    }

    res[16] = '\0';
    return res;
}

uint8_t int_to_hex_char(const uint8_t c) 
{
    return c > 9 ? c + 0x57 : c + 0x30;
}


uint8_t* bytes_to_hex_str(const uint8_t* source, size_t len) 
{
    if (source == NULL) return NULL;

    uint8_t high = 0, low = 0;

    uint8_t* dest = (uint8_t*)malloc(2*len+1);
    
    for (int i = 0; i < len; i++) 
    {
        high = source[i] >> 4;
        low = source[i] & 0x0f;
        dest[2*i] = int_to_hex_char(high);
        dest[2*i+1] = int_to_hex_char(low);
    }

    dest[2*len] = '\0';

    return dest;
}


