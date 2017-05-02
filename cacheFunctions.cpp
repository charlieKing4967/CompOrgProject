#include "regDefs.h"
#include<iostream>
#include<string>
#include<cmath>

using namespace std;

#define iCacheSize 8
#define dCacheSize 8
#define blockSize 4

bool iValid[iCacheSize];
int iTag[iCacheSize];
int iData[iCacheSize][blockSize];

int programMemoryRead(){
    int blockOffset = pc & (blockSize-1);
    int index = (((iCacheSize-1)*blockSize)&pc)/blockSize;
    int tag = pc / (blockSize*iCacheSize);
    if((iTag[index] == tag) && (iValid[index])){
        // cache hit
        cout << "Cache Hit\n";
    }
    else{
        // cache miss
        for(int i = 0; i <= (blockSize-1); i++){
            iData[index][i] = programMemory[(pc & ~(blockSize-1)) + i];
        }
        iValid[index] = 1;
        iTag[index] = tag;
        cout << "Cache Miss\n";
    }
    return iData[index][blockOffset];
}

int main(){
    programMemory[0] = 5;
    programMemory[1] = 10;
    programMemory[2] = 15;
    programMemory[3] = 20;
    programMemory[4] = 50;
    int result;
    pc = 5;
    result = programMemoryRead();
    cout << result << "\n";
    pc = 4;
    result = programMemoryRead();
    cout << result << "\n";
    pc = 1;
    result = programMemoryRead();
    cout << result << "\n";
    pc = 3;
    result = programMemoryRead();
    cout << result << "\n";
    pc = 4;
    result = programMemoryRead();
    cout << result << "\n";
    pc = 8;
    result = programMemoryRead();
    cout << result << "\n";

}
