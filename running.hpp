#ifndef OPL_RUNNING_HPP
#define OPL_RUNNING_HPP
#include "stack"
#include "vector"
#include "iostream"

struct Frame {
    std::stack<long long> s;
    std::vector<long long> local;
    int pc;
};



#endif //OPL_RUNNING_HPP
