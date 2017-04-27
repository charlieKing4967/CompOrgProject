#include "util.h"
#include "pipeFuncs.h"
#include <iostream>
#include <string>

using namespace std;

IFID_Reg IFID;

int main(){
  pc = 0;
  memory[0] = 0x152A0002;
  //memory[8] = 15;
  registers[9] = 15;
  registers[10] = 15;
  instruction_fetch(pc,&IFID);
  instruction_decode(&IFID,&IDEX);
  execute(&IDEX,&EXMEM);
  memory_access(&EXMEM,&MEMWB);
  write_back(&MEMWB);
  cout << pc;

  return 0;
}
