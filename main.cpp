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
  programMemory[0] = 0x21290001;
  programMemory[1] = 0x21290001;
  programMemory[4] = 0x152A000A;
  programMemory[5] = 0x216B0007;
  //programMemory[15] = 0x200B0007;
  registers[9] = 4;
  registers[10] = 7;
  //registers[11] = 4;
  //registers[12] = 3;
  memory[15] = 10;
  //registers[11]
  for(int clock = 0; clock < 25; clock++){
    cout << pc << "    ";
    instruction_fetch(&IFIDShadow);
    write_back(&MEMWB);
    instruction_decode(&IFID,&IDEXShadow);
    execute(&IDEX,&EXMEMShadow,&MEMWB);
    memory_access(&EXMEM,&MEMWBShadow);
    cout << IFID.Opcode << "\n";

    IDEX = IDEXShadow;
    EXMEM = EXMEMShadow;
    MEMWB = MEMWBShadow;
    if(!stall){
      IFID = IFIDShadow;
      pc++;
    }
    //cout << pc << "   " << EXMEM.Rd << IDEX.Rd << "   " << EXMEM.aluResult << "\n";
    //cout << IDEX.Rd;
  }
  return 0;
}
