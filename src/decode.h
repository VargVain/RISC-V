#ifndef MAIN_CPP_DECODE_H
#define MAIN_CPP_DECODE_H

#include "utils.h"
#include "instruction.h"

Instruction Decode(unsigned int data) {
    Instruction ret;
    ret.rd = Read(data, 11, 7);
    ret.rs1 = Read(data, 19, 15);
    ret.rs2 = Read(data, 24, 20);
    switch (Read(data, 6, 0)) {
        case 0b0110111 : {
            ret.imm = Read(data, 31, 12) << 12;
            ret.op = LUI;
            break;
        }
        case 0b0010111 : {
            ret.imm = Read(data, 31, 12) << 12;
            ret.op = AUIPC;
            break;
        }
        case 0b1101111 : {
            ret.imm |= Read(data, 31, 20, 20);
            ret.imm |= Read(data, 21, 10, 1);
            ret.imm |= Read(data, 20, 11, 11);
            ret.imm |= Read(data, 12, 19, 12);
            if (Read(data, 31, 31)) ret.imm |= 0xfff00000;
            ret.op = JAL;
            break;
        }
        case 0b1100111 : {
            ret.imm = Read(data, 31, 20);
            if (Read(data, 31, 31)) ret.imm |= 0xfffff000;
            ret.op = JALR;
            break;
        }
        case 0b1100011 : {
            ret.imm |= Read(data, 31, 12, 12);
            ret.imm |= Read(data, 25, 10, 5);
            ret.imm |= Read(data, 8, 4, 1);
            ret.imm |= Read(data, 7, 11, 11);
            if (Read(data, 31, 31)) ret.imm |= 0xffffe000;
            switch (Read(data, 14, 12)) {
                case 0b000 : {
                    ret.op = BEQ;
                    break;
                }
                case 0b001 : {
                    ret.op = BNE;
                    break;
                }
                case 0b100 : {
                    ret.op = BLT;
                    break;
                }
                case 0b101 : {
                    ret.op = BGE;
                    break;
                }
                case 0b110 : {
                    ret.op = BLTU;
                    break;
                }
                case 0b111 : {
                    ret.op = BGEU;
                    break;
                }
            }
            break;
        }
        case 0b0000011 : {
            ret.imm = Read(data, 31, 20);
            if (Read(data, 31, 31)) ret.imm |= 0xfffff000;
            switch (Read(data, 14, 12)) {
                case 0b000 : {
                    ret.op = LB;
                    break;
                }
                case 0b001 : {
                    ret.op = LH;
                    break;
                }
                case 0b010 : {
                    ret.op = LW;
                    break;
                }
                case 0b100 : {
                    ret.op = LBU;
                    break;
                }
                case 0b101 : {
                    ret.op = LHU;
                    break;
                }
            }
            break;
        }
        case 0b0100011 : {
            ret.imm |= Read(data, 25, 11, 5);
            ret.imm |= Read(data, 7, 4, 0);
            if (Read(data, 31, 31)) ret.imm |= 0xfffff000;
            switch (Read(data, 14, 12)) {
                case 0b000 : {
                    ret.op = SB;
                    break;
                }
                case 0b001 : {
                    ret.op = SH;
                    break;
                }
                case 0b010 : {
                    ret.op = SW;
                    break;
                }
            }
            break;
        }
        case 0b0010011 : {
            if (data == 0x0ff00513) {
                ret.op = END;
                break;
            }
            ret.imm = Read(data, 31, 20);
            if (Read(data, 31, 31)) ret.imm |= 0xfffff000;
            switch (Read(data, 14, 12)) {
                case 0b000 : {
                    ret.op = ADDI;
                    break;
                }
                case 0b010 : {
                    ret.op = SLTI;
                    break;
                }
                case 0b011 : {
                    ret.op = SLTIU;
                    break;
                }
                case 0b100 : {
                    ret.op = XORI;
                    break;
                }
                case 0b110 : {
                    ret.op = ORI;
                    break;
                }
                case 0b111 : {
                    ret.op = ANDI;
                    break;
                }
                case 0b001 : {
                    ret.imm = Read(data, 24, 20);
                    ret.op = SLLI;
                    break;
                }
                case 0b101 : {
                    ret.imm = Read(data, 24, 20);
                    if (Read(data, 30, 30)) ret.op = SRAI;
                    else ret.op = SRLI;
                    break;
                }
            }
            break;
        }
        case 0b0110011 : {
            switch (Read(data, 14, 12)) {
                case 0b000 : {
                    if (Read(data, 30, 30)) ret.op = SUB;
                    else ret.op = ADD;
                    break;
                }
                case 0b001 : {
                    ret.op = SLL;
                    break;
                }
                case 0b010 : {
                    ret.op = SLT;
                    break;
                }
                case 0b011 : {
                    ret.op = SLTU;
                    break;
                }
                case 0b100 : {
                    ret.op = XOR;
                    break;
                }
                case 0b101 : {
                    if (Read(data, 30, 30)) ret.op = SRA;
                    else ret.op = SRL;
                    break;
                }
                case 0b110 : {
                    ret.op = OR;
                    break;
                }
                case 0b111 : {
                    ret.op = AND;
                    break;
                }
            }
            break;
        }
        default : {
            std::cout << "Invalid opcode found in " << std::bitset<32>(data) << ".";
            exit(0);
        }
    }
    return ret;
}

#endif //MAIN_CPP_DECODE_H
