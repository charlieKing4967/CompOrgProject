#include "regDefs.h"
#include<iostream>
#include<string>

using namespace std;

bool dValid[8];
int dTag[8];
int dData[8];

int programMemoryRead(){
    int index = pc & 0x7;
    int tag = pc >> 3;
    if((dTag[index] == tag) && (dValid[index])){
        // cache hit
        cout << "Cache Hit\n";
    }
    else{
        // cache miss
        dData[index] = programMemory[pc];
        dValid[index] = 1;
        dTag[index] = tag;
        cout << "Cache Miss\n";
    }
    return dData[index];
}

int main(){
    programMemory[0] = 5;
    programMemory[1] = 10;
    programMemory[2] = 15;
    programMemory[9] = 20;
    int result;
    result = programMemoryRead();
    cout << result << "\n";
    pc = 1;
    result = programMemoryRead();
    cout << result << "\n";
    pc = 2;
    result = programMemoryRead();
    cout << result << "\n";
    pc = 9;
    result = programMemoryRead();
    cout << result << "\n";
    pc = 1;
    result = programMemoryRead();
    cout << result << "\n";


}
