#include "regDefs.h"


void instruction_fetch(IFID_Reg *IFID){
  uint32_t instruction = programMemory[pc];
  pc++;
  IFID->PCplus1 = pc;

  IFID->Opcode = (instruction >> 26) & 0x3F;
  if((IFID->Opcode == 2) || (IFID->Opcode == 3)){
    IFID->jumpaddress = instruction & 0x3FFFFFF;
  }
  else{
    IFID->Rs = (instruction >> 21) & 0x1F;
    IFID->Rt = (instruction >> 16) & 0x1F;
    if(IFID->Opcode == 0){
      IFID->immediate = 0;
      IFID->jumpaddress = 0;
      IFID->Rd = (instruction >> 11) & 0x1F;
      IFID->shamtl = (instruction >> 6) & 0x1F;
      IFID->funct = instruction & 0x3F;
    }
    else{
      IFID->Rd = 0;
      IFID->shamtl = 0;
      IFID->funct = 0;
      IFID->immediate = instruction & 0x0000FFFF;
    }
  }
}

void instruction_decode(IFID_Reg *IFID,IDEX_Reg *IDEX){

  if((IDEX->MemRead) && ((IDEX->Rt == IFID->Rs) ||  (IDEX->Rt == IFID->Rt))){
    // Stall the pipeline
    IDEX->RegDst = 0;
    IDEX->Branch = 0;
    IDEX->MemRead = 0;
    IDEX->MemWrite = 0;
    IDEX->RegWrite = 0;
    IDEX->MemtoReg = 0;
    IDEX->Opcode = 0;
    IDEX->Rs = 0;
    IDEX->Rt = 0;
    IDEX->Rd = 0;
    IDEX->shamtl = 0;
    IDEX->funct = 0;
    IDEX->jumpaddress = 0;
    IDEX->immediate = 0;
    stall = 1;
    return;
  }
  else stall = 0;

  // Bit mast elements out of instruction
  IDEX->Opcode = IFID->Opcode;
  if((IDEX->Opcode == 2) || (IDEX->Opcode == 3)){
    IDEX->jumpaddress = IFID->jumpaddress;
    IDEX->RegDst = 0;
    IDEX->Branch = 0;
    IDEX->MemRead = 0;
    IDEX->MemWrite = 0;
    IDEX->RegWrite = 0;
    IDEX->MemtoReg = 0;
  }
  else{
    IDEX->Rs = IFID->Rs;
    IDEX->Rt = IFID->Rt;
    // R-type instructions
    if(IDEX->Opcode == 0){
      IDEX->RegDst = 1;
      IDEX->Branch = 0;
      IDEX->MemRead = 0;
      IDEX->MemWrite = 0;
      IDEX->RegWrite = 1;
      IDEX->MemtoReg = 0;
      IDEX->immediate = 0;
      IDEX->jumpaddress = 0;
      IDEX->Rd = IFID->Rd;
      IDEX->shamtl = IFID->shamtl;
      IDEX->funct = IFID->funct;
    }
    else{
      IDEX->Rd = 0;
      IDEX->jumpaddress = 0;
      IDEX->immediate = IFID->immediate;
      // Branch instructions
      if((IDEX->Opcode >= 4) && (IDEX->Opcode <= 7)){
        IDEX->RegDst = 0;
        IDEX->Branch = 1;
        IDEX->MemRead = 0;
        IDEX->MemWrite = 0;
        IDEX->RegWrite = 0;
        IDEX->MemtoReg = 0;
      }
      // Load instructions
      if((IDEX->Opcode >= 32) && (IDEX->Opcode <= 38)){
        IDEX->RegDst = 0;
        IDEX->Branch = 0;
        IDEX->MemRead = 1;
        IDEX->MemWrite = 0;
        IDEX->RegWrite = 1;
        IDEX->MemtoReg = 1;
      }
      // Store instructions
      if((IDEX->Opcode >= 40) && (IDEX->Opcode <= 46)){
        IDEX->RegDst = 0;
        IDEX->Branch = 0;
        IDEX->MemRead = 0;
        IDEX->MemWrite = 1;
        IDEX->RegWrite = 0;
        IDEX->MemtoReg = 0;
      }
      // Immediate Arithmetic instructions
      if((IDEX->Opcode >= 8) && (IDEX->Opcode <= 15)){
        IDEX->RegDst = 0;
        IDEX->Branch = 0;
        IDEX->MemRead = 0;
        IDEX->MemWrite = 0;
        IDEX->RegWrite = 1;
        IDEX->MemtoReg = 0;
      }
    }
  }

  // Read registers
  IDEX->readRs = registers[IDEX->Rs];
  IDEX->readRt = registers[IDEX->Rt];

  // Sign extend immediate value
  if(IDEX->immediate >> 15){
    IFID->immediate |= 0xFFFF0000;
  }

  // Pass through PC
  IDEX->PCplus1 = IFID->PCplus1;


}

