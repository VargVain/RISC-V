#ifndef MAIN_CPP_INSTRUCTION_H
#define MAIN_CPP_INSTRUCTION_H

enum Type {
    LUI, AUIPC, JAL, JALR, BEQ, BNE, BLT, BGE, BLTU,
    BGEU, LB, LH, LW, LBU, LHU, SB, SH, SW, ADDI,
    SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,
    END, NONE
};


std::string name[39] = {"LUI", "AUIPC", "JAL", "JALR", "BEQ", "BNE", "BLT", "BGE", "BLTU",
                        "BGEU", "LB", "LH", "LW", "LBU", "LHU", "SB", "SH", "SW", "ADDI",
                        "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SLLI", "SRLI", "SRAI",
                        "ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR",
                        "AND", "END", "NONE"};


struct Instruction {
    Type op = NONE;
    int imm = 0, rd = 0, rs1 = 0, rs2 = 0, tmp = 0;
    bool isJump = false,
         isCalc = false,
         isS = false,
         isL = false,
         isB = false,
         isI = false,
         isJ = false,
         isR = false,
         isU = false;
};

#endif //MAIN_CPP_INSTRUCTION_H
