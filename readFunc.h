#include <fstream>

void readProgram(uint32_t* programMemory, std::string fileName) {

    const char* fileName_c = fileName.c_str();
    FILE* programFile = fopen(fileName_c, "r");

    int line = 0;
    char c = 0;

    while (c != EOF) {
        fscanf(programFile, "%10x", &programMemory[line]);
        do {
            c = getc(programFile);
        } while (c != '\n' && c != EOF);
        line++;
    }
    return;
}
