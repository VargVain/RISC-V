#ifndef MAIN_CPP_SLB_H
#define MAIN_CPP_SLB_H

#include "utils.h"
#include "instruction.h"

struct SLB_ele {
    Instruction ins;
    Status status = commit;
    int dest = 0, D = 0, V = 0, QD = 0, QV = 0;
};

struct mySLB {
    std::vector<SLB_ele> v;
    int hd = 1;
    int in = 0;
    mySLB() {
        v.emplace_back(SLB_ele());
        v.resize(CAP);
    }
    int insert(const SLB_ele &ele) {
        int n = nxt(in);
        if (in && n == frt(hd)) return 0;
        v[n] = ele;
        in = n;
        return n;
    }
    void clear() {
        v.clear();
        v.emplace_back(SLB_ele());
        v.resize(CAP);
        hd = 1;
        in = 0;
    }
};

#endif //MAIN_CPP_SLB_H
