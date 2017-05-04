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

    registers[9] = 0;
    registers[10] = 15;

  int cycles = 0;
  //Memory[1] = 0x21290001;
  Memory[2] = 0x0149580A;
  //IMemory[2] = 0x01200008;

  while (pc <= 30){
      cout << pc << "       ";

    write_back(&MEMWB);
    instruction_fetch(&IFIDShadow);
    instruction_decode(&IFID, &IDEXShadow, &EXMEM);
    execute(&IDEX, &EXMEMShadow, &MEMWB);
    memory_access(&EXMEM, &MEMWBShadow);

    cout << registers[11] << "\n";
    IDEX = IDEXShadow;
    EXMEM = EXMEMShadow;
    MEMWB = MEMWBShadow;
    if(!stall){
      IFID = IFIDShadow;
      pc++;
    }
    cycles++;

  }
  /**
  for (int x = 0; x < 32; x++) {
      if (x % 4 == 0) cout << "\n";
      cout << x << ": " << registers[x] << "\t\t";
  }
  **/
  return 0;

}
