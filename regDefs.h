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
}MEMWB;
