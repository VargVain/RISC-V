#ifndef MAIN_CPP_ROB_H
#define MAIN_CPP_ROB_H

#include "utils.h"
#include "instruction.h"

enum Status {
    not_ready, execute, write, commit
};

struct ROB_ele {
    Instruction ins;
    Status status = commit;
    unsigned int ret = 0, ret2 = 0;
};

struct myROB {
    std::vector<ROB_ele> v;
    int hd = 1;
    int in = 0;
    myROB() {
        v.emplace_back(ROB_ele());
        v.resize(CAP);
    }
    int insert(const ROB_ele &ele) {
        int n = nxt(in);
        v[n] = ele;
        in = n;
        return n;
    }
};

#endif //MAIN_CPP_ROB_H
