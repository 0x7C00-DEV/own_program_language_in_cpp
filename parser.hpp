#ifndef OPL_PARSER_HPP
#define OPL_PARSER_HPP
#include <iostream>
#include <vector>
#include "lexer.hpp"
#include <vector>
#include <unordered_map>

class AST {
public:
    enum AKind {
        A_IF, A_BLOCK, A_STRING, A_INT, A_FLO, A_TRUE, A_FALSE, A_WHILE,
        A_FOR, A_CLASS, A_RETURN, A_BREAK, A_CONTINUE, A_BIN_OP, A_BIT_NOT,
        A_MEMBER_ACCESS, A_ID, A_ELEMENT_GET, A_CALL, A_NOT, A_ARRAY, A_SELF_INC,
        A_SELF_DEC, A_VAR_DEF, A_FUNC_DEFINE, A_SELF_OPERA, A_MEM_MALLOC, A_LAMBDA,
        A_NULL, A_IMPORT
    } kind;

    AST(AKind kind) {
        this->kind = kind;
    }

    ~AST() {
        delete &kind;
    }
};

class TypeNode {
public:
    enum TNKind { TN_ARRAY, TN_NORMAL, TN_FUNC } kind;
    TypeNode(TNKind kind) { this->kind = kind; }
};

class ArrayKind : public TypeNode {
public:
    TypeNode* basic_type;
    ArrayKind(TypeNode* basic_type) : TypeNode(TN_ARRAY) {
        this->basic_type = basic_type;
    }
};

class FuncKind : public TypeNode {
public:
    std::vector<TypeNode*> __in__;
    TypeNode* __out__;
    FuncKind(std::vector<TypeNode*> __in__, TypeNode* __out__) : TypeNode(TN_FUNC) {
        this->__in__ = __in__;
        this->__out__ = __out__;
    }
};

class NormalKind : public TypeNode {
public:
    std::string type;
    NormalKind(std::string type) : TypeNode(TN_NORMAL) {
        this->type = type;
    }
};

class TrueNode : public AST {
public:
    TrueNode() : AST(AST::A_TRUE) { }
};

class ImportNode : public AST {
public: 
    std::string path;
    ImportNode(std::string path) : AST(A_IMPORT) {
        this->path = path;
    }

    ~ImportNode() {
        delete &path;
    }
};

class FalseNode : public AST {
public:
    FalseNode() : AST(AST::A_FALSE) { }
};

const int pre = 1; // ++a;

const int npre = 0; // a++;

class SelfIncNode : public AST {
public:
    int ipre = 0;
    AST* id;
    SelfIncNode(AST* id, int i) : AST(A_SELF_INC) {
        this->id = id;
        this->ipre = i;
    }

    ~SelfIncNode() {
        delete &ipre;
        delete id;
    }
};

class MemoryMallocNode : public AST {
public:
    std::string name;
    std::vector<AST*> args;
    bool is_call_c;
    MemoryMallocNode(std::string name, std::vector<AST*> args, bool is_call_constructor) : AST(A_MEM_MALLOC) {
        this->name = name;
        this->args = args;
        this->is_call_c = is_call_constructor;
    }
    ~MemoryMallocNode() {
        delete &name;
        delete &args;
    }
};

class SelfDecNode : public AST {
public:
    int ipre = 0;
    AST* id;
    SelfDecNode(AST* id, int i) : AST(A_SELF_DEC) {
        this->id = id;
        this->ipre = i;
    }

    ~SelfDecNode() {
        delete id;
        delete &ipre;
    }
};

class BinOpNode : public AST {
public:
    std::string op;
    AST *left, *right;
    BinOpNode(std::string op, AST* left, AST* right) : AST(AST::A_BIN_OP) {
        this->op = op;
        this->right = right;
        this->left = left;
    }

    ~BinOpNode() {
        delete &op;
        delete left;
        delete right;
    }
};

class MemberAccessNode : public AST {
public:
    AST* parent;
    std::string member;
    MemberAccessNode(AST* left, std::string member) : AST(AST::A_MEMBER_ACCESS) {
        this->parent = left;
        this->member = member;
    }

    ~MemberAccessNode() {
        delete parent;
        delete &member;
    }
};

class StringNode : public AST {
public:
    std::string str;
    StringNode(std::string str) : AST(AST::A_STRING) {
        this->str = str;
    }

    ~StringNode() {
        delete &str;
    }
};

class IntegerNode : public AST {
public:
    std::string number;
    IntegerNode(std::string number) : AST(AST::A_INT) {
        this->number = number;
    }

    ~IntegerNode() {
        delete &number;
    }
};

