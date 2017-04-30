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
<<<<<<< Updated upstream

  registers[8] = 0xaa;
  registers[9] = 0xbb;
  registers[10] = 0xcc;
  registers[11] = 0xdd;

  programMemory[0] = 0xA0080000;
  programMemory[1] = 0xA0090001;
  programMemory[2] = 0xA00A0002;
  programMemory[3] = 0xA00B0003;

  programMemory[4] = 0x8C0C0000;
=======
  programMemory[1] = 0x2009000F;
  programMemory[2] = 0x01200008;
  programMemory[3] = 0x20090005;
>>>>>>> Stashed changes

  for(int clock = 0; clock < 25; clock++){
    write_back(&MEMWB);
<<<<<<< Updated upstream
    instruction_fetch(&IFIDShadow);
    instruction_decode(&IFID, &IDEXShadow, &EXMEM);
    execute(&IDEX, &EXMEMShadow, &MEMWB);
    memory_access(&EXMEM, &MEMWBShadow);
=======
    instruction_decode(&IFID,&IDEXShadow,&EXMEM);
    execute(&IDEX,&EXMEMShadow,&MEMWB);
    memory_access(&EXMEM,&MEMWBShadow);
    cout << registers[9] << "\n";

>>>>>>> Stashed changes
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

  cout << registers[12] << "\n";

  return 0;
}
