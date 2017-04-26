#include "util.h"
#include "pipeFuncs.h"
#include <iostream>
#include <string>

using namespace std;

IFID_Reg IFID;

int main(){
  pc = 0;
  memory[0] = 0x112A0005;
  registers[9] = 2;
  registers[10] = 2;
  instruction_fetch(pc,&IFID);
  instruction_decode(&IFID,&IDEX);
  execute(&IDEX,&EXMEM);
  cout << IDEX.immediate;


  return 0;
}
