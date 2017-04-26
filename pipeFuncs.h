#include "regDefs.h"

void instruction_fetch(unsigned int pc,IFID_Reg IFID){
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

  IFID.PCplus1 = pc+1;
}

void instruction_decode(IFID_Reg IFID,IDEX_Reg IDEX){

  //reg_read(IFID.Rs, IDEX.readRs);
  //reg_read(IFID.Rt, IDEX.readRt);


  // Need to sign extend
  if(IFID.immediate >> 15){
    IDEX.immediate = IFID.immediate + 0xFFFF0000;
  }
  else{
    IDEX.immediate = IFID.immediate;
  }

  IFID.PCplus1 = IDEX.PCplus1;
}

void execute(IDEX_Reg IDEX,EXMEM_Reg EXMEM){
  if(IDEX.ALUSrc){
    alu(EXMEM.aluResult, IDEX.readRs, IDEX.immediate);
  }
  else{
    alu(EXMEM.aluResult, IDEX.readRs, IDEX.readRt);
  }

  EXMEM.readRt = IDEX.readRt;
}

void memory_access(EXMEM_Reg EXMEM,MEMWB_Reg MEMWB){

}

void write_back(MEMWB_Reg MEMWB){

}
