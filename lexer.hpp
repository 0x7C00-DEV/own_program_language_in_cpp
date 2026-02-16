#ifndef OPL_LEXER_HPP
#define OPL_LEXER_HPP
#include <iostream>
#include <vector>
#include <algorithm>

struct Token {
    std::string data;
    enum TokenKind {
        TT_INTEGER, TT_FLOAT, TT_STRING, TT_ID, TT_OP, TT_BOOL, TT_KEY
    } kind;

    Token(std::string data, TokenKind kind) {
        this->data = data;
        this->kind = kind;
    }

    void debug() { std::cout << "('" << data << "', " << kind << "')"; }
};

const std::vector<std::string> keys = {
        "if", "else", "for", "while", "def", "let", "class", "new", "break", "continue", "return",
        "import"
};

class Lexer {
public:
    Lexer(std::string expr) {
        this->expr = expr;
        pos = -1;
        advance();
        make_tokens();
    }
    std::vector<Token> tokens;
    std::vector<std::string> ops = {
            "++", "--", "+=", "-=", "*=", "/=", "%=", ">>=", "<<=", ">>", "<<", "==", ">=", "<=", "!=", "**", "||", "&&", "|=", "&="
    };
private:
    int pos;
    char current;
    std::string expr;

    void advance(int len = 1) {
        pos += len;
        current = 0;
        if (pos < expr.size())
            current = expr[pos];
    }

    bool back_comp(std::string data) {
        if (pos + data.size() > expr.size())
            return false;
        for (int i = 0; i < data.size(); ++i)
            if (expr[pos + i] != data[i])
                return false;
        return true;
    }

    Token make_string() {
        char eof = current;
        advance();
        std::string tmp;
        while (current && current != eof) {
            tmp += current;
            advance();
        }
        advance();
        return {tmp, Token::TokenKind::TT_STRING};
    }

    Token make_digit() {
        Token::TokenKind kind = Token::TT_INTEGER;
        std::string tmp;
        while (current && (std::isdigit(current) || current == '.')) {
            tmp += current;
            if (current == '.') kind = Token::TT_FLOAT;
            advance();
        }
        return {tmp, kind};
    }

    Token make_id() {
        Token::TokenKind kind = Token::TT_ID;
        std::string data;
        while (current && (('a' <= current && current <= 'z') || ('A' <= current && current <= 'Z') || current == '_' || std::isdigit(current))) {
            data += current;
            advance();
        }
        if (std::count(keys.begin(), keys.end(), data) == 1) kind = Token::TT_KEY;
        else if (data == "true" || data == "false") kind = Token::TT_BOOL;
        return {data, kind};
    }

    void make_tokens() {
        while (current) {
            if (std::isdigit(current)) tokens.push_back(make_digit());
            else if (('a' <= current && current <= 'z') || ('A' <= current && current <= 'Z') || current == '_')
                tokens.push_back(make_id());
            else if (current == '\'' || current == '"') tokens.push_back(make_string());
            else if (!std::isspace(current)) {
                std::string res;
                bool isf = false;
                for (auto i : ops)
                    if (back_comp(i) && i.size() > res.size())
                        res = i, isf = true;
                if (isf) {
                    tokens.emplace_back(res, Token::TT_OP);
                    advance(res.size());
                    continue;
                } else {
                    tokens.emplace_back(std::string(1, current), Token::TT_OP);
                    advance();
                }
            } else {
                advance();
            }
        }
    }
};

#endif //OPL_LEXER_HPP
