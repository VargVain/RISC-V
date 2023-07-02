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
        if (in && n == frt(hd)) return 0;
        v[n] = ele;
        in = n;
        return n;
    }
    void clear() {
        v.clear();
        v.emplace_back(ROB_ele());
        v.resize(CAP);
        hd = 1;
        in = 0;
    }
};

#endif //MAIN_CPP_ROB_H