class BitNotNode : public AST {
public:
    AST* expr;
    BitNotNode(AST* expr) : AST(AST::A_BIT_NOT) {
        this->expr = expr;
    }

    ~BitNotNode() {
        delete &expr;
    }
};

class IdNode : public AST {
public:
    std::string id;
    IdNode(std::string id) : AST(AST::A_ID) {
        this->id = id;
    }

    ~IdNode() {
        delete &id;
    }
};

class CallNode : public AST {
public:
    AST* func_name;
    std::vector<AST*> args;
    CallNode(AST* func_name, std::vector<AST*> args) : AST(AST::A_CALL) {
        this->func_name = func_name;
        this->args = args;
    }

    ~CallNode() {
        delete func_name;
        delete &args;
    }
};

class ElementGetNode : public AST {
public:
    AST* array_name;
    AST* position;
    ElementGetNode(AST* array_name, AST* position) : AST(AST::A_ELEMENT_GET) {
        this->array_name = array_name;
        this->position = position;
    }

    ~ElementGetNode() {
        delete array_name;
        delete position;
    }
};

class NotNode : public AST {
public:
    AST* expr;
    NotNode(AST* expr) : AST(AST::A_NOT) {
        this->expr = expr;
    }

    ~NotNode() {
        delete expr;
    }
};

class Block : public AST {
public:
    std::vector<AST*> codes;
    Block(std::vector<AST*> codes) : AST(A_BLOCK) {
        this->codes = codes;
    }

    ~Block() {
        delete &codes;
    }
};

class IfNode : public AST {
public:
    AST* condition;
    Block *if_true;
    Block *if_false;
    IfNode(AST* condition, Block* if_true, Block* if_false) : AST(A_IF) {
        this->condition = condition;
        this->if_false = if_false;
        this->if_true = if_true;
    }

    ~IfNode() {
        delete condition;
        delete if_false;
        delete if_true;
    }
};

class WhileNode : public AST {
public:
    AST* condition;
    Block* body;
    WhileNode(AST* condition, Block* body) : AST(A_WHILE) {
        this->condition = condition;
        this->body = body;
    }

    ~WhileNode() {
        delete condition;
        delete body;
    }
};

class LambdaNode : public AST {
public:
    std::vector<AST*> args;
    Block* body;
    LambdaNode(std::vector<AST*> args, Block* body) : AST(AST::A_LAMBDA) {
        this->args = args;
        this->body = body;
    }

    ~LambdaNode() {
        delete &args;
        delete body;
    }
};


class VarDefineNode : public AST {
public:
    std::string name;
    AST* init_value;
    TypeNode* vtype;
    VarDefineNode(std::string name, TypeNode* vtype, AST* init_value = nullptr) : AST(A_VAR_DEF) {
        this->name = name;
        this->init_value = init_value;
        this->vtype = vtype;
    }

    ~VarDefineNode() {
        delete &name;
        delete init_value;
    }
};

class FunctionNode : public AST {
public:
    Block* body;
    std::string name;
    std::vector<AST*> args;
    FuncKind* kid;
    FunctionNode(std::string name, std::vector<AST*> args, Block* body, FuncKind* fk) : AST(A_FUNC_DEFINE) {
        this->body = body;
        this->name = name;
        this->args = args;
        this->kid = fk;
    }

    ~FunctionNode() {
        delete body;
        delete &name;
        delete &args;
    }
};

class SelfOperator : public AST {
public:
    std::string op;
    AST* target;
    AST* value;
    SelfOperator(std::string op, AST* target, AST* value) : AST(A_SELF_OPERA) {
        this->op = op;
        this->target = target;
        this->value = value;
    }

    ~SelfOperator() {
        delete &op;
        delete target;
        delete value;
    }
};

class ForNode : public AST {
public:
    AST* init; 
    AST* is_continue;
    AST* change;
    Block* body;
    ForNode(AST* init, AST* is_continue, AST* change, Block* body) : AST(A_FOR) {
        this->init = init;
        this->is_continue = is_continue;
        this->change = change;
        this->body = body;
    }

    ~ForNode() {
        delete init;
        delete is_continue;
        delete change;
        delete body;
    }
};

class ContinueNode : public AST {
public:
    ContinueNode() : AST(A_CONTINUE) {}
};

class BreakNode : public AST {
public:
    BreakNode() : AST(A_BREAK) {}
};


class ReturnNode : public AST {
public:
    AST* value;
    ReturnNode(AST* value) : AST(A_RETURN) {
        this->value = value;
    }

    ~ReturnNode() {
        delete value;
    }
};

