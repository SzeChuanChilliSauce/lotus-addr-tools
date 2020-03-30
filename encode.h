#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENCODE_INPUT_LEN 64*1024*1024

// 错误类型
typedef enum encode_errno__  {
    OK = 0,
    ERR_INVALID_INPUT = 1,
    ERR_INPUT_TOO_BIG = 2,
    ERR_INVALID_DATA = 3,
    ERR_BAD_ALLOCATION = 4,
} encode_error_t;


// 
uint8_t* base32_encode(const uint8_t* input_data, size_t input_len, encode_error_t* err);



#endif 

