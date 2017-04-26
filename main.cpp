#include "util.h"
#include "pipeFuncs.h"
#include <iostream>
#include <string>

using namespace std;

IFID_Reg IFID;

int main(){
  pc = 0;
  memory[0] = 0x012A4820;
  instruction_fetch(pc,&IFID);
  cout << IFID.Rd;


  return 0;
}
