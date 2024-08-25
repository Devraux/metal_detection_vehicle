#include "ring_Buffer.h"

void ring_Buffer_Init(ring_Buffer_t* ring_Buffer_t, uint16_t buffer_size)
{
    ring_Buffer_t->DataX = (int32_t*)calloc(buffer_size * sizeof(int32_t), buffer_size);
    ring_Buffer_t->DataY = (int32_t*)calloc(buffer_size * sizeof(int32_t), buffer_size);
    ring_Buffer_t->DataZ = (int32_t*)calloc(buffer_size * sizeof(int32_t), buffer_size);
    
    ring_Buffer_t->Buffer_Size = buffer_size;
    ring_Buffer_t->Head = 0;
    ring_Buffer_t->Tail = 0;
}

void ring_Buffer_Push(ring_Buffer_t* ring_Buffer, float dataX, float dataY, float dataZ)
{
    ring_Buffer->DataX[ring_Buffer->Head] = dataX;
    ring_Buffer->DataY[ring_Buffer->Head] = dataY;
    ring_Buffer->DataZ[ring_Buffer->Head] = dataZ;

    ring_Buffer->Head = (ring_Buffer->Head + 1) % ring_Buffer->Buffer_Size;
    ring_Buffer->Counter++;

    if(ring_Buffer->Counter > ring_Buffer->Buffer_Size)
        ring_Buffer->Counter = 0; // data overflow
}

void ring_Buffer_Pop(ring_Buffer_t* ring_Buffer, float* dataX, float* dataY, float* dataZ)
{
    *dataX = ring_Buffer->DataX[ring_Buffer->Tail];
    *dataY = ring_Buffer->DataY[ring_Buffer->Tail];
    *dataZ = ring_Buffer->DataZ[ring_Buffer->Tail];

    ring_Buffer->Tail = (ring_Buffer->Tail + 1) % ring_Buffer->Buffer_Size;
    ring_Buffer->Counter--;

    if(ring_Buffer->Counter <= 0)
        ring_Buffer->Counter = 0;
}

uint16_t ring_Buffer_Get_Capacity(ring_Buffer_t* ring_Buffer)
{
    return ring_Buffer->Counter;
}

void ring_Buffer_Clear(ring_Buffer_t* ring_Buffer)
{
    ring_Buffer->Counter = 0;
    ring_Buffer->Head = 0;
    ring_Buffer->Tail = 0;
}

void ring_Buffer_Print(ring_Buffer_t* ring_Buffer)
{
    for(uint16_t i = ring_Buffer->Tail; i < ring_Buffer->Head; i++)
    {
       printf("%d: %f\n", i, ring_Buffer->DataX[i]);
       printf("%d: %f\n", i, ring_Buffer->DataY[i]);
       printf("%d: %f\n", i, ring_Buffer->DataZ[i]);
       printf("---------------=end of data=----------------\n");
    }
    
}