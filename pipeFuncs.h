#include "regDefs.h"

void instruction_fetch(int pc,IFID_Reg IFID){
  int instruction = memory[pc];
  IFID.Opcode = instruction >> 26;
  if((IFID.Opcode == 2) || (IFID.Opcode == 3)){
    IFID.jumpaddress = instruction & 0x3FFFFFF;
  }
  else{
    IFID.Rs = (instruction >> 21) & 0x1F;
    IFID.Rt = (instruction >> 16) & 0x1F;
    if(IFID.Opcode == 0){
      IFID.Rd = (instruction >> 11) & 0x1F;
      IFID.shamtl = (instruction >> 6) & 0x1F;
      IFID.funct = instruction & 0x3F;
    }
    else{
      IFID.immediate = instruction & 0xFFFF;
    }
  }
  IFID.PCplus4 = pc+4;
}

void instruction_decode(IFID_Reg IFID,IDEX_Reg IDEX){

}

void execute(IDEX_Reg IDEX,EXMEM_Reg EXMEM){

}

void memory_access(EXMEM_Reg EXMEM,MEMWB_Reg MEMWB){

}

void write_back(MEMWB_Reg MEMWB){

}
