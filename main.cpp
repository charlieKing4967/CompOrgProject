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

  // read program
  readProgram(programMemory, "Program1File1.txt");

  // set up sp, fp, & pc
  registers[29] = programMemory[0];
  registers[30]= programMemory[1];
  pc = programMemory[5];

  for(int clock = 0; clock < 6000; clock++){
    cout << clock << ": " << registers[3] << ":" << registers[9] << " "<< pc+1 << ": " << programMemory[pc] << "\n";
    write_back(&MEMWB);
    instruction_fetch(&IFIDShadow);
    instruction_decode(&IFID, &IDEXShadow, &EXMEM);
    execute(&IDEX, &EXMEMShadow, &MEMWB);
    memory_access(&EXMEM, &MEMWBShadow);

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
