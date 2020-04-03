#ifndef LOTUS_ADDR_TOOLS_RANDOM_H
#define LOTUS_ADDR_TOOLS_RANDOM_H

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <bits/stdint-uintn.h>

#define RANDOM_DEVICE "/dev/urandom"

typedef struct random_mapper__ {
    void (*random_init)(void);
    uint8_t (*random_bytes)(uint8_t* buf, uint32_t len, const uint8_t update_seed);
} random_mapper;


void random_init();

uint8_t random_bytes(uint8_t* buf, uint32_t len, const uint8_t update_seed);

#endif 
