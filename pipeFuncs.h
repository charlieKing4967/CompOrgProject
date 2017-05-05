#include "regDefs.h"
#include "cacheFuncs.h"

IDEX_Reg zeroReg;

void instruction_fetch(IFID_Reg *IFID){
  uint32_t instruction = programMemoryRead();
  //pc++;
  IFID->PCplus1 = pc;

  IFID->Opcode = (instruction >> 26) & 0x3F;
  if((IFID->Opcode == 2) || (IFID->Opcode == 3)){
    IFID->jumpaddress = instruction & 0x3FFFFFF;
  }
  else{
    IFID->Rs = (instruction >> 21) & 0x1F;
    IFID->Rt = (instruction >> 16) & 0x1F;
    // R-type or seb-type
    if(IFID->Opcode == 0 || IFID->Opcode == 31){
      IFID->immediate = 0;
      IFID->jumpaddress = 0;
      IFID->Rd = (instruction >> 11) & 0x1F;
      IFID->shamtl = (instruction >> 6) & 0x1F;
      IFID->funct = instruction & 0x3F;
    }
    // I-type
    else{
      IFID->Rd = 0;
      IFID->shamtl = 0;
      IFID->funct = 0;
      IFID->immediate = instruction & 0x0000FFFF;
      // Sign extend immediate value
      if(IFID->immediate >> 15){
        IFID->immediate |= 0xFFFF0000;
      }
    }
  }
}

