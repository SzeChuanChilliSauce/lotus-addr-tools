#include "encode.h"
#include <stdio.h>

static size_t strip_space(uint8_t* input);
static bool is_invalid_base32_input(uint8_t* input, size_t data_len);
static void check_input(const uint8_t* input_data, size_t input_len, encode_error_t* err);
static int get_base32_char_index(uint8_t c);

//
static const uint8_t bas32_alphabet[32] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                           'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                                           'u', 'v', 'w', 'x', 'y', 'z', '2', '3', '4', '5',
                                           '6', '7'};

uint8_t* base32_encode(const uint8_t* input_data, size_t input_len, encode_error_t* err)
{
    encode_error_t error;
    check_input(input_data, input_len, &error);
    if (error != ENCODE_OK)
    {
        *err = error;
        if (error == ERR_INPUT_EMPTY)
            return (uint8_t*)strdup("");
        else
            return NULL;
    }

    size_t extra = 0;
    switch (input_len % 5)
    {
    case 4:
        extra = (4*8)/5+1;
        break;
    case 3:
        extra = (3*8)/5+1;
        break;
    case 2:
        extra = (2*8)/5+1;
        break;
    case 1:
        extra = 8/5+1;
        break;
    default: // 0
        break;
    }

    // 编码后的长度(字节)
    size_t output_len = input_len/5*8 + extra;
    // 分配空间
    uint8_t* encoded_data = (uint8_t*)calloc(output_len + 1, 1);
    if (encoded_data == NULL)
    {
        *err = ERR_BAD_ALLOCATION;
        return NULL;
    }

    uint64_t first_byte = 0,second_byte = 0, third_byte = 0, fourth_byte = 0, fifth_byte = 0;
    int i, j = 0;
    for (i = 0; i + 5 < input_len; i += 5)
    {
        uint64_t num = 0;

        first_byte  = i < input_len ? input_data[i] : 0;
        second_byte = i < input_len ? input_data[i+1] : 0;
        third_byte  = i < input_len ? input_data[i+2] : 0;
        fourth_byte = i < input_len ? input_data[i+3] : 0;
        fifth_byte  = i < input_len ? input_data[i+4] : 0;
        
        // 将5个数合成一个数
        num = ((first_byte >> 3) << 35) +
              ((((first_byte  & 0x07) << 2) | (second_byte >> 6)) << 30 ) +
              (((second_byte  & 0x3f) >> 1) << 25) +
              ((((second_byte & 0x01) << 4) | (third_byte  >> 4)) << 20) +
              ((((third_byte  & 0x0f) << 1) | (fourth_byte >> 7)) << 15) +
              (((fourth_byte  & 0x7f) >> 2) << 10) +
              ((((fourth_byte & 0x3)  << 3) | (fifth_byte  >> 5)) << 5) +
              (fifth_byte     & 0x1f);


        // 取对应下标字符
        encoded_data[j++] = bas32_alphabet[(num >> 35) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 30) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 25) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 20) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 15) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 10) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 5)  & 0x1f];
        encoded_data[j++] = bas32_alphabet[num & 0x1f];
    }

    first_byte = 0, second_byte = 0, third_byte = 0, fourth_byte = 0, fifth_byte = 0;
    uint64_t num = 0;
    switch (input_len % 5)
    {
    case 4:
        first_byte  = input_data[input_len-4];
        second_byte = input_data[input_len-3];
        third_byte  = input_data[input_len-2];
        fourth_byte = input_data[input_len-1];
      
        num = ((first_byte >> 3) << 30) + 
                     ((((first_byte  & 0x07) << 2) | (second_byte >> 6)) << 25) +
                     (((second_byte  & 0x3f) >> 1) << 20) +
                     ((((second_byte & 0x01) << 4) | (third_byte >> 4)) << 15) +
                     ((((third_byte  & 0x0f) << 1) | (fourth_byte >> 7)) << 10) +
                     (((fourth_byte  & 0x7f) >> 2) << 5) +
                     ((fourth_byte   & 0x03) << 3);

        encoded_data[j++] = bas32_alphabet[(num >> 30) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 25) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 20) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 15) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 10) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 5)  & 0x1f];
        encoded_data[j++] = bas32_alphabet[num & 0x1f];
        break;

    case 3:
        first_byte  = input_data[input_len-3];
        second_byte = input_data[input_len-2];
        third_byte  = input_data[input_len-1];


        num = ((first_byte >> 3) << 20) +
              ((((first_byte  & 0x07) << 2) | (second_byte >> 6)) << 15) + 
              (((second_byte  & 0x3f) >> 1) << 10) +
              ((((second_byte & 0x01) << 4) | (third_byte >> 4)) << 5) +
              ((third_byte    & 0x0f) << 1);

        encoded_data[j++] = bas32_alphabet[(num >> 20) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 15) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 10) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 5)  & 0x1f];
        encoded_data[j++] = bas32_alphabet[num & 0x1f];
        break;

    case 2:
        first_byte  = input_data[input_len-2];
        second_byte = input_data[input_len-1];

        num = ((first_byte >> 3) << 15) +
              ((((first_byte & 0x07) << 2) | (second_byte >> 6)) << 10) + 
              (((second_byte & 0x3f) >> 1) << 5) +
              (second_byte   & 0x01);

        encoded_data[j++] = bas32_alphabet[(num >> 15) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 10) & 0x1f];
        encoded_data[j++] = bas32_alphabet[(num >> 5)  & 0x1f];
        encoded_data[j++] = bas32_alphabet[num & 0x1f];
        break;

    case 1:
        first_byte = input_data[input_len-1];
    
        num = ((first_byte >> 3) << 5) +  ((first_byte & 0x07) << 2);

        encoded_data[j++] = bas32_alphabet[(num >> 5) & 0x1f];
        encoded_data[j++] = bas32_alphabet[num & 0x1f];
        break;

    default:
        *err = ERR_INVALID_BASE32_DATA;
        return NULL;    
    }
    

    encoded_data[output_len] = '\0';
    *err = ENCODE_OK;

    return encoded_data;
}


