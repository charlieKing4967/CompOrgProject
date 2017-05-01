#include "regDefs.h"

bool dValid[8];
int dTag[8];
int dData[8];

int programMemoryRead(){
    int index = pc & 0x7;
    int tag = pc >> 3;
    if((dTag[index] == tag) && (dValid[index]){
        // cache hit

    }
    else{
        // cache miss
        dData[index] = programMemory[pc];
        dValid[index] = 1;
    }
    return dData[index];
}
