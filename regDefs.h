unsigned int pc;
bool stall;
int stallCount;
int test;

// Need to make byte addressable
//uint8_t memory[40000];
int memory[10000];
uint32_t programMemory[10000];
uint32_t registers[32];


struct IFID_Reg {
  unsigned int PCplus1;

  unsigned int Opcode;
  unsigned int Rs;
  unsigned int Rt;
  unsigned int Rd;
  unsigned int shamtl;
  unsigned int funct;
  unsigned int jumpaddress;
  unsigned int immediate;
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
  uint32_t readRs;
  uint32_t readRt;
  uint32_t immediate;

  bool RegDst;
  bool Branch;
  bool MemRead;
  bool MemWrite;
  bool RegWrite;
  bool MemtoReg;

};

struct EXMEM_Reg {
  unsigned int PCplus1;
  unsigned int Opcode;
  unsigned int Rs;
  unsigned int Rt;
  unsigned int Rd;
  unsigned int shamtl;
  unsigned int funct;
  unsigned int jumpaddress;
  uint32_t readRt;
  uint32_t aluResult;

  unsigned int branchPC;

  unsigned int writeReg;

  bool Branch;
  bool MemRead;
  bool MemWrite;
  bool RegWrite;
  bool MemtoReg;
};

struct MEMWB_Reg {
  unsigned int PCplus1;
  unsigned int Opcode;
  unsigned int Rs;
  unsigned int Rt;
  unsigned int Rd;
  unsigned int shamtl;
  unsigned int funct;
  unsigned int jumpaddress;
  uint32_t readData;
  uint32_t aluResult;

  unsigned int writeReg;

  bool RegWrite;
  bool MemtoReg;
};
