#include "util.h"
#include "pipeFuncs.h"
#include "readFunc.h"
#include <iostream>
#include <string>

using namespace std;

IFID_Reg IFID, IFIDShadow;
IDEX_Reg IDEX, IDEXShadow;
EXMEM_Reg EXMEM, EXMEMShadow;
MEMWB_Reg MEMWB, MEMWBShadow;

int main(){
  pc = 0;

  registers[9] = 10;


  programMemory[0] = 0x27BDFFE8;
  programMemory[1] = 0xAFBE0014;
  programMemory[2] = 0x03A0F025;
  programMemory[3] = 0x03E0F80A;
  programMemory[4] = 0x20020005;
  programMemory[5] = 0xAFC2000C;
  programMemory[6] = 0x8FC2000C;
  programMemory[7] = 0x0;
  programMemory[8] = 0x2442000A;
  programMemory[9] = 0xAFC20008;
  programMemory[10] = 0x00001025;
  programMemory[11] = 0x03C0E825;
  programMemory[12] = 0x8FBE0014;
  programMemory[13] = 0x27BD0018;
  programMemory[14] = 0x03E00008;
  programMemory[15] = 0x0;

  for(int clock = 0; clock < 25; clock++){
    cout << pc << "     ";
    write_back(&MEMWB);
    instruction_fetch(&IFIDShadow);
    instruction_decode(&IFID, &IDEXShadow, &EXMEM);
    execute(&IDEX, &EXMEMShadow, &MEMWB);
    memory_access(&EXMEM, &MEMWBShadow);
    cout << registers[2] << "\n";

    IDEX = IDEXShadow;
    EXMEM = EXMEMShadow;
    MEMWB = MEMWBShadow;
    if(!stall){
      IFID = IFIDShadow;
      pc++;
    }
  }
  return 0;
}
