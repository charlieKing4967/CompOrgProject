unsigned int pc;

int memory[10000];
int registers[32];


struct IFID_Reg {
  int instruction;
  unsigned int PCplus1;
};

struct IDEX_Reg {
  unsigned int PCplus1;
  unsigned int Opcode;
  unsigned int Rs;
  unsigned int Rt;
  unsigned int Rd;
  unsigned int shamtl;
  unsigned int funct;
  unsigned int jumpaddress;
  int readRs;
  int readRt;
  int immediate;

  bool RegDst;
  bool Branch;
  bool MemRead;
  bool MemWrite;
  bool RegWrite;
  bool MemtoReg;

}IDEX;

struct EXMEM_Reg {
  unsigned int PCplus1;
  unsigned int Opcode;
  unsigned int Rs;
  unsigned int Rt;
  unsigned int Rd;
  unsigned int shamtl;
  unsigned int funct;
  unsigned int jumpaddress;
  int readRt;
  int aluResult;

  unsigned int branchPC;

  unsigned int writeReg;

  bool Branch;
  bool MemRead;
  bool MemWrite;
  bool RegWrite;
  bool MemtoReg;
}EXMEM;

struct MEMWB_Reg {
  unsigned int PCplus1;
  unsigned int Opcode;
  unsigned int Rs;
  unsigned int Rt;
  unsigned int Rd;
  unsigned int shamtl;
  unsigned int funct;
  unsigned int jumpaddress;
  int readData;
  int aluResult;

  unsigned int writeReg;

  bool RegWrite;
  bool MemtoReg;
}MEMWB;
