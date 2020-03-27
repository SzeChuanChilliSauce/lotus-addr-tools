#ifndef ADDR_H
#define ADDR_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "filecoin.h"
#include "blake2-impl.h"
#include "blake2.h"


// network
#define MAIN_NET 0
#define TEST_NET 1

// network prefix
#define MAIN_NET_PREFIX "f"
#define TEST_NET_PREFIX "t"

// protocol
#define ID        0
#define SECP256K1 1
#define ACTOR     2
#define BLS       3
#define UNKNOWN   255

//
#define CHECKSUM_HASH_LENGTH      4
#define PAYLOAD_HASH_LENGTH       20
#define BLS_PUBLIC_KEY_BYTES      48
#define BLS_PRIVATE_KEY_BYTES     32
#define MAX_ADDRESS_STRING_LENGTH 86

// Address
typedef struct {
    uint8_t protocol;
    uint8_t data[48];
} Address;


//
Address new_id_address(const uint8_t protocol, uint64_t id); 

//
Address new_secp256k1_address(const uint8_t protocol, const uint8_t* pubkey);

//
Address new_bls_address(const uint8_t protocol, const uint8_t* pubkey); 

//
Address new_actor_address(const uint8_t protocol, const uint8_t* data); 

// 
int checksum(const uint8_t* in, size_t in_len, uint8_t* checkusm, size_t checksum_len);

//
uint8_t* encode_address_to_str(uint8_t network, Address* addr);

// int_to_hex_char 将整数转为对应的字符
uint8_t int_to_hex_char(const uint8_t c);

// byte_to_hex_str 将字节数组转为16进制字符串(只包含数字和小写字母)
uint8_t* bytes_to_hex_str(const uint8_t* source, size_t len);

//
uint8_t* uint64_to_bytes(uint64_t x);

#endif 

