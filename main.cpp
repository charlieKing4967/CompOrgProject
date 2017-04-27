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
  memory[1] = 0x112A000C;
  //memory[8] = 15;
  registers[9] = 12;
  registers[10] = 12;
  for(int i = 1; i < 15; i++){
    instruction_fetch(&IFIDShadow);
    instruction_decode(&IFID,&IDEXShadow);
    execute(&IDEX,&EXMEMShadow);
    memory_access(&EXMEM,&MEMWBShadow);
    write_back(&MEMWB);
    IFID = IFIDShadow;
    IDEX = IDEXShadow;
    EXMEM = EXMEMShadow;
    MEMWB = MEMWBShadow;
    cout << pc << "\n";
  }
  return 0;
}
