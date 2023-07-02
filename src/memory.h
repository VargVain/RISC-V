#ifndef MAIN_CPP_MEMORY_H
#define MAIN_CPP_MEMORY_H

#include "utils.h"

extern unsigned int mem[1000000];
extern int reg[32];
extern bool b;

void MemRead() {
    memset(mem, 0, sizeof(mem));
    memset(reg, 0, sizeof(reg));
    std::string s;
    int pos = 0;
    while (std::cin >> s) {
        if (s[0] != '@') {
            mem[pos] |= hToD1(s);
            for (int i = 1; i < 4; ++i) {
                std::cin >> s;
                mem[pos] |= hToD1(s) << (i << 3);
            }
            pos += 4;
        } else {
            pos = hToD2(s);
        }
    }
    if (mem[5840] == 253) b = true;
}

#endif //MAIN_CPP_MEMORY_H
