#ifndef _buffer_
#define _buffer_

#include "stdint.h"
#include <string.h>
#include <stdio.h>

typedef struct buffer_t{
    uint32_t *buffer_Data;
    uint32_t head;
    uint32_t tail;
    uint32_t counter;
    uint32_t buffer_Size;
}buffer_t;

void buffer_Init(buffer_t *buffer, uint32_t *buffer_Data_t, uint32_t buffer_Size_t);

void buffer_Add(buffer_t *buffer, uint32_t data);

void buffer_Get(buffer_t *buffer, uint32_t *data);

void buffer_Clear(buffer_t *buffer);

void buffer_print(buffer_t* buffer);

#endif