void instruction_decode(IFID_Reg *IFID,IDEX_Reg *IDEX,EXMEM_Reg *EXMEM){
  // Jump detection
  IFflush = 0;
  if((IFID->Opcode == 2) || (IFID->Opcode == 3)){
    if(IFID->Opcode == 3) registers[31] = IFID->PCplus1+2;
    pc = IFID->jumpaddress-1;
    IFflush = 1;
  }

  // Stall if dependency after load instruction
  if((IDEX->MemRead) && ((IDEX->Rt == IFID->Rs) || (IDEX->Rt == IFID->Rt))){
    // Stall the pipeline
    stallCount++;
    *IDEX = zeroReg;
    stall = 1;
    return;
  }
  else stall = 0;

  // Jump detection
  if(((IFID->Opcode == 0) && (IFID->funct == 8)) || (IFID->Opcode == 6) || (IFID->Opcode == 7) || (IDEX->Opcode == 1 && IDEX->readRt == 0)){
    // Stall if adjacent dependency
    // Check for R-Type dependency
    if((IDEX->RegWrite) && (IDEX->Rd != 0) && (IDEX->Rd == IFID->Rs)){
      // Stall
      stallCount++;
      *IDEX = zeroReg;
      stall = 1;
      return;
    }
    // Check for I-Type dependency
    if((IDEX->RegWrite) && (IDEX->Rd == 0) && (IDEX->Rt == IFID->Rs)){
      // Stall
      stallCount++;
      *IDEX = zeroReg;
      stall = 1;
      return;
    }
    // Stall again if adjacent load instruction
    if((EXMEM->MemRead) && (EXMEM->Rt == IFID->Rs)){
      // Stall the pipeline
      stallCount++;
      *IDEX = zeroReg;
      stall = 1;
      return;
    }
  }
  // Branch detection
  // Beq and Bne
  if((IFID->Opcode == 4) || (IFID->Opcode == 5)){
    // Stall if adjacent dependency
    // Check for R-Type dependency
    if((IDEX->RegWrite) && (IDEX->Rd != 0) && ((IDEX->Rd == IFID->Rs) || (IDEX->Rd == IFID->Rt))){
      // Stall
      stallCount++;
      *IDEX = zeroReg;
      stall = 1;
      return;
    }
    // Check for I-Type dependency
    if((IDEX->RegWrite) && (IDEX->Rd == 0) && ((IDEX->Rt == IFID->Rs) || (IDEX->Rt == IFID->Rt))){
      // Stall
      stallCount++;
      *IDEX = zeroReg;
      stall = 1;
      return;
    }
    // Stall again if adjacent load instruction
    if((EXMEM->MemRead) && ((EXMEM->Rt == IFID->Rs) ||  (EXMEM->Rt == IFID->Rt))){
      // Stall the pipeline
      stallCount++;
      *IDEX = zeroReg;
      stall = 1;
      return;
    }
  }

  // Bit mask elements out of instruction
  IDEX->Opcode = IFID->Opcode;
  if((IDEX->Opcode == 2) || (IDEX->Opcode == 3)){
    IDEX->jumpaddress = IFID->jumpaddress;
    IDEX->RegDst = 0;
    IDEX->Branch = 0;
    IDEX->MemRead = 0;
    IDEX->MemWrite = 0;
    IDEX->RegWrite = 0;
    IDEX->MemtoReg = 0;
    IDEX->ByteData = 0;
    IDEX->HalfData = 0;
    IDEX->SignedData = 0;
  }
  else{
    IDEX->Rs = IFID->Rs;
    IDEX->Rt = IFID->Rt;
    // R-type or seb-type instructions
    if(IDEX->Opcode == 0 || IDEX->Opcode == 31){
      IDEX->RegDst = 1;
      IDEX->Branch = 0;
      IDEX->MemRead = 0;
      IDEX->MemWrite = 0;
      IDEX->RegWrite = 1;
      IDEX->MemtoReg = 0;
      IDEX->ByteData = 0;
      IDEX->HalfData = 0;
      IDEX->SignedData = 0;
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
      // Branch instructions (including the special bltz)
      if(((IDEX->Opcode >= 4) && (IDEX->Opcode <= 7)) || (IDEX->Opcode == 1 && IDEX->readRt == 0)){
        IDEX->RegDst = 0;
        IDEX->Branch = 1;
        IDEX->MemRead = 0;
        IDEX->MemWrite = 0;
        IDEX->RegWrite = 0;
        IDEX->MemtoReg = 0;
        IDEX->ByteData = 0;
        IDEX->HalfData = 0;
        IDEX->SignedData = 0;
      }
      // Load instructions
      if((IDEX->Opcode >= 32) && (IDEX->Opcode <= 38)){
        IDEX->RegDst = 0;
        IDEX->Branch = 0;
        IDEX->MemRead = 1;
        IDEX->MemWrite = 0;
        IDEX->RegWrite = 1;
        IDEX->MemtoReg = 1;
        IDEX->ByteData = (IDEX->Opcode == 32 || IDEX->Opcode == 36) ? 1 : 0;
        IDEX->HalfData = (IDEX->Opcode == 33 || IDEX->Opcode == 37) ? 1 : 0;
        IDEX->SignedData = (IDEX->Opcode == 32 || IDEX->Opcode == 33) ? 1 : 0;
      }
      // Store instructions
      if((IDEX->Opcode >= 40) && (IDEX->Opcode <= 43)){
        IDEX->RegDst = 0;
        IDEX->Branch = 0;
        IDEX->MemRead = 0;
        IDEX->MemWrite = 1;
        IDEX->RegWrite = 0;
        IDEX->MemtoReg = 0;
        IDEX->ByteData = (IDEX->Opcode == 40) ? 1 : 0;
        IDEX->HalfData = (IDEX->Opcode == 41) ? 1 : 0;
        IDEX->SignedData = 0;
      }
      // Immediate Arithmetic instructions
      if((IDEX->Opcode >= 8) && (IDEX->Opcode <= 15)){
        IDEX->RegDst = 0;
        IDEX->Branch = 0;
        IDEX->MemRead = 0;
        IDEX->MemWrite = 0;
        IDEX->RegWrite = 1;
        IDEX->MemtoReg = 0;
        IDEX->ByteData = 0;
        IDEX->HalfData = 0;
        IDEX->SignedData = 0;
      }
    }
  }

  // Read registers
  IDEX->readRs = registers[IDEX->Rs];
  IDEX->readRt = registers[IDEX->Rt];

  // Branch Forwarding
  if(((IFID->Opcode >= 4) && (IFID->Opcode <= 7)) || ((IFID->Opcode == 0) && (IFID->funct == 8))){
    // R-Type
    if((EXMEM->RegWrite) && (EXMEM->Rd != 0) && (EXMEM->Rd == IFID->Rs)){
      IDEX->readRs = EXMEM->aluResult;
    }
    if((EXMEM->RegWrite) && (EXMEM->Rd != 0) && (EXMEM->Rd == IFID->Rt)){
      IDEX->readRt = EXMEM->aluResult;
    }
    // I-Type
    if((EXMEM->RegWrite) && (EXMEM->Rd == 0) && (EXMEM->Rt == IFID->Rs)){
      IDEX->readRs = EXMEM->aluResult;
    }
    if((EXMEM->RegWrite) && (EXMEM->Rd == 0) && (EXMEM->Rt == IFID->Rt)){
      IDEX->readRt = EXMEM->aluResult;
    }
  }

  // Calculating branch address
  IDEX->branchPC = IFID->PCplus1 + IFID->immediate;

  switch (IFID->Opcode){
    // beq
    case 4:
      if(IDEX->readRs == IDEX->readRt) pc = IDEX->branchPC;
    break;
    // bne
    case 5:
      if(IDEX->readRs != IDEX->readRt) pc = IDEX->branchPC;
    break;
    // blez
    case 6:
      if((int32_t)IDEX->readRs <= 0) pc = IDEX->branchPC;
    break;
    // bgtz
    case 7:
      if((int32_t)IDEX->readRs > 0) pc = IDEX->branchPC;
    break;
    // bltz
    case 1:
      if((IDEX->readRt == 0) && ((int32_t)IDEX->readRs < 0)) pc = IDEX->branchPC;
    break;

  }

  // Calculating jump address
  if((IFID->Opcode == 0) && (IFID->funct == 8)){
    pc = IDEX->readRs - 1;
    IDEX->RegWrite = 0;
    IFflush = 1;
  }

  // Flush instruction if it writes to zero
  // if (IDEX->Opcode == 0 && IDEX->RegWrite && IDEX->Rd == 0) IFflush = 1;
  // else if (IDEX->RegWrite && IDEX->Rt == 0) IFflush = 1;

  // If branching, flush IFID register
  if(IFflush){
      IFID->PCplus1 = 0;
      IFID->Opcode = 0;
      IFID->Rs = 0;
      IFID->Rt = 0;
      IFID->Rd = 0;
      IFID->shamtl = 0;
      IFID->funct = 0;
      IFID->jumpaddress = 0;
      IFID->immediate = 0;
  }

  // Pass through PC
  IDEX->PCplus1 = IFID->PCplus1;


}

