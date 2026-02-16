#ifndef OPL_LEXER_HPP
#define OPL_LEXER_HPP
#include <iostream>
#include <vector>
#include <algorithm>

struct Position {
    int lin, col;
    Position(int l, int c) {
        this->lin = l;
        this->col = c;
    }

    Position() { lin = 0; col = 0; }

    std::string debug() {
        return "[" + std::to_string(lin) + ", " + std::to_string(col) + "]";
    }
};

struct Token {
    std::string data;
    enum TokenKind {
        TT_INTEGER, TT_FLOAT, TT_STRING, TT_ID, TT_OP, TT_BOOL, TT_KEY
    } kind;

    Position start_pos, end_pos;

    Token(std::string data, TokenKind kind, Position start_pos, Position end_pos) {
        this->data = data;
        this->kind = kind;
        this->start_pos = start_pos;
        this->end_pos = end_pos;
    }

    void debug() { std::cout << "('" << data << "', " << kind << "')" << start_pos.debug() << end_pos.debug(); }
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
        lin = col = 1;
        advance();
        make_tokens();
    }

    int lin, col;
    std::vector<Token> tokens;
    std::vector<std::string> ops = {
            "++", "--", "+=", "-=", "*=", "/=", "%=", ">>=", "<<=", ">>", "<<", "==", ">=", "<=", "!=", "**", "||", "&&", "|=", "&="
    };
private:
    int pos;
    char current;
    std::string expr;

    Position make_pos() {
        return Position(lin, col);
    }

    void advance(int len = 1) {
        pos += len;
        current = 0;
        ++col;
        if (pos < expr.size()) {
            current = expr[pos];
            if (current == '\n' || current == '\r') {
                ++lin;
                col = 1;
            }
        }
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
        auto begin = make_pos();
        char eof = current;
        advance();
        std::string tmp;
        while (current && current != eof) {
            tmp += current;
            advance();
        }
        advance();
        auto end = make_pos();
        return Token(tmp, Token::TT_STRING, begin, end);
    }

    Token make_digit() {
        auto begin = make_pos();
        Token::TokenKind kind = Token::TT_INTEGER;
        std::string tmp;
        while (current && (std::isdigit(current) || current == '.')) {
            tmp += current;
            if (current == '.') kind = Token::TT_FLOAT;
            advance();
        }
        auto end_pos = make_pos();
        return Token(tmp, kind, begin, end_pos);
    }

    Token make_id() {
        auto begin = make_pos();
        Token::TokenKind kind = Token::TT_ID;
        std::string data;
        while (current && (('a' <= current && current <= 'z') || ('A' <= current && current <= 'Z') || current == '_' || std::isdigit(current))) {
            data += current;
            advance();
        }
        if (std::count(keys.begin(), keys.end(), data) == 1) kind = Token::TT_KEY;
        else if (data == "true" || data == "false") kind = Token::TT_BOOL;
        auto end = make_pos();
        return Token(data, kind, begin, end);
    }

    void skip() {
        while (current && (current != '\n' && current != '\r'))
            advance();
    }

    void make_tokens() {
        while (current) {
            if (std::isdigit(current)) tokens.push_back(make_digit());
            else if (('a' <= current && current <= 'z') || ('A' <= current && current <= 'Z') || current == '_')
                tokens.push_back(make_id());
            else if (current == '\'' || current == '"') tokens.push_back(make_string());
            else if (current == '#') skip();
            else if (!std::isspace(current)) {
                std::string res;
                bool isf = false;
                for (auto i : ops)
                    if (back_comp(i) && i.size() > res.size())
                        res = i, isf = true;
                if (isf) {
                    tokens.emplace_back(res, Token::TT_OP, make_pos(), make_pos());
                    advance(res.size());
                    continue;
                } else {
                    tokens.emplace_back(std::string(1, current), Token::TT_OP, make_pos(), make_pos());
                    advance();
                }
            } else {
                advance();
            }
        }
    }
};

#endif //OPL_LEXER_HPP