class ObjectNode : public AST {
public:
    std::string name;

    enum AccessState {
        PUBLIC, PRIVATE
    };

    std::unordered_map<std::string, AST*> members;
    std::unordered_map<std::string, AccessState> as;
    ObjectNode(std::string name, std::unordered_map<std::string, AST*> members, std::unordered_map<std::string, AccessState> as) : AST(A_CLASS) {
        this->members = members;
        this->name = name;
        this->as = as;
    }

    AST* get_constructor() {
        if (members.find("constructor") == members.end())
            return nullptr;
        return members[name];
    }

    ~ObjectNode() {
        delete &name;
    }
};

class NullNode : public AST {
public:
    NullNode() : AST(A_NULL) {}
};

class ArrayNode : public AST {
public:
    std::vector<AST*> elements;
    ArrayNode(std::vector<AST*> elements) : AST(AST::A_ARRAY){
        this->elements = elements;
    }

    ~ArrayNode() {
        delete &elements;
    }
};

class FloatNode : public AST {
public:
    std::string number;
    FloatNode(std::string number) : AST(AST::A_FLO) {
        this->number = number;
    }

    ~FloatNode() {
        delete &number;
    }
};

class Parser;

typedef AST*(Parser::*CALLBACK_FUNCTION)();


void make_error(std::string error_type, std::string error_info, Position error_pos) {
    std::cout << error_type << ": " << error_info << " at lin " << error_pos.lin << ", col " << error_pos.col << std::endl;
    exit(-1);
}

void make_error(std::string error_type, std::string error_info) {
    std::cout << error_type << ": " << error_info << " at EOF" << std::endl;
    exit(-1);
}

class SymbolTable {
public:
    enum SymbolBaseKind { S_CLASS, S_FUNC, S_VAR } kind;
    SymbolTable(SymbolBaseKind kind) { this->kind = kind; }
};

class VarTable : public SymbolTable{
public:
    std::string name;
    TypeNode* typeNode;
    VarTable(std::string name, TypeNode* typeNode) : SymbolTable(S_VAR) {
        this->name = name;
        this->typeNode = typeNode;
    }
};

class FunctionTable : public SymbolTable {
public:
    std::string name;
    FuncKind* fk;
    FunctionTable(std::string name, FuncKind* fk) : SymbolTable(S_FUNC) {
        this->name = name;
        this->fk = fk;
    }
};

class Parser {
public:
    Parser(std::vector<Token> tokens) {
        this->tokens = tokens;
        pos = -1;
        current = nullptr;
        advance();
        make_all();
        build_symbol_table();
    }
    std::vector<AST*> ast;

    const std::vector<std::string> self_operator = {
            "+=", "-=", "*=", "/=", "%=", ">>=", "<<=", "|=", "&=", "="
    };

    void make_all() {
        while (current) {
            if (match("if")) ast.push_back(make_if());
            else if (match("def")) ast.push_back(make_function_define());
            else if (match("while")) ast.push_back(make_while());
            else if (match("for")) ast.push_back(make_for());
            else if (match("class")) ast.push_back(make_class());
            else if (match("continue")) ast.push_back(make_continue());
            else if (match("import")) ast.push_back(make_import());
            else if (match("break")) ast.push_back(make_break());
            else if (match("return")) ast.push_back(make_return());
            else if (match("$")) ast.push_back(make_lambda());
            else if (match("new")) ast.push_back(make_malloc());
            else if (match("let")) {
                auto a = make_var_define_group();
                for (auto i : a) ast.push_back(i);
            }
            else {
                auto tmp = make_expression();
                if (std::count(self_operator.begin(), self_operator.end(), current->data) == 1 &&
                    (tmp->kind == AST::A_ID || tmp->kind == AST::A_MEMBER_ACCESS || tmp->kind == AST::A_ELEMENT_GET)) {
                    std::string op = current->data;
                    advance();
                    auto val = make_expression();
                    tmp = new SelfOperator(op, tmp, val);
                }
                ast.push_back(tmp);
                expect_data(";", get_pos());
            }
        }
    }

private:
    std::vector<Token> tokens;
    int pos;
    Token* current;

    std::unordered_map<std::string, SymbolTable> symbols;

    TypeNode* get_value_type(AST* a) {

    }

    void build_var(VarDefineNode* vdn) {

    }

    void build_function(FunctionNode *fn) {

    }

    void build_object(ObjectNode *oj) {

    }

    void build_symbol_table() {
        for (auto i : ast) {
            int t = i->kind;
            if (t == AST::A_VAR_DEF) build_var((VarDefineNode*)i);
            if (t == AST::A_FUNC_DEFINE) build_function((FunctionNode*)i);
            if (t == AST::A_CLASS) build_object((ObjectNode*)i);
        }
    }

