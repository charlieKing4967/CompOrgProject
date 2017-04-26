#include "regDefs.h"


void instruction_fetch(unsigned int pc,IFID_Reg *IFID){
  IFID->instruction = memory[pc];
  IFID->PCplus1 = pc+1;
}

void instruction_decode(IFID_Reg *IFID,IDEX_Reg *IDEX){
  // Bit mast elements out of instruction
  IDEX->Opcode = (IFID->instruction >> 26) & 0x3F;
  if((IDEX->Opcode == 2) || (IDEX->Opcode == 3)){
    IDEX->jumpaddress = IFID->instruction & 0x3FFFFFF;
    IDEX->RegDst = 0;
    IDEX->Branch = 0;
    IDEX->MemRead = 0;
    IDEX->MemWrite = 0;
    IDEX->RegWrite = 0;
    IDEX->MemtoReg = 0;
  }
  else{
    IDEX->Rs = (IFID->instruction >> 21) & 0x1F;
    IDEX->Rt = (IFID->instruction >> 16) & 0x1F;
    if(IDEX->Opcode == 0){
      IDEX->RegDst = 1;
      IDEX->Branch = 0;
      IDEX->MemRead = 0;
      IDEX->MemWrite = 0;
      IDEX->RegWrite = 1;
      IDEX->MemtoReg = 0;
      IDEX->Rd = (IFID->instruction >> 11) & 0x1F;
      IDEX->shamtl = (IFID->instruction >> 6) & 0x1F;
      IDEX->funct = IFID->instruction & 0x3F;
    }
    else{
      IDEX->immediate = IFID->instruction & 0xFFFF;
    }
  }

  // Read registers
  IDEX->readRs = registers[IDEX->Rs];
  IDEX->readRt = registers[IDEX->Rt];

  // Sign extend immediate value
  if(IDEX->immediate >> 15){
    IDEX->immediate = IDEX->immediate + 0xFFFF0000;
  }

  // Pass through PC
  IDEX->PCplus1 = IFID->PCplus1;


}

void execute(IDEX_Reg *IDEX,EXMEM_Reg *EXMEM){

  EXMEM->branchPC = IDEX->PCplus1+IDEX->immediate;

  // R-type instructions
  if(IDEX->Opcode == 0){
    switch (IDEX->funct){
      // add
      case 32: EXMEM->aluResult = IDEX->readRs + IDEX->readRt;
      break;
      // addu (not sure what's different)
      case 33: EXMEM->aluResult = IDEX->readRs + IDEX->readRt;
      break;
      // and
      case 36: EXMEM->aluResult = IDEX->readRs & IDEX->readRt;
      break;
      // jr (Not sure about this one)
      case 8:  EXMEM->PCplus1 = IDEX->readRs;
      break;
      // nor
      case 39: EXMEM->aluResult = ~(IDEX->readRs | IDEX->readRt);
      break;
      // or
      case 37: EXMEM->aluResult = IDEX->readRs | IDEX->readRt;
      break;
      // movn?
      // movz?
      // slt
      case 42: EXMEM->aluResult = (IDEX->readRs < IDEX->readRt) ? 1 : 0;
      break;
      // sltu (nned to sign-extend)
      //case 43: EXMEN->aluResult =
      // sll
      case 0: EXMEM->aluResult = IDEX->readRt << IDEX->shamtl;
      break;
      // srl
      case 2: EXMEM->aluResult = IDEX->readRt >> IDEX->shamtl;
      break;
      // sub
      case 34: EXMEM->aluResult = IDEX->readRs - IDEX->readRt;
      break;
      // subu (not sure what to change here)
      case 35: EXMEM->aluResult = IDEX->readRs - IDEX->readRt;
      break;
      // xor
      case 38: EXMEM->aluResult = IDEX->readRs != IDEX->readRt;
      break;

    }
  }
  // J and I-type instructions
  switch(IDEX->Opcode){
    // j (not sure if I should update PC directly)
    case 2: EXMEM->PCplus1 = IDEX->jumpaddress;
    break;
    // jal (not sure if I should update PC directly here too)
    case 3:
      registers[31] = IDEX->PCplus1 + 2;
      EXMEM->PCplus1 = IDEX->jumpaddress;
      break;
    // addi
    case 8: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;
    // addiu (not sure what to change)
    case 9: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;
    // andi
    case 12: EXMEM->aluResult = IDEX->readRs & IDEX->immediate;
    break;
    // xori
    case 14: EXMEM->aluResult = IDEX->readRs != IDEX->immediate;
    break;
    // beg
    case 4:
      if(IDEX->readRs == IDEX->readRt){
        EXMEM->PCplus1 = EXMEM->PCplus1 + 1 + IDEX->immediate;
      }
      break;
    // bne
    case 5:
    if(IDEX->readRs != IDEX->readRt){
      EXMEM->PCplus1 = EXMEM->PCplus1 + 1 + IDEX->immediate;
    }
    break;
    // bgtz (wtf is this shit)
    // bltz (again wtf)
    // blez (okay for real wtf)
    // lb (not sure what to do for bytes)
    case 32: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;
    // lbu
    // lhu
    // lui
    // lw
    case 35: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    EXMEM->MemRead = 1;
    EXMEM->MemtoReg = 1;
    break;
    // ori
    case 13: EXMEM->aluResult = IDEX->readRs | IDEX->immediate;
    break;
    // slti
    case 10: EXMEM->aluResult = (IDEX->readRs < IDEX->immediate) ? 1:0;
    break;
    // sltiu (not sure what to change here)
    case 11: EXMEM->aluResult = (IDEX->readRs < IDEX->immediate) ? 1:0;
    break;
    // sb
    // sh
    // sw
    case 43: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    EXMEM->MemWrite = 1;
    break;
    // seb
  }

  EXMEM->readRt = IDEX->readRt;
  if(IDEX->RegDst){
    EXMEM->writeReg = IDEX->Rd;
  }
  else{
    EXMEM->writeReg = IDEX->Rt;
  }

  EXMEM->Branch = IDEX->Branch;
  EXMEM->MemRead = IDEX->MemRead;
  EXMEM->MemWrite = IDEX->MemWrite;
  EXMEM->RegWrite = IDEX->RegWrite;
  EXMEM->MemtoReg = IDEX->MemtoReg;

}

void memory_access(EXMEM_Reg *EXMEM,MEMWB_Reg *MEMWB){
  // Read from memory
  if(EXMEM->MemRead){
    MEMWB->readData = memory[EXMEM->aluResult];
  }
  // Write to memory
  if(EXMEM->MemWrite){
    memory[EXMEM->aluResult] = EXMEM->readRt;
  }
  if(EXMEM->Branch){

  }
  MEMWB->writeReg = EXMEM->writeReg;
  MEMWB->MemtoReg = EXMEM->MemtoReg;
  MEMWB->aluResult = EXMEM->aluResult;
  MEMWB->RegWrite = EXMEM->RegWrite;
}

void write_back(MEMWB_Reg *MEMWB){
  if(MEMWB->MemtoReg){
    registers[MEMWB->writeReg] = MEMWB->readData;
  }
  else{
    registers[MEMWB->writeReg] = MEMWB->aluResult;
  }
}
