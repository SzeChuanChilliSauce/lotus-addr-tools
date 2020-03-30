#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ENCODE_INPUT_LEN 64*1024*1024
#define MAX_DECODE_BASE32_INPUT_LEN ((MAX_ENCODE_INPUT_LEN*8+4)/5)


// 错误类型
typedef enum encode_errno__  {
    OK = 0,
    ERR_INPUT_NULL = 1,
    ERR_INPUT_EMPTY = 2,
    ERR_INPUT_TOO_BIG = 3,
    ERR_INVALID_BASE32_DATA = 4,
    ERR_BAD_ALLOCATION = 5,
} encode_error_t;


// 功能:对数据进行base32编码
// 参数:
//     input_data:需编码的数据
//     input_len:数据长度
//     error:错误
// 返回值:编码后的数据
uint8_t* base32_encode(const uint8_t* input_data, size_t input_len, encode_error_t* error);


// 功能:对数据解码
// 参数:
//     input_data:需解码数据
//     input_len:数据长度
//     error:错误
// 返回值:解码后的数据
uint8_t *base32_decode(const uint8_t* input_data, size_t input_len, encode_error_t* error);



#endif 