    bool match(Token::TokenKind kind) { return current != nullptr && current->kind == kind; }

    bool match(std::string data) { return current != nullptr && current->data == data; }

    std::string expect_get(Token::TokenKind kind) {
        if (!match(kind))
            make_error("SyntaxError", "want '" + std::to_string(kind) + "' get '" + current->data + "'");
        auto tmp = current->data;
        advance();
        return tmp;
    }

    void expect_data(std::string name, Position _pos) {
        if (!current || !match(name))
            make_error("SyntaxError", "want '" + name + "', meet '" + ((current)? current->data : "None"), _pos);
        advance();
    }

    Position get_pos() { if (!current) { make_error("SyntaxError", "meet eof"); } return current->start_pos; }

    ImportNode* make_import() {
        expect_data("import", get_pos());
        auto path = expect_get(Token::TT_STRING);
        expect_data(";", get_pos());
        return new ImportNode(path);
    }

    LambdaNode* make_lambda() {
        expect_data("$", get_pos());
        auto args = make_area("(", ")", ",", &Parser::make_var_define);
        auto body = make_block();
        return new LambdaNode(args, body);
    }

    ContinueNode* make_continue() {
        ++pos;
        expect_data(";", get_pos());
        return new ContinueNode();
    }

    ReturnNode* make_return() {
        expect_data("return", get_pos());
        if (match(";")) {
            advance();
            return new ReturnNode(nullptr);
        }
        auto val = make_expression();
        expect_data(";", get_pos());
        return new ReturnNode(val);
    }

    BreakNode* make_break() {
        ++pos;
        expect_data(";", get_pos());
        return new BreakNode();
    }

    IfNode* make_if() {
        expect_data("if", get_pos());
        expect_data("(", get_pos());
        auto cond = make_expression();
        expect_data(")", get_pos());
        Block* body = make_block();
        Block* el = nullptr;
        if (match("else")) {
            advance();
            el = make_block();
        }
        return new IfNode(cond, body, el);
    }

    TypeNode* make_type() {
        // TypeName
        // [TypeName]
        // func(Type1, Type2, ..., TypeN) ReturnType
        if (match(Token::TT_ID) && !match("func")) {
            std::string tn = current->data;
            advance();
            return new NormalKind(tn);
        } else if (match("[")) {
            advance();
            auto t = make_type();
            expect_data("]", get_pos());
            return new ArrayKind(t);
        } else if (match("func")) {
            advance();
            expect_data("(", get_pos());
            std::vector<TypeNode*> type;
            while (current && !match(")")) {
                type.push_back(make_type());
                if (match(")")) break;
                expect_data(",", get_pos());
            }
            expect_data(")", get_pos());
            expect_data("->", get_pos());
            TypeNode* ret_type = make_type();
            return new FuncKind(type, ret_type);
        } else {
            std::cout << "Unknown type '" << ((current)? current->data : "None") << std::endl;
            exit(-1);
        }
    }

    FunctionNode* make_function_define() {
        if (match("def")) advance();
        std::string name = expect_get(Token::TT_ID);
        std::vector<AST*> vals = make_area("(", ")", ",", &Parser::make_var_define);
        int w = 0;
        TypeNode* type = nullptr;
        if (match("->")) {
            advance();
            type = make_type();
        }
        std::vector<TypeNode*> types;
        for (auto i : vals) types.push_back(((VarDefineNode*)i)->vtype);
        auto body = make_block();
        return new FunctionNode(name, vals, body, new FuncKind(types, type));
    }

    AST* make_var_define() {
        std::string name;
        AST* init_value = nullptr;
        name = expect_get(Token::TT_ID);
        expect_data(":", get_pos());
        TypeNode* tn = make_type();
        if (match("=")) {
            advance();
            init_value = make_expression();
        }
        return new VarDefineNode(name, tn, init_value);
    }

    MemoryMallocNode* make_malloc() {
        expect_data("new", get_pos());
        std::string name = expect_get(Token::TT_ID);
        std::vector<AST*> arg;
        bool is_call_constructor = false;
        if (match("(")) is_call_constructor = true, arg = make_area("(", ")", ",", &Parser::make_expression);
        return new MemoryMallocNode(name, arg, is_call_constructor);
    }

    WhileNode* make_while() {
        expect_data("while", get_pos());
        expect_data("(", get_pos());
        auto condition = make_expression();
        expect_data(")", get_pos());
        auto block = make_block();
        return new WhileNode(condition, block);
    }