void execute(IDEX_Reg *IDEX,EXMEM_Reg *EXMEM,MEMWB_Reg *MEMWB){

  // Forwarding
  // EX Hazards
  // R-type to R-type
  if((EXMEM->RegWrite) && (EXMEM->Rd != 0) && (IDEX->Rd != 0) && (EXMEM->Rd == IDEX->Rs)){
    //ForwardA = 10
    IDEX->readRs = EXMEM->aluResult;
  }
  if((EXMEM->RegWrite) && (EXMEM->Rd != 0) && (IDEX->Rd != 0) && (EXMEM->Rd == IDEX->Rt)){
    //ForwardB = 10
    IDEX->readRt = EXMEM->aluResult;
  }
  // I-Type to I-type
  if((EXMEM->RegWrite) && (EXMEM->Rd == 0) && (IDEX->Rd == 0) && (EXMEM->Rt == IDEX->Rs)){
    //ForwardA = 10
    IDEX->readRs = EXMEM->aluResult;
  }
  // I-Type to R-Type
  if((EXMEM->RegWrite) && (EXMEM->Rd == 0) && (IDEX->Rd != 0) && (EXMEM->Rt == IDEX->Rs)){
    IDEX->readRs = EXMEM->aluResult;
  }
  if((EXMEM->RegWrite) && (EXMEM->Rd == 0) && (IDEX->Rd != 0) && (EXMEM->Rt == IDEX->Rt)){
    IDEX->readRt = EXMEM->aluResult;
  }
  // R-Type to I-Type
  if((EXMEM->RegWrite) && (EXMEM->Rd != 0) && (IDEX->Rd == 0) && (EXMEM->Rd == IDEX->Rs)){
    IDEX->readRs = EXMEM->aluResult;
  }


  // MEM Hazards
  // R-Type to R-Type
  if((MEMWB->RegWrite) && (MEMWB->Rd != 0) && (IDEX->Rd != 0) && ~((EXMEM->RegWrite) && (EXMEM->Rd != 0) && (EXMEM->Rd != IDEX->Rs)) && (MEMWB->Rd == IDEX->Rs)){
    if(MEMWB->MemtoReg){
      IDEX->readRs = MEMWB->readData;
      }
    else{
      IDEX->readRs = MEMWB->aluResult;
    }
  }
  if((MEMWB->RegWrite) && (MEMWB->Rd != 0) && (IDEX->Rd != 0) && ~((EXMEM->RegWrite) && (EXMEM->Rd != 0) && (EXMEM->Rd != IDEX->Rt)) && (MEMWB->Rd == IDEX->Rt)){
    if(MEMWB->MemtoReg){
      IDEX->readRt = MEMWB->readData;
    }
    else{
      IDEX->readRt = MEMWB->aluResult;
    }
  }

  // Calculating branch address
  EXMEM->branchPC = IDEX->PCplus1+IDEX->immediate;

  // R-type instructions
  if(IDEX->Opcode == 0){
    switch (IDEX->funct){
      // add
      case 32: EXMEM->aluResult = IDEX->readRs + IDEX->readRt;
      break;
      // addu
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
      // slt (convert from unsigned to signed 32 bit ints)
      case 42: EXMEM->aluResult = ((int32_t)IDEX->readRs < (int32_t)IDEX->readRt) ? 1 : 0;
      break;
      // sltu
      case 43: EXMEM->aluResult = (IDEX->readRs < IDEX->readRt) ? 1 : 0;
      break;
      // sll
      case 0: EXMEM->aluResult = IDEX->readRt << IDEX->shamtl;
      break;
      // srl
      case 2: EXMEM->aluResult = IDEX->readRt >> IDEX->shamtl;
      break;
      // sub
      case 34: EXMEM->aluResult = IDEX->readRs - IDEX->readRt;
      break;
      // subu
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
    // addiu
    case 9: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;
    // andi
    case 12: EXMEM->aluResult = IDEX->readRs & IDEX->immediate;
    break;
    // xori
    case 14: EXMEM->aluResult = IDEX->readRs != IDEX->immediate;
    break;
    // beq
    case 4:
      if(IDEX->readRs == IDEX->readRt){
        EXMEM->aluResult = 1;
      }
      else EXMEM->aluResult = 0;
      break;
    // bne
    case 5:
    if(IDEX->readRs != IDEX->readRt){
      EXMEM->aluResult = 1;
    }
    else{
      EXMEM->aluResult = 0;
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
  EXMEM->Rd = IDEX->Rd;
  EXMEM->Rs = IDEX->Rs;
  EXMEM->Rt = IDEX->Rt;

  // Pass through PC
  EXMEM->PCplus1 = IDEX->PCplus1;

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

  if(EXMEM->Branch && EXMEM->aluResult){
    pc = EXMEM->branchPC;
  }

  MEMWB->writeReg = EXMEM->writeReg;
  MEMWB->MemtoReg = EXMEM->MemtoReg;
  MEMWB->aluResult = EXMEM->aluResult;
  MEMWB->RegWrite = EXMEM->RegWrite;
}

void write_back(MEMWB_Reg *MEMWB){
  if(MEMWB->RegWrite){
    if(MEMWB->MemtoReg){
      registers[MEMWB->writeReg] = MEMWB->readData;
    }
    else{
      registers[MEMWB->writeReg] = MEMWB->aluResult;
    }
  }
}
