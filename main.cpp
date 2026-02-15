#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include <fstream>

#define TEST

#ifdef TEST

void shell() {
    while (true) {
        std::string expr;
        printf("shell > ");
        std::getline(std::cin, expr);
        Lexer lexer(expr);
        Parser parser(lexer.tokens);
        for (auto i: parser.ast)
            decompiler(i, 0);
    }
}

void file() {
    std::ifstream ifs("D:\\CLionProjects\\OPL\\example");
    std::string buffer, res;
    while (std::getline(ifs, buffer))
        res += buffer + '\n';
    Lexer lexer(res);
    Parser parser(lexer.tokens);
    printf("OUTPUT:\n");
    Interpreter ip("<Program>", parser.ast);
}
#endif

int main() {
#ifdef TEST
    file();
#endif
    return 0;
}
