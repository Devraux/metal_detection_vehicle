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

/// @brief  buffer initialization
/// @param buffer buffer structure
/// @param buffer_Data_t buffer pointer to data
/// @param buffer_Size_t buffer size
void buffer_Init(buffer_t *buffer, uint32_t *buffer_Data_t, uint32_t buffer_Size_t);

/// @brief buffer add data
/// @param buffer buffer structure
/// @param data data to push
void buffer_Add(buffer_t *buffer, uint32_t data);

/// @brief buffer get data function
/// @param buffer buffer structure
/// @param data data pointer where data from buffer was popped
void buffer_Get(buffer_t *buffer, uint32_t *data);

/// @brief buffer clear / reset (fill buffer data with int type 0 
/// @param buffer buffer structure
void buffer_Clear(buffer_t *buffer);

/// @brief buffer print content
/// @param buffer buffer structure
void buffer_print(buffer_t* buffer);

/// @brief return number of data stored in buffer  
/// @param buffer 
uint32_t buffer_Get_Count(buffer_t *buffer);

#endif