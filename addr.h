#ifndef LOTUS_ADDR_TOOLS_ADDR_H
#define LOTUS_ADDR_TOOLS_ADDR_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "filecoin.h"
#include "blake2-impl.h"
#include "blake2.h"
#include "encode.h"

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

// 地址
typedef struct {
    // 协议号
    // id         0 
    // secp256k1  1 
    // actor      2 
    // bls        3
    uint8_t protocol; 
    uint8_t data[48]; // 公钥
} BlsAddress;

typedef struct {
    uint8_t protocol;
    uint8_t data[65];
} Secp256k1Address;


// 地址错误类型
typedef enum address_errno__  {
    ADDRESS_OK = 0,
    ERR_CHECKSUM = 0xA1,
} address_error_t;


// 功能:计算校验和
// 参数:
//     in:输入数据
//     in_len:输入数据长度
//     out:checksum
//     out_len:checksum长度
// 返回值: 错误码
int calc_checksum(const uint8_t* in, size_t in_len, uint8_t* checkusm, size_t checksum_len);

// 功能:将地址进行编码
// 参数:
//     network:网络号, 测试网('t'), 主网('m')
//     addr:地址
//     error:错误信息
// 返回值:编码后的地址信息(字符串)
uint8_t* encode_bls_address(uint8_t network, BlsAddress* addr, address_error_t* error);

// 功能:将地址进行编码
// 参数:
//     network:网络号, 测试网('t'), 主网('m')
//     addr:地址
//     error:错误信息
// 返回值:编码后的地址信息(字符串)
uint8_t* encode_secp256k1_actor_address(uint8_t network, Secp256k1Address* addr, address_error_t* error);

// 功能:将地址进行编码
// 参数:
//     network:网络号, 测试网('t'), 主网('m')
//     addr:地址
//     error:错误信息
// 返回值:编码后的地址信息(字符串)
uint8_t* encode_id_address(uint8_t network, BlsAddress* addr, address_error_t* error);

// 功能:将1字节整数转为对应的字符
// 参数:
//     c:整数
// 返回值:整数字符
uint8_t int_to_hex_char(const uint8_t c);

// 功能:将字节数组转为16进制字符串(只包含数字和小写字母)
// 参数:
// 返回值:
uint8_t* bytes_to_hex_str(const uint8_t* source, size_t len);

// 功能:将64位无符号整数转为16进制字符串
// 参数：
//     num:整数
// 返回值:字符串
uint8_t* uint64_to_bytes(uint64_t num);

#endif 

