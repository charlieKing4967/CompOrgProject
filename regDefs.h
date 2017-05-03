unsigned int pc;
bool stall;
bool IFflush;
int stallCount;
int test;

// MEMORY LOCATIONS
uint32_t Memory[1200];
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

  unsigned int branchPC;

  bool RegDst;
  bool Branch;
  bool MemRead;
  bool MemWrite;
  bool RegWrite;
  bool MemtoReg;
  bool ByteData;
  bool HalfData;
  bool SignedData;

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
  bool ByteData;
  bool HalfData;
  bool SignedData;
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
