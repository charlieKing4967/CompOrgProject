#define iCacheSize 64
#define dCacheSize 1024
#define blockSize 1
#define writeBack 0

bool iValid[iCacheSize];
int iTag[iCacheSize];
int iData[iCacheSize][blockSize];

bool dValid[dCacheSize];
int dTag[dCacheSize];
int dData[dCacheSize][blockSize];

uint32_t programMemoryRead(){
    int blockOffset = pc & (blockSize-1);
    int index = (((iCacheSize-1)*blockSize)&pc)/blockSize;
    int tag = pc / (blockSize*iCacheSize);
    if((iTag[index] == tag) && (iValid[index])){
        // cache hit
        ihit++;
        //cout << "Cache Hit\n";
    }
    else{
        // cache miss
        cycles += 7 + 2*(blockSize-1);
        imiss++;
        for(int i = 0; i <= (blockSize-1); i++){
            iData[index][i] = Memory[(pc & ~(blockSize-1)) + i];
        }
        iValid[index] = 1;
        iTag[index] = tag;
        //cout << "Cache Miss\n";
    }
    return iData[index][blockOffset];
}

// Write-Through
uint32_t dataShadowRead_wt(uint32_t address){
    int blockOffset = address & (blockSize-1);
    int index = (((dCacheSize-1)*blockSize)&address)/blockSize;
    int tag = address / (blockSize*dCacheSize);
    if((dTag[index] == tag) && (dValid[index])){
        // cache hit
        dhit++;
        //cout << "Cache Hit\n";
    }
    else{
        // cache miss
        cycles += 7 + 2*(blockSize-1);
        dmiss++;
        for(int i = 0; i <= (blockSize-1); i++){
            dData[index][i] = Memory[(address & ~(blockSize-1)) + i];
        }
        //cout << "Memory Read\n";
        dValid[index] = 1;
        dTag[index] = tag;
        //cout << "Cache Miss\n";
    }
    return dData[index][blockOffset];
}

// Write Back
uint32_t dataShadowRead_wb(uint32_t address){
    int blockOffset = address & (blockSize-1);
    int index = (((dCacheSize-1)*blockSize)&address)/blockSize;
    int tag = address / (blockSize*dCacheSize);
    if((dTag[index] == tag) && (dValid[index])){
        // cache hit
        dhit++;
        //cout << "Cache Hit\n";
    }
    else{
        // cache miss
        cycles += 7 + 2*(blockSize-1);
        dmiss++;
        // Write soon-to-be-overwrited data to memory
        if(dValid[index]){
            int repAddress = dTag[index] * blockSize * dCacheSize;
            repAddress = repAddress + index*blockSize;
            for(int i = 0; i <= blockSize-1; i++){
                Memory[repAddress+i] = dData[index][i];
            }
            //cout << "Memory Write\n";
        }
        for(int i = 0; i <= (blockSize-1); i++){
            dData[index][i] = Memory[(address & ~(blockSize-1)) + i];
        }
        //cout << "Memory Read\n";
        dValid[index] = 1;
        dTag[index] = tag;
        //cout << "Cache Miss\n";
    }
    return dData[index][blockOffset];
}

// Write-Through
uint32_t dataMemoryRead_wt(uint32_t address){
    int blockOffset = address & (blockSize-1);
    int index = (((dCacheSize-1)*blockSize)&address)/blockSize;
    int tag = address / (blockSize*dCacheSize);
    if((dTag[index] == tag) && (dValid[index])){
        // cache hit
        dhit++;
        //cout << "Cache Hit\n";
    }
    else{
        // cache miss
        cycles += 7 + 2*(blockSize-1);
        dmiss++;
        for(int i = 0; i <= (blockSize-1); i++){
            dData[index][i] = Memory[(address & ~(blockSize-1)) + i];
        }
        //cout << "Memory Read\n";
        dValid[index] = 1;
        dTag[index] = tag;
        //cout << "Cache Miss\n";
    }
    return dData[index][blockOffset];
}

// Write Back
uint32_t dataMemoryRead_wb(uint32_t address){
    int blockOffset = address & (blockSize-1);
    int index = (((dCacheSize-1)*blockSize)&address)/blockSize;
    int tag = address / (blockSize*dCacheSize);
    if((dTag[index] == tag) && (dValid[index])){
        // cache hit
        dhit++;
        //cout << "Cache Hit\n";
    }
    else{
        // cache miss
        cycles += 7 + 2*(blockSize-1);
        dmiss++;
        // Write soon-to-be-overwrited data to memory
        if(dValid[index]){
            int repAddress = dTag[index] * blockSize * dCacheSize;
            repAddress = repAddress + index*blockSize;
            for(int i = 0; i <= blockSize-1; i++){
                Memory[repAddress+i] = dData[index][i];
            }
            //cout << "Memory Write\n";
        }
        for(int i = 0; i <= (blockSize-1); i++){
            dData[index][i] = Memory[(address & ~(blockSize-1)) + i];
        }
        //cout << "Memory Read\n";
        dValid[index] = 1;
        dTag[index] = tag;
        //cout << "Cache Miss\n";
    }
    return dData[index][blockOffset];
}

// Write though
void dataMemoryWrite_wt(uint32_t address, uint32_t data){
    int blockOffset = address & (blockSize-1);
    int index = (((dCacheSize-1)*blockSize)&address)/blockSize;
    int tag = address / (blockSize*dCacheSize);
    if((dTag[index] == tag) && (dValid[index])){
        // Write hit
        dData[index][blockOffset] = data;
        //cout << "Write Hit\n";
    }
    else{
        // Write miss
        for(int i = 0; i <= (blockSize-1); i++){
            dData[index][i] = Memory[(address & ~(blockSize-1)) + i];
        }
        dValid[index] = 1;
        dTag[index] = tag;
        dData[index][blockOffset] = data;
        //cout <<"Write Miss\n";
    }
    Memory[address] = data;
}

// Write back
void dataMemoryWrite_wb(uint32_t address, uint32_t data){
   int blockOffset = address & (blockSize-1);
   int index = (((dCacheSize-1)*blockSize)&address)/blockSize;
   int tag = address / (blockSize*dCacheSize);
   if((dTag[index] == tag) && (dValid[index])){
       // Write hit
       dData[index][blockOffset] = data;
       //cout << "Write Hit\n";
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
           //cout << "Memory Write\n";
       }
       for(int i = 0; i <= (blockSize-1); i++){
           dData[index][i] = Memory[(address & ~(blockSize-1)) + i];
       }
       dValid[index] = 1;
       dTag[index] = tag;
       dData[index][blockOffset] = data;
       //cout <<"Write Miss\n";
   }
}

// BOOLEAN VERSION OF FUNCTIONS TO CHECK IF WRITEBACK

uint32_t dataShadowRead(uint32_t address){
    return (writeBack) ? dataShadowRead_wb(address) : dataShadowRead_wt(address);
}

uint32_t dataMemoryRead(uint32_t address){
    return (writeBack) ? dataMemoryRead_wb(address) : dataMemoryRead_wt(address);
}

void dataMemoryWrite(uint32_t address, uint32_t data){
    return (writeBack) ? dataMemoryWrite_wb(address, data) : dataMemoryWrite_wt(address, data);
}