    std::vector<AST*> make_var_define_group() {
        return make_area("let", ";", ",", &Parser::make_var_define);
    }

    NullNode* make_null () {
        expect_data("null", get_pos());
        return new NullNode();
    }

    ForNode* make_for() {
        expect_data("for", get_pos());
        expect_data("(", get_pos());
        AST *init, *is_continue, *change;
        Block *body;
        init = make_var_define();
        expect_data(";", get_pos());
        is_continue = make_expression();
        expect_data(";", get_pos());
        change = make_expression();
        expect_data(")", get_pos());
        body = make_block();
        return new ForNode(init, is_continue, change, body);
    }

    ObjectNode* make_class() {
        expect_data("class", get_pos());
        std::unordered_map<std::string, AST*> members;
        std::unordered_map<std::string, ObjectNode::AccessState> as;
        std::string name = expect_get(Token::TT_ID);
        expect_data("{", get_pos());
        ObjectNode::AccessState _as = ObjectNode::PRIVATE;
        while (current && !match("}")) {
            if (match("public")) advance(), _as = ObjectNode::PUBLIC;
            else if (match("private")) advance(),_as = ObjectNode::PRIVATE;
            if (match("def") || match("constructor")) {
                std::string name = current->data;
                if (name == "constructor") _as = ObjectNode::PUBLIC;
                auto tmp = make_function_define();
                members[tmp->name] = tmp;
                as[name] = _as;
                _as = ObjectNode::PRIVATE;
            } else if (match("let")) {
                auto vars = make_var_define_group();
                for (auto i : vars) members[((VarDefineNode*)i)->name] = i, as[((VarDefineNode*)i)->name] = _as;
            } else {
                auto v = (VarDefineNode*)make_var_define();
                members[v->name] = v;
                as[v->name] = _as;
                expect_data(";", get_pos());
            }
        }
        expect_data("}", get_pos());
        return new ObjectNode(name, members, as);
    }

    Block* make_block() {
        if (!match("{")) {
            std::vector<AST*> codes;
            if (match("if")) codes.push_back(make_if());
            else if (match("while")) codes.push_back(make_while());
            else if (match("import")) codes.push_back(make_import());
            else if (match("for")) codes.push_back(make_for());
            else if (match("continue")) codes.push_back(make_continue());
            else if (match("new")) codes.push_back(make_malloc());
            else if (match("break")) codes.push_back(make_break());
            else if (match("return")) codes.push_back(make_return());
            else if (match("let")) {
                auto a = make_var_define_group();
                for (auto i : a) codes.push_back(i);
            }
            else {
                auto tmp = make_expression();
                if (std::count(self_operator.begin(), self_operator.end(), current->data) == 1 &&
                    (tmp->kind == AST::A_ID || tmp->kind == AST::A_MEMBER_ACCESS || tmp->kind == AST::A_ELEMENT_GET)) {
                    std::string op = current->data;
                    advance();
                    auto val = make_expression();
                    tmp = new SelfOperator(op, tmp, val);
                }
                expect_data(";", get_pos());
                codes.push_back(tmp);
            }
            return new Block(codes);
        }
        expect_data("{", get_pos());
        std::vector<AST*> codes;
        while (current && !match("}")) {
            if (match("if")) codes.push_back(make_if());
            else if (match("while")) codes.push_back(make_while());
            else if (match("import")) codes.push_back(make_import());
            else if (match("for")) codes.push_back(make_for());
            else if (match("continue")) codes.push_back(make_continue());
            else if (match("new")) codes.push_back(make_malloc());
            else if (match("break")) codes.push_back(make_break());
            else if (match("return")) codes.push_back(make_return());
            else if (match("let")) {
                auto a = make_var_define_group();
                for (auto i : a) codes.push_back(i);
            }
            else {
                auto tmp = make_expression();
                if (std::count(self_operator.begin(), self_operator.end(), current->data) == 1 &&
                    (tmp->kind == AST::A_ID || tmp->kind == AST::A_MEMBER_ACCESS || tmp->kind == AST::A_ELEMENT_GET)) {
                    std::string op = current->data;
                    advance();
                    auto val = make_expression();
                    tmp = new SelfOperator(op, tmp, val);
                }
                expect_data(";", get_pos());
                codes.push_back(tmp);
            }
        }
        expect_data("}", get_pos());
        return new Block(codes);
    }

    void advance() {
        ++pos;
        current = nullptr;
        if (pos < tokens.size())
            current = &tokens[pos];
    }

