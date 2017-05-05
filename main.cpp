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
  readProgram(Memory, "Program1File1.txt");

  // Set up sp, fp, & pc
  registers[29] = Memory[0];
  registers[30]= Memory[1];
  pc = Memory[5];

  // set up cpi values
  cycles = 0;
  instructions = 0;

  // set up hit values;
  ihit = 0; imiss = 0;
  dhit = 0; dmiss = 0;

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
      instructions++;
    }
    cycles++;
  }

  cout << "Cycles: " << cycles << "\tInstructions: " << instructions << "\tCPI: " << cycles/(float)instructions << "\n";
  cout << "ihit: " << 100*ihit/(float)(ihit+imiss) << "%\tdhit: " << 100*dhit/(float)(dhit+dmiss) << "%\n";

  cout << "Memory 6: " << dataMemoryRead(6) << "\n";
  cout << "Memory 7: " << dataMemoryRead(7) << "\n";
  cout << "Memory 8: " << dataMemoryRead(8) << "\n";
  cout << "Memory 9: " << dataMemoryRead(9) << "\n";

  /**
  for (int x = 0; x < 32; x++) {
      if (x % 4 == 0) cout << "\n";
      cout << x << ": " << registers[x] << "\t\t";
  }
  **/
  return 0;

}
