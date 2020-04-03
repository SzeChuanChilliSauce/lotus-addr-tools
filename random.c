#include "random.h"


void random_init_internal(void);
uint8_t random_bytes_internal(uint8_t* buf, uint32_t len, const uint8_t update_seed);

static random_mapper default_mapper = { random_init_internal, random_bytes_internal };

void btc_random_init(void)
{
    default_mapper.random_init();
}

uint8_t random_bytes(uint8_t* buf, uint32_t len, const uint8_t update_seed)
{
    return default_mapper.random_bytes(buf,len, update_seed);
}

void random_init_internal(void) { }

uint8_t random_bytes_internal(uint8_t* buf, uint32_t len, const uint8_t update_seed)
{   
    (void)update_seed;
    FILE* frand = fopen(RANDOM_DEVICE, "r");
    if (!frand) {
        return 0;
    }

    size_t len_read = fread(buf, 1, len, frand);
    assert(len_read == len);
    fclose(frand);
    
    return 1;
}