uint8_t* base32_decode(const uint8_t* input_data, size_t input_len, encode_error_t* err)
{
    // TODO
    return NULL;
}

// 功能: 检查是否为合法的base32数据
// 参数:
//     input: 输入数据
//     data_len: 输入数据长度
// 返回值:
//     0: 不合法
//     1: 合法
static bool is_invalid_base32_input(uint8_t* input, size_t data_len)
{
    uint8_t *ptr = input;
    size_t len = 0;
    while (*ptr)
    {
        if (!(*ptr >= 'a' && *ptr <= 'z') && !(*ptr >= '2' && *ptr <= '7'))
            return false;
        len++;
        ptr++;
    }

    printf("decode len:%ld data_len=%ld\n", len, data_len);

    if (len != data_len)
        return false;
    else
        return true;
}

// 功能:检查输入
// 参数:
//     input_data: 需编码的数据
//     input_len: 数据长度
//     encode_error_t: 错误
// 返回值:无
static void check_input(const uint8_t* input_data, size_t input_len, encode_error_t* err)
{
    if (input_data == NULL || (input_len == 0 && input_data[0] != '\0'))
    {
        *err = ERR_INPUT_NULL;
        return ;
    }

    if (input_data[0] == '\0')
    {
        *err = ERR_INPUT_EMPTY;
        return ;
    }

    if (input_len > MAX_ENCODE_INPUT_LEN)
    {
        *err = ERR_INPUT_TOO_BIG;
        return ;
    }

    *err = ENCODE_OK;
}

// 功能: 统计空格的数量
// 参数:
//     input: 输入数据
// 返回值: 空格数量
static size_t strip_space(uint8_t* input)
{
    uint8_t* ptr = input;
    size_t count = 0;
    while (*ptr)
    {
        if (*ptr == ' ')
            count++;
        ptr++;
    }

    return count;
}

// 功能: 获取字符在base32字母表中的序号
// 参数: 
//     c: base字符
// 返回值: 字符序号
static int get_base32_char_index(uint8_t c)
{
    for (int i = 0; i < 32; i++)
    {
        if (bas32_alphabet[i] == c)
        {
            return i;
        }
    }

    return -1;
}
