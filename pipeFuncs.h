#include "regDefs.h"


void instruction_fetch(unsigned int pc,IFID_Reg *IFID){
  int instruction = memory[pc];
  IFID->Opcode = instruction >> 26;
  if((IFID->Opcode == 2) || (IFID->Opcode == 3)){
    IFID->jumpaddress = instruction & 0x3FFFFFF;
  }
  else{
    IFID->Rs = (instruction >> 21) & 0x1F;
    IFID->Rt = (instruction >> 16) & 0x1F;
    if(IFID->Opcode == 0){
      IFID->Rd = (instruction >> 11) & 0x1F;
      IFID->shamtl = (instruction >> 6) & 0x1F;
      IFID->funct = instruction & 0x3F;
    }
    else{
      IFID->immediate = instruction & 0xFFFF;
    }
  }

  IFID->PCplus1 = pc+1;
}

void instruction_decode(IFID_Reg *IFID,IDEX_Reg *IDEX){

  // Need to sign extend
  if(IFID->immediate >> 15){
    IDEX->immediate = IFID->immediate + 0xFFFF0000;
  }
  else{
    IDEX->immediate = IFID->immediate;
  }

  IFID->PCplus1 = IDEX->PCplus1;
}

void execute(IDEX_Reg *IDEX,EXMEM_Reg *EXMEM){

  if(IDEX->Opcode == 0){
    switch (IDEX->funct){
      // add
      case 32: EXMEM->aluResult = IDEX->readRs + IDEX->readRt;
      // addu (not sure what's different)
      case 33: EXMEM->aluResult = IDEX->readRs + IDEX->readRt;
      // and
      case 36: EXMEM->aluResult = IDEX->readRs & IDEX->readRt;
      // jr (Not sure about this one)
      case 8:  EXMEM->PCplus1 = IDEX->readRs;
      // nor
      case 39: EXMEM->aluResult = ~(IDEX->readRs | IDEX->readRt);
      // or
      case 37: EXMEM->aluResult = IDEX->readRs | IDEX->readRt;
      // movn?
      // movz?
      // slt
      case 42: EXMEN->aluResult = (R[rs] < R[rt]) ? 1 : 0;
      // sltu (nned to sign-extend)
      //case 43: EXMEN->aluResult =
      // sll
      case 0: EXMEN->aluResult = IDEX->readRt << IDEX->shamtl;
      // srl
      case 2: EXMEN->aluResult = IDEX->readRt >> IDEX->shamtl;
      

    }
  }
  EXMEM->readRt = IDEX->readRt;
}

void memory_access(EXMEM_Reg *EXMEM,MEMWB_Reg *MEMWB){

}

void write_back(MEMWB_Reg *MEMWB){

}
