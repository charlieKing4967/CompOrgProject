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
  programMemory[0] = 0x8D2A0002;
  programMemory[1] = 0x214A0001;
  //memory[8] = 15;
  registers[9] = 5;
  memory[7] = 15;
  //registers[11]
  for(int clock = 1; clock < 25; clock++){
    cout << pc << "\n";

    instruction_fetch(&IFIDShadow);
    instruction_decode(&IFID,&IDEXShadow);
    execute(&IDEX,&EXMEMShadow,&MEMWB);
    memory_access(&EXMEM,&MEMWBShadow);
    write_back(&MEMWB);
    IFID = IFIDShadow;
    IDEX = IDEXShadow;
    EXMEM = EXMEMShadow;
    MEMWB = MEMWBShadow;
    //cout << pc << "   " << EXMEM.Rd << IDEX.Rd << "   " << EXMEM.aluResult << "\n";
    //cout << IDEX.Rd;
  }
  return 0;
}
