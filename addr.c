#include "addr.h"

uint8_t* encode_id_address(uint8_t network, BlsAddress* addr, address_error_t* error)
{
    // TODO
    return NULL;
}


uint8_t* encode_secp256k1_actor_address(uint8_t network, Secp256k1Address* addr, address_error_t* error)
{
    blake2b_state stat = { 0 };
    uint8_t checksum[CHECKSUM_HASH_LENGTH] = { 0 };
    uint8_t payload[PAYLOAD_HASH_LENGTH] = { 0 };

    // 计算公钥哈希
    blake2b_init(&stat, PAYLOAD_HASH_LENGTH);
    blake2b_update(&stat, addr->data, sizeof(addr->data));
    blake2b_final(&stat, payload, PAYLOAD_HASH_LENGTH);

    // 计算checksum
    uint8_t proto_payload[1+PAYLOAD_HASH_LENGTH] = { 0 };
    proto_payload[0] = addr->protocol;
    memcpy(proto_payload+1, payload, PAYLOAD_HASH_LENGTH);
    blake2b_init(&stat, CHECKSUM_HASH_LENGTH);
    blake2b_update(&stat, proto_payload, 1+PAYLOAD_HASH_LENGTH);
    blake2b_final(&stat, checksum, CHECKSUM_HASH_LENGTH);

    //
    uint8_t payload_checksum[PAYLOAD_HASH_LENGTH+CHECKSUM_HASH_LENGTH] = { 0 };
    memcpy(payload_checksum, payload, PAYLOAD_HASH_LENGTH);
    memcpy(payload_checksum+PAYLOAD_HASH_LENGTH, checksum, CHECKSUM_HASH_LENGTH);

    // base32编码
    encode_error_t encode_err;
    uint8_t* encode_data = base32_encode(payload_checksum, PAYLOAD_HASH_LENGTH+4, &encode_err);
    if (encode_err != ENCODE_OK)
    {
        *error = encode_err;
        return strdup("");
    }

    uint8_t* addr_str = (uint8_t*)malloc(strlen(encode_data)+2+1);
    addr_str[0] = network;
    addr_str[1] = int_to_hex_char(addr->protocol);
    memcpy(addr_str+2, encode_data, strlen(encode_data));
    
    *error = ADDRESS_OK;
    free(encode_data);

    return addr_str;
}

uint8_t* encode_bls_address(uint8_t network, BlsAddress* addr, address_error_t* error)
{
    uint8_t checksum[4] = { 0 };

    // 合并protocol和pubkey
    uint8_t proto_pubkey[1+PUBLIC_KEY_BYTES] = { 0 };
    proto_pubkey[0] = addr->protocol;
    memcpy(proto_pubkey+1, addr->data, PUBLIC_KEY_BYTES);
 
    // 计算checksum
    int res = calc_checksum(proto_pubkey, 1+PUBLIC_KEY_BYTES, checksum, 4);
    if (res == -1)
    {
        *error = ERR_CHECKSUM;
        return strdup("");
    }

    // 合并pubkey和checksum
    uint8_t pubkey_checksum[PUBLIC_KEY_BYTES+4] = { 0 };
    memcpy(pubkey_checksum, addr->data, PUBLIC_KEY_BYTES);
    memcpy(pubkey_checksum+PUBLIC_KEY_BYTES, checksum, 4);

    // base32编码
    encode_error_t encode_err;
    uint8_t* encode_data = base32_encode(pubkey_checksum, PUBLIC_KEY_BYTES+4, &encode_err);
    if (encode_err != ENCODE_OK)
    {
        *error = encode_err;
        return strdup("");
    }

    uint8_t* addr_str = (uint8_t*)malloc(strlen(encode_data)+2+1);
    addr_str[0] = network;
    addr_str[1] = int_to_hex_char(addr->protocol);
    memcpy(addr_str+2, encode_data, strlen(encode_data));
    
    *error = ADDRESS_OK;
    free(encode_data);

    return addr_str;
}

int calc_checksum(const uint8_t* in, size_t in_len, uint8_t* out, size_t out_len)
{
    // 检查参数
    if (in == NULL || in_len == 0 || (in[0] == '\0' && in_len != 0)) 
        return -1;
    if (out == NULL || out_len == 0) 
        return -1;

    // 计算校验和
    blake2b_state stat = { 0 };
    if (blake2b_init(&stat, out_len)) 
        return -1;
    if (blake2b_update(&stat, in, in_len)) 
        return -1;
    if (blake2b_final(&stat, out, out_len)) 
        return -1;

    return 0;
}

// 9DC333C7FE149CCC
uint8_t* uint64_to_bytes(uint64_t x)
{
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


