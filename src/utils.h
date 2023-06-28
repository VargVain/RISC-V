#ifndef MAIN_CPP_UTILS_H
#define MAIN_CPP_UTILS_H

#include <bits/stdc++.h>

const int CAP = 32;

inline int hToD1(const std::string &s) {
    int x1, x2;
    if (s[0] >= '0' && s[0] <= '9') x1 = s[0] - '0';
    else x1 = s[0] - 'A' + 10;
    if (s[1] >= '0' && s[1] <= '9') x2 = s[1] - '0';
    else x2 = s[1] - 'A' + 10;
    return (x1 << 4) | x2;
}

inline int hToD2(const std::string &s) {
    int ret = 0;
    for (int i = 1; i <= 8; ++i) {
        int p;
        if (s[i] >= '0' && s[i] <= '9') p = s[i] - '0';
        else p = s[i] - 'A' + 10;
        if (p) ret |= p << (4 * (8 - i));
    }
    return ret;
}

int Read(const unsigned int &data, int ed, int x, int y) {
    return ((data >> ed) & ((1 << (x - y + 1)) - 1)) << y;
}

int Read(const unsigned int &data, int a, int b) {
    return (data >> b) & ((1 << (a - b + 1)) - 1);
}

int nxt(int x) {
    return (x == CAP - 1) ? 1 : (x + 1);
}

#endif //MAIN_CPP_UTILS_H
