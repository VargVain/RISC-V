#ifndef MAIN_CPP_INSTRUCTION_H
#define MAIN_CPP_INSTRUCTION_H

enum Type {
    LUI, AUIPC, JAL, JALR, BEQ, BNE, BLT, BGE, BLTU,
    BGEU, LB, LH, LW, LBU, LHU, SB, SH, SW, ADDI,
    SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,
    END
};

std::string name[38] = {"LUI", "AUIPC", "JAL", "JALR", "BEQ", "BNE", "BLT", "BGE", "BLTU",
                        "BGEU", "LB", "LH", "LW", "LBU", "LHU", "SB", "SH", "SW", "ADDI",
                        "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SLLI", "SRLI", "SRAI",
                        "ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND", "END"};

struct Instruction {
    Type op = END;
    int imm = 0, rd = 0, rs1 = 0, rs2 = 0;
    bool isJump() {
        return op == JAL || op == JALR || op == BEQ || op == BNE ||
               op == BLT || op == BGE || op == BLTU || op == BGEU;
    }
    bool isSL() {
        return op == LB || op == LH || op == LW || op == LBU ||
               op == LHU || op == SB || op == SH || op == SW;
    }
    bool isS() {
        return op == SB || op == SH || op == SW;
    }
    bool isL() {
        return op == LB || op == LH || op == LW || op == LBU ||
               op == LHU;
    }
    bool isB() {
        return op == BEQ || op == BNE ||
               op == BLT || op == BGE || op == BLTU || op == BGEU;
    }
    bool isCalc() {
        return op == LUI || op == AUIPC || op == ADDI || op == SLTI ||
               op == SLTIU || op == XORI || op == ORI || op == ANDI ||
               op == SLLI || op == SRLI || op == SRAI || op == ADD ||
               op == SUB || op == SLL || op == SLT || op == SLTU ||
               op == XOR || op == SRL || op == SRA || op == OR ||
               op == AND || op == END;
    }
    bool isI() {
        return op == ADDI || op == SLTI ||
               op == SLTIU || op == XORI || op == ORI || op == ANDI ||
               op == SLLI || op == SRLI || op == SRAI || op == JALR;
    }
    bool isUorJ() {
        return op == LUI || op == AUIPC || op == JAL;
    }
};

#endif //MAIN_CPP_INSTRUCTION_H