    AST* make_bin_op_node(CALLBACK_FUNCTION left_process, std::vector<std::string> opers, CALLBACK_FUNCTION right_process = nullptr) {
        if (right_process == nullptr)
            right_process = left_process;
        AST* left = (this->*left_process)();
        while (current && std::count(opers.begin(), opers.end(), current->data) > 0) {
            std::string op = current->data;
            advance();
            AST* right = (this->*right_process)();
            left = new BinOpNode(op, left, right);
        }
        return left;
    }

    std::vector<AST*> make_area(std::string begin, std::string end, std::string split, CALLBACK_FUNCTION proc, int cnt = -1) {
        std::vector<AST*> res;
        expect_data(begin, get_pos());
        int count = 0;
        while (current && !match(end) && (cnt == -1 || ++count <= cnt)) {
            res.push_back((this->*proc)());
            if (match(end)) break;
            expect_data(split, get_pos());
        }
        expect_data(end, get_pos());
        return res;
    }

    AST* make_call_node() {
        return _make_call_node(make_member_access());
    }

    AST* _make_call_node(AST* name) {
        while (current && (match("(") || match("[") || match("."))) {
            if (match("(")) {
                auto args = make_area("(", ")", ",", &Parser::make_expression);
                name = new CallNode(name, args);
            } else if (match("[")) {
                advance();
                name = new ElementGetNode(name, make_expression());
                expect_data("]", get_pos());
            } else if (match(".")) {
                name = _make_member_access(name);
            }
        }
        return name;
    }

    AST* make_element_get_node() {
        return _make_element_get_node(make_member_access());
    }

    AST* _make_element_get_node(AST* name) {
        while (current && (match("["))) {
            advance();
            name = new ElementGetNode(name, make_expression());
            expect_data("]", get_pos());
        }
        return name;
    }

    AST* make_member_access() {
        AST* left = new IdNode(current->data);
        advance();
        return _make_member_access(left);
    }

    AST* _make_member_access(AST* left) {
        while (current && match(".")) {
            advance();
            left = new MemberAccessNode(left, current->data);
            advance();
        }
        return left;
    }

    AST* make_expression() {
        return make_bin_op_node(&Parser::make_expression5, {"||"});
    }

    AST* make_expression5() {
        return make_bin_op_node(&Parser::make_expression4, {"&&"});
    }

    AST* make_expression4() {
        return make_bin_op_node(&Parser::make_expression3, {">", "<", ">=", "<=", "==", "!="});
    }

    AST* make_expression3() {
        return make_bin_op_node(&Parser::make_expression2, {"+", "-"});
    }

    AST* make_expression2() {
        return make_bin_op_node(&Parser::make_expression1, {"*", "/", "<<", ">>", "%", "^", "|", "&"});
    }

    AST* make_array() {
        return new ArrayNode(make_area("[", "]", ",", &Parser::make_expression));
    }

    AST* make_expression1() {
        return make_bin_op_node(&Parser::make_value, {"**"});
    }

    AST* make_value() {
        if (match(Token::TT_INTEGER)) {
            auto tmp = new IntegerNode(current->data);
            advance();
            return tmp;
        } else if (match("$")) {
            return make_lambda();
        } else if (match("null")) {
            return make_null();
        } else if (match("++")) {
            advance();
            return new SelfIncNode(make_value(), pre);
        } else if (match("--")) {
            advance();
            return new SelfDecNode(make_value(), pre);
        } else if (match("new")) {
            return make_malloc();
        } else if (match(Token::TT_FLOAT)) {
            auto tmp = new FloatNode(current->data);
            advance();
            return tmp;
        } else if (match("[")) {
            auto tmp = make_array();
            while (match(".") || match("(") || match("[")) {
                if (match("."))
                    tmp = _make_member_access(tmp);
                if (match("("))
                    tmp = _make_call_node(tmp);
                if (match("["))
                    tmp = _make_element_get_node(tmp);
            }
            return tmp;
        } else if (match(Token::TT_STRING)) {
            auto t = new StringNode(current->data);
            advance();
            return t;
        } else if (match("~")) {
            advance();
            return new BitNotNode(make_value());
        } else if (match("!")) {
            advance();
            return new NotNode(make_value());
        } else if (match("-")) {
            advance();
            return new BinOpNode("*", new FloatNode("-1.0"), make_value());
        } else if (match("true")) {
            advance();
            return new TrueNode();
        } else if (match("false")) {
            advance();
            return new FalseNode();
        } else if (match(Token::TT_ID)) {
            auto tmp = make_call_node();
            while (match("++") || match("--")) {
                if (match("++")) {
                    advance();
                    tmp = new SelfIncNode(tmp, npre);
                }
                if (match("--")) {
                    advance();
                    tmp = new SelfDecNode(tmp, npre);
                }
            }
            return tmp;
        } else {
            expect_data("(", get_pos());
            auto tmp = make_expression();
            expect_data(")", get_pos());
            if (match("."))
                tmp = _make_member_access(tmp);
            if (match("("))
                tmp = _make_call_node(tmp);
            return tmp;
        }
    }
};

