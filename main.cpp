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
  programMemory[1] = 0x016A5820;
  //programMemory[2] = 0x01896020;
  //memory[8] = 15;
  registers[9] = 15;
  //registers[10] = 7;
  memory[15] = 10;
  //registers[11]
  for(int clock = 0; clock < 25; clock++){
    instruction_fetch(&IFIDShadow);
    instruction_decode(&IFID,&IDEXShadow);
    execute(&IDEX,&EXMEMShadow,&MEMWB);
    memory_access(&EXMEM,&MEMWBShadow);
    write_back(&MEMWB);
    IDEX = IDEXShadow;
    EXMEM = EXMEMShadow;
    MEMWB = MEMWBShadow;
    if(!stall){
      IFID = IFIDShadow;
      pc++;
    }
    cout << pc << "   " << IFID.funct << "\n";
    //cout << pc << "   " << EXMEM.Rd << IDEX.Rd << "   " << EXMEM.aluResult << "\n";
    //cout << IDEX.Rd;
  }
  return 0;
}
