#include "buffer.h"

void buffer_Init(buffer_t *buffer, uint32_t *buffer_Data_t, uint32_t buffer_Size_t)
{
    buffer->counter = 0;
    buffer->head = 0;
    buffer->tail = 0;
    buffer->buffer_Size = buffer_Size_t;
    buffer->buffer_Data = buffer_Data_t;

    memset(buffer->buffer_Data, 0, buffer_Size_t * sizeof(uint32_t));
}

void buffer_Add(buffer_t *buffer, uint32_t data)
{
    buffer->buffer_Data[buffer->head] = data;
    buffer->head = (buffer->head + 1) % buffer->buffer_Size;
    buffer->counter++;
}

void buffer_Get(buffer_t *buffer, uint32_t *data)
{
    *data = buffer->buffer_Data[buffer->tail];
    buffer->tail = (buffer->tail + 1) % buffer->buffer_Size;
    buffer->counter--;
}

void buffer_Clear(buffer_t *buffer)
{
    buffer->head = 0;
    buffer->tail = 0;
    buffer->counter = 0;
    memset(buffer->buffer_Data, 0, buffer->buffer_Size * sizeof(uint32_t));
}

void buffer_print(buffer_t *buffer)
{
    for(uint16_t i = buffer->tail; i < buffer->head; i++)
    {
       printf("%d: %d\n", i, buffer->buffer_Data[i]);
       printf("---------------=----------------\n");
    }
}

uint32_t buffer_Get_Count(buffer_t *buffer)
{
    return buffer->counter;
}
