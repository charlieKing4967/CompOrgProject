 unsigned int pc;

 int memory[10000];

 struct IFID_Reg {
   unsigned int Opcode;
   unsigned int Rs;
   unsigned int Rt;
   unsigned int Rd;
   unsigned int shamtl;
   unsigned int funct;
   short int immediate;
   unsigned int jumpaddress;
   unsigned int PCplus4;

   bool RegDst;
   bool RegWrite;
   bool ALUSrc;
   bool PCSrc;
   bool MemRead;
   bool MemWrite;
   bool MemtoReg;
 } IFID;

 struct IDEX_Reg {
   bool RegDst;
   bool RegWrite;
   bool ALUSrc;
   bool PCSrc;
   bool MemRead;
   bool MemWrite;
   bool MemtoReg;
 }IDEX;

 struct EXMEM_Reg {
   bool RegDst;
   bool RegWrite;
   bool ALUSrc;
   bool PCSrc;
   bool MemRead;
   bool MemWrite;
   bool MemtoReg;
 }EXMEM;

 struct MEMWB_Reg {
   bool RegDst;
   bool RegWrite;
   bool ALUSrc;
   bool PCSrc;
   bool MemRead;
   bool MemWrite;
   bool MemtoReg;
 }MEMWB;
