#include "regDefs.h"
#include<iostream>
#include<string>

using namespace std;

#define iCacheSize 8
#define dCacheSize 8
#define blockSize 4

bool iValid[iCacheSize];
int iTag[iCacheSize];
int iData[iCacheSize][blockSize];

bool dValid[dCacheSize];
int dTag[dCacheSize];
int dData[dCacheSize][blockSize];

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
            iData[index][i] = Memory[(pc & ~(blockSize-1)) + i];
        }
        iValid[index] = 1;
        iTag[index] = tag;
        cout << "Cache Miss\n";
    }
    return iData[index][blockOffset];
}

int dataMemoryRead(int address){
    int blockOffset = address & (blockSize-1);
    int index = (((dCacheSize-1)*blockSize)&address)/blockSize;
    int tag = address / (blockSize*dCacheSize);
    if((dTag[index] == tag) && (dValid[index])){
        // cache hit
        cout << "Cache Hit\n";
    }
    else{
        // cache miss
        for(int i = 0; i <= (blockSize-1); i++){
            dData[index][i] = Memory[(address & ~(blockSize-1)) + i];
        }
        dValid[index] = 1;
        dTag[index] = tag;
        cout << "Cache Miss\n";
    }
    return dData[index][blockOffset];
}

/*
 // Write though
void dataMemoryWrite(int address, int data){
    int blockOffset = address & (blockSize-1);
    int index = (((dCacheSize-1)*blockSize)&address)/blockSize;
    int tag = address / (blockSize*dCacheSize);
    if((dTag[index] == tag) && (dValid[index])){
        // Write hit
        dData[index][blockOffset] = data;
        cout << "Write Hit\n";
    }
    else{
        // Write miss
        for(int i = 0; i <= (blockSize-1); i++){
            dData[index][i] = Memory[(address & ~(blockSize-1)) + i];
        }
        dValid[index] = 1;
        dTag[index] = tag;
        dData[index][blockOffset] = data;
        cout <<"Write Miss\n";
    }
    Memory[address] = data;
}
*/

// Write back
void dataMemoryWrite(int address, int data){
   int blockOffset = address & (blockSize-1);
   int index = (((dCacheSize-1)*blockSize)&address)/blockSize;
   int tag = address / (blockSize*dCacheSize);
   if((dTag[index] == tag) && (dValid[index])){
       // Write hit
       dData[index][blockOffset] = data;
       cout << "Write Hit\n";
   }
   else{
       // Write miss
       // Write soon-to-be-overwrited data to memory
       if(dValid[index]){
           int repAddress = dTag[index] * blockSize * dCacheSize;
           repAddress = repAddress + index*blockSize;
           for(int i = 0; i <= blockSize-1; i++){
               Memory[repAddress+i] = dData[index][i];
           }
           cout << "Memory Write\n";
       }
       for(int i = 0; i <= (blockSize-1); i++){
           dData[index][i] = Memory[(address & ~(blockSize-1)) + i];
       }
       dValid[index] = 1;
       dTag[index] = tag;
       dData[index][blockOffset] = data;
       cout <<"Write Miss\n";
   }
}

int main(){
    dataMemoryWrite(4,30);
    dataMemoryWrite(4,20);
    dataMemoryWrite(5,15);
    dataMemoryWrite(37,20);
    cout << Memory[5] << "\n";
}
