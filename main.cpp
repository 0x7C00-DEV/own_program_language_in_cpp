#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include <fstream>

#define TEST

#ifdef RELEASE

void start(int argc, char** argv) {
    if (argc <= 1) {
        printf("Usage: %s [FILE_NAME]", argv[0]);
        return;
    }
    std::string name = argv[1];
    std::ifstream ifs(name);
    std::string data, buffer;
    while (std::getline(ifs, buffer))
        data += buffer + '\n';
    Lexer lexer(data);
    Parser parser(lexer.tokens);
    ModuleManager* mg = new ModuleManager;
    Interpreter ip("<Program>", parser.ast, mg);
}

#endif

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
    ModuleManager* mg = new ModuleManager;
    Interpreter ip("<Program>", parser.ast, mg);

    std::cout << std::endl << std::endl;
    std::cout << "Module import state: \n";
    for (auto i : mg->count)
        std::cout << "\t" << i.first << std::endl;
}

void file() {
    run(R"(D:\CLionProjects\OPL\tests\bin_tree)");
}
#endif

int main(int argc, char** argv) {
#ifdef TEST
    file();
#endif
#ifdef RELEASE
    start(argc, argv);
#endif
    return 0;
}
