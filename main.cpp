#include <cstdint>
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

  // Read program
  readProgram(Memory, "Program2File1.txt");

  // Set up sp, fp, & pc
  registers[29] = Memory[0];
  registers[30]= Memory[1];
  pc = Memory[5];

  int cycles = 0;

  while (pc != 0){
    write_back(&MEMWB);
    instruction_fetch(&IFIDShadow);
    instruction_decode(&IFID, &IDEXShadow, &EXMEM);
    execute(&IDEX, &EXMEMShadow, &MEMWB);
    memory_access(&EXMEM, &MEMWBShadow);

    IDEX = IDEXShadow;
    EXMEM = EXMEMShadow;
    MEMWB = MEMWBShadow;
    if(!stall){
      if(!IFflush) IFID = IFIDShadow;
      pc++;
      cycles++;
    }
  }
  cout << "Cycles: " << cycles << "\n";
  cout << dataMemoryRead(6) << "\t" << dataMemoryRead(7) << "\t" << dataMemoryRead(8) << "\t" << dataMemoryRead(9) << "\n";

  /**
  for (int x = 0; x < 32; x++) {
      if (x % 4 == 0) cout << "\n";
      cout << x << ": " << registers[x] << "\t\t";
  }
  **/
  return 0;

}