void execute(IDEX_Reg *IDEX,EXMEM_Reg *EXMEM,MEMWB_Reg *MEMWB){

  // Forwarding
  // MEM Hazards
  // R-Type
  if((MEMWB->RegWrite) && (MEMWB->Rd != 0) && (MEMWB->Rd == IDEX->Rs)){
    if(MEMWB->MemtoReg){
      IDEX->readRs = MEMWB->readData;
    }
    else{
      IDEX->readRs = MEMWB->aluResult;
    }
  }
  if((MEMWB->RegWrite) && (MEMWB->Rd != 0) && (MEMWB->Rd == IDEX->Rt)){
    if(!((EXMEM->RegWrite) && (EXMEM->Rd != 0) && (EXMEM->Rd == IDEX->Rs))){
      if(MEMWB->MemtoReg){
        IDEX->readRt = MEMWB->readData;
      }
      else{
        IDEX->readRt = MEMWB->aluResult;
      }
    }
  }
  // I-Type
  if((MEMWB->RegWrite) && (MEMWB->Rd == 0) && (MEMWB->Rt == IDEX->Rs)){
    if(MEMWB->MemtoReg){
      IDEX->readRs = MEMWB->readData;
    }
    else{
      IDEX->readRs = MEMWB->aluResult;
    }
  }
  if((MEMWB->RegWrite) && (MEMWB->Rd == 0) && (MEMWB->Rt == IDEX->Rt)){
    if(MEMWB->MemtoReg){
      IDEX->readRt = MEMWB->readData;
    }
    else{
      IDEX->readRt = MEMWB->aluResult;
    }
  }
  // EX Hazards
  // R-type
  if((EXMEM->RegWrite) && (EXMEM->Rd != 0) && (EXMEM->Rd == IDEX->Rs)){
    //ForwardA = 10
    IDEX->readRs = EXMEM->aluResult;
  }
  if((EXMEM->RegWrite) && (EXMEM->Rd != 0) && (EXMEM->Rd == IDEX->Rt)){
    //ForwardB = 10
    IDEX->readRt = EXMEM->aluResult;
  }
  // I-Type
  if((EXMEM->RegWrite) && (EXMEM->Rd == 0) && (EXMEM->Rt == IDEX->Rs)){
    IDEX->readRs = EXMEM->aluResult;
  }
  if((EXMEM->RegWrite) && (EXMEM->Rd == 0) && (EXMEM->Rt == IDEX->Rt)){
    IDEX->readRt = EXMEM->aluResult;
  }

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
      // nor
      case 39: EXMEM->aluResult = ~(IDEX->readRs | IDEX->readRt);
      break;
      // or
      case 37: EXMEM->aluResult = IDEX->readRs | IDEX->readRt;
      break;
      // movn
      case 11:
        IDEX->RegWrite = (IDEX->readRt != 0) ? 1 : 0;
        EXMEM->aluResult = IDEX->readRs;
      break;
      // movz
      case 10:
        IDEX->RegWrite = (IDEX->readRt == 0) ? 1 : 0;
        EXMEM->aluResult = IDEX->readRs;
      break;
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
      case 38: EXMEM->aluResult = IDEX->readRs ^ IDEX->readRt;
      break;

    }
  }
  // J and I-type instructions
  switch(IDEX->Opcode){
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
    case 14: EXMEM->aluResult = IDEX->readRs ^ IDEX->immediate;
    break;
    // lb
    case 32: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;
    // lh (added instruction for consistency)
    case 33: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;
    // lbu
    case 36: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;
    // lhu
    case 37: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;
    // lui
    case 15: EXMEM->aluResult = IDEX->immediate << 16;
    break;
    // lw
    case 35: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;
    // ori
    case 13: EXMEM->aluResult = IDEX->readRs | IDEX->immediate;
    break;
    // slti (convert from unsigned to signed 32 bit ints)
    case 10: EXMEM->aluResult = ((int32_t)IDEX->readRs < (int32_t)IDEX->immediate) ? 1:0;
    break;
    // sltiu
    case 11: EXMEM->aluResult = (IDEX->readRs < IDEX->immediate) ? 1:0;
    break;
    // sb
    case 40: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;
    // sh
    case 41: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;
    // sw
    case 43: EXMEM->aluResult = IDEX->readRs + IDEX->immediate;
    break;

    // Sub-opcode Section (for non standard instruction)
    case 31: // SPECIAL3 (R-type like)
      // seb
      if(IDEX->shamtl == 16) EXMEM->aluResult = (IDEX->readRt >> 7) ? IDEX->readRt | 0xffffff00 : IDEX->readRt;
    break;

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
  EXMEM->ByteData = IDEX->ByteData;
  EXMEM->HalfData = IDEX->HalfData;
  EXMEM->SignedData = IDEX->SignedData;
  EXMEM->Rd = IDEX->Rd;
  EXMEM->Rs = IDEX->Rs;
  EXMEM->Rt = IDEX->Rt;

  // Pass through PC
  EXMEM->PCplus1 = IDEX->PCplus1;

}

