#ifndef MAIN_CPP_EXECUTE_H
#define MAIN_CPP_EXECUTE_H

#include "utils.h"
#include "instruction.h"

struct RS_ele;

unsigned int Compute(const Instruction& ins, int V1, int V2, int pc) {
    int imm = ins.imm;
    switch (ins.op) {
        case LUI : {
            return imm;
        }
        case AUIPC : {
            return pc + imm;
        }
        case ADDI : {
            return V1 + imm;
        }
        case SLTI : {
            return V1 < imm;
        }
        case SLTIU : {
            return (unsigned int)V1 < (unsigned int)imm;
        }
        case XORI : {
            return V1 ^ imm;
        }
        case ORI : {
            return V1 | imm;
        }
        case ANDI : {
            return V1 & imm;
        }
        case SLLI : {
            return V1 << imm;
        }
        case SRLI : {
            return (unsigned int)V1 >> imm;
        }
        case SRAI : {
            return V1 >> imm;
        }
        case ADD : {
            return V1 + V2;
        }
        case SUB : {
            return V1 - V2;
        }
        case SLL : {
            return V1 << (V2 & 0b11111);
        }
        case SLT : {
            return V1 < V2;
        }
        case SLTU : {
            return (unsigned int)V1 < (unsigned int)V2;
        }
        case XOR : {
            return V1 ^ V2;
        }
        case SRL : {
            return (unsigned int)V1 >> (V2 & 0b11111);
        }
        case SRA : {
            return V1 >> (V2 & 0b11111);
        }
        case OR : {
            return V1 | V2;
        }
        case AND : {
            return V1 & V2;
        }
        case END : {
            return 0;
        }
        case JAL : {
            return pc;
        }
        case JALR : {
            return pc;
        }
        case BEQ : {
            return V1 == V2;
        }
        case BNE : {
            return V1 != V2;
        }
        case BLT : {
            return V1 < V2;
        }
        case BGE : {
            return V1 >= V2;
        }
        case BLTU : {
            return (unsigned int)V1 < (unsigned int)V2;
        }
        case BGEU : {
            return (unsigned int)V1 >= (unsigned int)V2;
        }
        default : {
            std::cout << "Invalid opcode when computing.";
            exit(0);
        }
    }
}

unsigned Load(const Instruction &ins, int dest) {
    switch (ins.op) {
        case LB : {
            int offset = dest % 4;
            int tmp = (mem[dest - offset] >> (offset << 3)) & 0xff;
            if (tmp >> 7) tmp |= 0xffffff00;
            return tmp;
        }
        case LH : {
            int offset = dest % 4;
            int tmp = (mem[dest - offset] >> (offset << 3)) & 0xffff;
            if (tmp >> 15) tmp |= 0xffff0000;
            return tmp;
        }
        case LW : {
            return mem[dest];
        }
        case LBU : {
            int offset = dest % 4;
            return (mem[dest - offset] >> (offset << 3)) & 0xff;
        }
        case LHU : {
            int offset = dest % 4;
            return (mem[dest - offset] >> (offset << 3)) & 0xffff;
        }
        default : {
            std::cout << "Invalid opcode when loading.";
            exit(0);
        }
    }
}

unsigned int Store(const Instruction &ins, int value) {
    switch (ins.op) {
        case SB : {
            return (unsigned int)(value & 0xff);
        }
        case SH : {
            return (unsigned int)(value & 0xffff);
        }
        case SW : {
            return (unsigned int)value;
        }
        default : {
            std::cout << "Invalid opcode when storing.";
            exit(0);
        }
    }
}

#endif //MAIN_CPP_EXECUTE_H
