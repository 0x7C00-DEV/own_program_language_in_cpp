#ifndef EXECUTE
#define EXECUTE
#include <vector>
#include "assembly.hpp"
#include <stack>
#include <cmath>

struct RunningFrame {
    int pc;
    std::stack<long long> sta;
    std::vector<long long> loc;
    RunningFrame(int pc) {
        this->pc = pc;
    }
};

struct OPL_Object {
    std::vector<long long> loc_mem;
};

OPL_Object* copy_object(OPL_Object* obj) {
    std::vector<long long> loc_mem;
    for (auto i : obj->loc_mem) loc_mem.push_back(i);
    auto a = new OPL_Object;
    a->loc_mem = loc_mem;
    return a;
}

class VirtualMachine {
public:
    VirtualMachine(std::vector<int> opc) {
        this->opc = opc;
        create_task_by_address(opc[0]); // opc[0] is main function address
    }

    int execute() {
        int i = get();
        switch (i) {
            case IMPORT_MODULE: { break; }

            case STACK_ADD: { long long a = pop(), b = pop(); push(a + b); break; }
            case STACK_SUB: { long long a = pop(), b = pop(); push(a - b); break; }
            case STACK_DIV: { long long a = pop(), b = pop(); push(a * b); break; }
            case STACK_MUL: { long long a = pop(), b = pop(); push(a / b); break; }
            case STACK_MOD: { long long a = pop(), b = pop(); push(a % b); break; }
            case STACK_LEFT: { long long a = pop(), b = pop(); push(a << b); break; }
            case STACK_RIGHT: { long long a = pop(), b = pop(); push(a >> b); break; }
            case STACK_POW: { long long a = pop(), b = pop(); push(pow(a, b)); break; }

            case POP: { pop(); break; }
            case PUSH: { push(get()); break; }
            case LOAD: { load(); break; }
            case STORE: {  break; }
            case JMP: { break; }
            case JMP_IF_TRUE: { break; }
            case JMP_IF_FALSE: { break; }
            case CALL: { break; }
            case RETURN: { break; }
            case LEAVE: { break; }
            case HALT: { exit(pop()); break; }
            case NEW_OBJECT: { push(new_object(pop())); break; }
            case MEMBER_SET: {

                break;
            }
            case STACK_COPY: {
                long long tmp = funcs[funcs.size() - 1].sta.top();
                push(tmp);
                break;
            }
            case DEEP_COPY: { push((long long)copy_object((OPL_Object*)pop())); break; }
            default: {
                printf("Unknown operator code %d\n", i);
                exit(-1);
            }
        }
    }

private:
    std::vector<int> opc;
    std::vector<RunningFrame> funcs;

    int get() {
        return opc[funcs[funcs.size() - 1].pc++];
    }

    long long pop() {
        long long tmp = funcs[funcs.size() - 1].sta.top();
        funcs[funcs.size() - 1].sta.pop();
        return tmp;
    }

    void load() {
        push(funcs[funcs.size() - 1].loc[pop()]);
    }

    void push(long long value) {
        funcs[funcs.size() - 1].sta.push(value);
    }

    long long new_object(int size) {
        OPL_Object *obj = new OPL_Object;
        obj->loc_mem.resize(size);
        return (long long) obj;
    } 

    void create_task_by_address(int addr) {
        funcs.push_back(RunningFrame(addr));
    }
};

#endif 