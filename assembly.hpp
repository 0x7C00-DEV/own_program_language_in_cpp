//
// Created by User on 2026/2/16.
//

#ifndef OPL_ASSEMBLY_HPP
#define OPL_ASSEMBLY_HPP

enum {
    IMPORT_MODULE,

    STACK_ADD,
    STACK_SUB,
    STACK_DIV,
    STACK_MUL,
    STACK_MOD,
    STACK_LEFT,
    STACK_RIGHT,
    STACK_POW,

    POP,
    PUSH,
    LOAD,
    STORE,

    JMP,
    JMP_IF_TRUE,
    JMP_IF_FALSE,
    CALL,
    RETURN,
    LEAVE,
    HALT,

    NEW_OBJECT,
    MEMBER_SET,
    STACK_COPY,
    DEEP_COPY
};

#endif //OPL_ASSEMBLY_HPP
