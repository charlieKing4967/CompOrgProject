#include "util.h"
#include "pipeFuncs.h"
#include <iostream>
#include <string>

using namespace std;

IFID_Reg IFID, IFIDShadow;
IDEX_Reg IDEX, IDEXShadow;
EXMEM_Reg EXMEM, EXMEMShadow;
MEMWB_Reg MEMWB, MEMWBShadow;

int main(){
  pc = 0;
  programMemory[0] = 0x8D2A0000;
  programMemory[1] = 0x214A0001;
  programMemory[2] = 0x214A0001;
  memory[8] = 15;
  registers[9] = 8;
  //registers[10] = 7;
  //registers[11] = 4;
  //registers[12] = 3;
  memory[15] = 10;
  //registers[11]
  for(int clock = 0; clock < 25; clock++){
    write_back(&MEMWB);
    instruction_fetch(&IFIDShadow);
    instruction_decode(&IFID,&IDEXShadow);
    execute(&IDEX,&EXMEMShadow,&MEMWB);
    memory_access(&EXMEM,&MEMWBShadow);
    IDEX = IDEXShadow;
    EXMEM = EXMEMShadow;
    MEMWB = MEMWBShadow;
    cout << pc << "    " << registers[10] << "\n";
    if(!stall){
      IFID = IFIDShadow;
      pc++;
    }
    //cout << pc << "   " << EXMEM.Rd << IDEX.Rd << "   " << EXMEM.aluResult << "\n";
    //cout << IDEX.Rd;
  }
  return 0;
}