std::string print_indent(int indent) {
    std::string res;
    while (indent--) res += "    ";
    return res;
}

void decompiler(AST* a, int indent = 0, std::string fo = "") {
    switch (a->kind) {
        case AST::A_ID: {
            std::cout << print_indent(indent) << fo << "Id<" << ((IdNode*)a)->id << ">\n";
            break;
        }
        case AST::A_IF: {
            auto n = (IfNode*) a;
            std::cout << print_indent(indent) << fo << "IfNode {\n";
            decompiler(n->condition, indent + 1, "Condition: ");
            decompiler(n->if_true, indent + 1, "IfConditionIsTrue: ");
            if (n->if_false) decompiler(n->if_false, indent + 1, "IfNotTrue: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_BLOCK: {
            auto al = (Block*) a;
            std::cout << print_indent(indent) << fo << "Block {\n";
            for (int i = 0; i < al->codes.size(); ++i)
                decompiler(al->codes[i], indent + 1, std::to_string(i) + ": ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_STRING: {
            std::cout << print_indent(indent) << fo << "String('" << ((StringNode*)a)->str << "')\n";
            break;
        }
        case AST::A_INT: {
            std::cout << print_indent(indent) << fo << "Int('" << ((IntegerNode*)a)->number << "')\n";
            break;
        }
        case AST::A_FLO: {
            std::cout << print_indent(indent) << fo << "Float('" << ((FloatNode*)a)->number << "')\n";
            break;
        }
        case AST::A_ARRAY: {
            auto node = (ArrayNode*) a;
            std::cout << print_indent(indent) << fo << "Array: [\n";
            for (int i = 0; i < node->elements.size(); ++i)
                decompiler(node->elements[i], indent + 1, "E[" + std::to_string(i) + "] = ");
            std::cout << print_indent(indent) << "]\n";
            break;
        }
        case AST::A_TRUE: {
            std::cout << print_indent(indent) << fo << "Bool<true>\n";
            break;
        }
        case AST::A_FALSE: {
            std::cout << print_indent(indent) << fo << "Bool<false>\n";
            break;
        }
        case AST::A_WHILE: {
            std::cout << print_indent(indent) << fo << "WhileNode {\n";
            auto w = (WhileNode*) a;
            decompiler(w->condition, indent + 1, "Condition: ");
            decompiler(w->body, indent + 1, "Body: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_FOR: {
            std::cout << print_indent(indent) << fo << "ForLoopNode {\n";
            auto n = (ForNode*)a;
            decompiler(n->init, indent + 1, "ForLoopInit: ");
            decompiler(n->is_continue, indent + 1, "LoopCondition: ");
            decompiler(n->change, indent + 1, "ForLoopChange: ");
            decompiler(n->body, indent + 1, "ForLoopBody: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_CLASS: {
            auto node = (ObjectNode*) a;
            std::cout << print_indent(indent) << fo << "Class {\n";
            std::cout << print_indent(indent + 1) << "Name: " << node->name << std::endl;
            if (!node->members.empty()) {
                std::cout << print_indent(indent + 1) << "Members [\n";
                for (auto i : node->members)
                    decompiler(i.second, indent + 2, i.first + ": ");
                std::cout << print_indent(indent + 1) << "]\n";
            }
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_RETURN: {
            std::cout << print_indent(indent) << fo << "ReturnSignal(\n";
            decompiler(((ReturnNode*)a)->value, indent + 1, "ReturnValue: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_BREAK: {
            std::cout << print_indent(indent) << fo << "<BreakSignal>\n";
            break;
        }
        case AST::A_CONTINUE: {
            std::cout << print_indent(indent) << fo << "<ContinueSignal>\n";
            break;
        }
        case AST::A_NOT: {
            std::cout << print_indent(indent) << fo << "Not(\n";
            decompiler(((NotNode*)a)->expr, indent + 1);
            std::cout << print_indent(indent) << ")\n";
            break;
        }
        case AST::A_BIN_OP: {
            std::cout << print_indent(indent) << fo << "BinOpNode: <'" << ((BinOpNode*)a)->op << "'>{\n";
            decompiler(((BinOpNode*)a)->left, indent + 1, "Left: ");
            decompiler(((BinOpNode*)a)->right, indent + 1, "Right: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_BIT_NOT: {
            std::cout << print_indent(indent) << fo << "BitNode: ";
            decompiler(((BitNotNode*)a)->expr, indent + 1);
            break;
        }
        case AST::A_MEMBER_ACCESS: {
            std::cout << print_indent(indent) << fo << "MemberAccessNode {\n";
            decompiler(((MemberAccessNode*)a)->parent, indent + 1, "ParentClass: ");
            std::cout << print_indent(indent + 1) << "Member: " << ((MemberAccessNode*)a)->member << std::endl;
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_ELEMENT_GET: {
            auto node = (ElementGetNode*)a;
            std::cout << print_indent(indent) << fo << "ElementGetNode {\n";
            decompiler(node->array_name, indent + 1, "ArrayAddress: ");
            decompiler(node->position, indent + 1, "Position: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_CALL: {
            std::cout << print_indent(indent) << fo << "CallNode { \n";
            decompiler(((CallNode*)a)->func_name, indent + 1, "FuncAddress: ");
            std::cout << print_indent(indent + 1) << "Args: [" ;
            std::string ind;
            if (!((CallNode*)a)->args.empty()) {
                printf("\n");
                ind = print_indent(indent + 1);
                for (int i = 0; i < ((CallNode *) a)->args.size(); ++i)
                    decompiler(((CallNode *) a)->args[i], indent + 2, "Args[" + std::to_string(i) + "]: ");
            }
            std::cout << ind << "]\n";
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_SELF_INC: {
            auto node = (SelfIncNode*) a;
            std::cout << print_indent(indent) << fo << "SelfInc {\n";
            std::cout << print_indent(indent + 1) << "PRE: " << node->ipre << std::endl;
            decompiler(node->id, indent + 1, "Address: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_SELF_DEC: {
            auto node = (SelfDecNode*) a;
            std::cout << print_indent(indent) << fo << "SelfDec {\n";
            std::cout << print_indent(indent + 1) << "PRE: " << node->ipre << std::endl;
            decompiler(node->id, indent + 1, "Address: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_VAR_DEF: {
            auto v = (VarDefineNode*) a;
            std::cout << print_indent(indent) << fo << "VarDefine {\n";
            std::cout << print_indent(indent + 1) << "VarName: " << v->name << "\n";
            if (v->init_value) decompiler(v->init_value, indent + 1, "InitValue: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_FUNC_DEFINE: {
            auto fn = (FunctionNode*) a;
            auto fn_arg = fn->args;
            std::cout << print_indent(indent) << fo << "FunctionDefine<'" << fn->name << "'> {\n";
            std::cout << print_indent(indent + 1) << "FunctionArgs: ";
            if (fn_arg.empty()) { std::cout << "[ ]\n"; }
            else {
                std::cout << "[\n";
                for (int i = 0; i < fn_arg.size(); ++i)
                    decompiler(fn_arg[i], indent + 2, std::to_string(i) + ": ");
                std::cout << print_indent(indent + 1) << "]\n";
            }
            decompiler(fn->body, indent + 1, "FuncBody: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_SELF_OPERA: {
            auto sa = (SelfOperator*) a;
            std::cout << print_indent(indent) << fo << "SelfOperator<'" + sa->op << "'> {\n";
            decompiler(sa->target, indent + 1, "Target: ");
            decompiler(sa->value, indent + 1, "Value: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_LAMBDA: {
            std::cout << print_indent(indent) << fo << "Lambda {\n";
            auto ln = (LambdaNode*) a;
            std::cout << print_indent(indent + 1) << "Args: (\n";
            for (int i = 0; i < ln->args.size(); ++i)
                decompiler(ln->args[i], indent + 2, std::to_string(i) + ": ");
            std::cout << print_indent(indent + 1) << ")\n";
            decompiler(ln->body, indent + 1, "Body: ");
            std::cout << print_indent(indent) << "}\n";
            break;
        }
        case AST::A_MEM_MALLOC: {
            std::cout << print_indent(indent) << fo << "MemoryMalloc (\n";
            auto n = (MemoryMallocNode*) a;
            for (int i = 0; i < n->args.size(); ++i)
                decompiler(n->args[i], indent + 1, std::to_string(i) + ": ");
            std::cout << print_indent(indent) << ")\n";
            break;
        }
        case AST::A_NULL: {
            std::cout << print_indent(indent) << fo << "Null\n";
            break;
        }
        case AST::A_IMPORT: {
            std::cout << print_indent(indent) << "Import(" << ((ImportNode*)a)->path << ")\n";
            break;
        }
    }
}

#endif