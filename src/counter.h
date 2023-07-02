#ifndef TOMASULO_COUNTER_H
#define TOMASULO_COUNTER_H

#include "utils.h"

struct myCounter {
    int x = 0;
    bool jump() const { return x & 2; }
    void see(bool j) {
        if (j) {
            if (x != 3) ++x;
        }
        else {
            if (x) --x;
        }
    }
};

#endif //TOMASULO_COUNTER_H
