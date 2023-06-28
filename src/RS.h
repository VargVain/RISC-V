#ifndef MAIN_CPP_RS_H
#define MAIN_CPP_RS_H

#include "utils.h"
#include "instruction.h"
#include "ROB.h"

struct RS_ele {
    Instruction ins;
    Status status = not_ready;
    int dest = 0, V1 = 0, V2 = 0, Q1 = 0, Q2 = 0, ret = 0;
};

struct myRS {
    std::vector<RS_ele> v;
    int insert(const RS_ele &ele) {
        for (int i = 0; i < v.size(); ++i) {
            if (v[i].status == commit) {
                v[i] = ele;
                return i;
            }
        }
        v.push_back(ele);
        return (int)v.size() - 1;
    }
};

#endif //MAIN_CPP_RS_H
