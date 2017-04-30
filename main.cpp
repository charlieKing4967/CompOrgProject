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
  //programMemory[0] = 0x21290001;
  //registers[10] = 12;
  //registers[9] = 10;
  //registers[10] = 15;
  //programMemory[0] = 0x21290001;
  //programMemory[1] = 0x21290001;
  programMemory[1] = 0x2009000F;
  //programMemory[2] = 0x112A000C;
  programMemory[2] = 0x01200008;
  programMemory[3] = 0x20090003;

  //registers[9] = 6;
  //registers[10] = 7;
  //registers[11] = 4;
  //registers[12] = 3;
  //memory[20] = 10;
  //registers[11]
  for(int clock = 0; clock < 25; clock++){
    cout << pc << "    ";
    instruction_fetch(&IFIDShadow);
    write_back(&MEMWB);
    instruction_decode(&IFID,&IDEXShadow,&EXMEM);
    cout << registers[9] << "\n";

    execute(&IDEX,&EXMEMShadow,&MEMWB);
    memory_access(&EXMEM,&MEMWBShadow);
    IDEX = IDEXShadow;
    EXMEM = EXMEMShadow;
    MEMWB = MEMWBShadow;
    if(!stall){
      if(!IFflush) IFID = IFIDShadow;
      pc++;
    }
    //cout << pc << "   " << EXMEM.Rd << IDEX.Rd << "   " << EXMEM.aluResult << "\n";
    //cout << IDEX.Rd;
  }
  return 0;
}