void memory_access(EXMEM_Reg *EXMEM,MEMWB_Reg *MEMWB){
  // Read from memory
  if(EXMEM->MemRead){
    // Read bytes
    uint32_t data = dataMemoryRead(EXMEM->aluResult>>2);
    //uint32_t data = Memory[EXMEM->aluResult>>2];
    if(EXMEM->ByteData){
        MEMWB->readData = (data >> ((3 - (EXMEM->aluResult & 3)) << 3)) & 0xFF;
        if (EXMEM->SignedData && MEMWB->readData >> 7) MEMWB->readData |= 0xFFFFFF00;
    }
    // Read Halfwords
    else if(EXMEM->HalfData){
        MEMWB->readData = (data >> ((1 - (EXMEM->aluResult>>1 & 1)) << 4)) & 0xFFFF;
        if (EXMEM->SignedData && MEMWB->readData >> 15) MEMWB->readData |= 0xFFFF0000;
    }
    // Read Words
    else MEMWB->readData = data;
  }
  // Write to memory
  if(EXMEM->MemWrite){
    uint32_t data = dataShadowRead(EXMEM->aluResult>>2);
    //uint32_t data = Memory[EXMEM->aluResult>>2];
    // Write Bytes
    if(EXMEM->ByteData){
        data &= ~(0xFF << ((3 - (EXMEM->aluResult & 3)) << 3));
        data |= (EXMEM->readRt & 0xFF) << ((3 - (EXMEM->aluResult & 3)) << 3);
    }
    // Write Halfwords
    else if(EXMEM->HalfData){
        data &= ~(0xFFFF << ((1 - (EXMEM->aluResult>>1 & 1)) << 4));
        data |= (EXMEM->readRt & 0xFFFF) << ((1 - (EXMEM->aluResult>>1 & 1)) << 4);
    }
    // Write Words
    else data = EXMEM->readRt;
    dataMemoryWrite(EXMEM->aluResult>>2, data);
    //Memory[EXMEM->aluResult>>2] = data;
  }

  MEMWB->writeReg = EXMEM->writeReg;
  MEMWB->MemtoReg = EXMEM->MemtoReg;
  MEMWB->aluResult = EXMEM->aluResult;
  MEMWB->RegWrite = EXMEM->RegWrite;
  MEMWB->Rd = EXMEM->Rd;
  MEMWB->Rt = EXMEM->Rt;
}

void write_back(MEMWB_Reg *MEMWB){
  if(MEMWB->RegWrite && MEMWB->writeReg != 0){
    if(MEMWB->MemtoReg){
      registers[MEMWB->writeReg] = MEMWB->readData;
    }
    else{
      registers[MEMWB->writeReg] = MEMWB->aluResult;
    }
  }
}
