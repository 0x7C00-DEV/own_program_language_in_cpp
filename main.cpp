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

void run(std::string name) {
    std::ifstream ifs(name);
    std::string buffer, res;
    while (std::getline(ifs, buffer))
        res += buffer + '\n';
    Lexer lexer(res);
    Parser parser(lexer.tokens);
    printf("[%s] OUTPUT:\n", name.c_str());
    Interpreter ip("<Program>", parser.ast);
}

void file() {
    run(R"(D:\CLionProjects\OPL\tests\bin_tree)");
    run(R"(D:\CLionProjects\OPL\tests\lambda)");
}
#endif

int main() {
#ifdef TEST
    file();
#endif
    return 0;
}
