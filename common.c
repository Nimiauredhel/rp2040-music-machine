#include <avr/io.h>
#include "common.h"

uint16_t VSP_Read(variableSizePointer *target)
{
    switch (target->pointerSize)
    {
        case 8:
            return *target->pointer.eight;
            break;
        case 16:
            return *target->pointer.sixteen;
            break;
        default:
            return *target->pointer.eight;
            break;
    }
}

void VSP_Write(uint16_t value, variableSizePointer *target)
{
    switch (target->pointerSize)
    {
        case 8:
            *target->pointer.eight = value;
            break;
        case 16:
            *target->pointer.sixteen = value;
            break;
        default:
            break;
    }
}
