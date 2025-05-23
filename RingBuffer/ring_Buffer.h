#ifndef _ring_Buffer_t_
#define _ring_Buffer_t_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/// THIS LIBRARY IS NOT USED AND CAN CONTAIN SOME BUGS AND UNTESTED CODE /// 

typedef struct ring_Buffer_t
{
    int32_t *DataX;       //ring buffer data
    int32_t *DataY;       //ring buffer data
    int32_t *DataZ;       //ring buffer data
    uint32_t Counter;     //counts data in buffer
    uint32_t Head;      
    uint32_t Tail;      
    uint32_t Buffer_Size;
}ring_Buffer_t;

/// @brief buffer initialize
/// @param ring_Buffer_t  buffer pointer to data
/// @param buffer_size buffer size
void ring_Buffer_Init(ring_Buffer_t* ring_Buffer_t, uint16_t buffer_size);


/// @brief ring buffer push data
/// @param ring_Buffer_t 
/// @param dataX 
/// @param dataY 
/// @param dataZ 
void ring_Buffer_Push(ring_Buffer_t* ring_Buffer_t, float dataX, float dataY, float dataZ);

/// @brief ring buffer pop data
/// @param ring_Buffer_t 
/// @param dataX 
/// @param dataY 
/// @param dataZ 
void ring_Buffer_Pop(ring_Buffer_t* ring_Buffer_t, float* dataX, float* dataY, float* dataZ);

/// @brief ring buffer get capacity
/// @param ring_Buffer_t 
/// @return ring_Buffer_t data pointer
uint16_t ring_Buffer_Get_Capacity(ring_Buffer_t* ring_Buffer_t);

/// @brief clear ring buffer
/// @param ring_Buffer_t 
void ring_Buffer_Clear(ring_Buffer_t* ring_Buffer_t);

/// @brief print data contained in ring buffer
/// @param ring_Buffer_t 
void ring_Buffer_Print(ring_Buffer_t* ring_Buffer_t);

#endif 