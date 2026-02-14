#ifndef OPL_INTERPRETER_HPP
#define OPL_INTERPRETER_HPP
#include <stdlib.h>
#include <cstring>
#include "parser.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>

class Value {
public:
    enum ValueKind {
        V_FLOAT, V_INT, V_STRING, V_BOOL, V_ARRAY, V_FUNC, V_OBJECT, V_NULL, V_RT_RESULT
    } kind;

    Value(ValueKind kind) {
        this->kind = kind;
    }

    virtual void set(Value*) { operator_not_supposed_err("="); }

    virtual Value* bit_not() { operator_not_supposed_err("~"); }

    virtual Value* copy() { operator_not_supposed_err("copy"); }

    virtual Value* add(Value*) { operator_not_supposed_err("+"); }

    virtual Value* sub(Value*) { operator_not_supposed_err("-"); }

    virtual Value* div(Value*) { operator_not_supposed_err("/"); }

    virtual Value* mul(Value*) { operator_not_supposed_err("*"); }

    virtual Value* left_move(Value*) { operator_not_supposed_err("<<"); }

    virtual std::string str() { operator_not_supposed_err("basicString"); }

    void expect(Value* v, ValueKind vk) {
        if (v->kind != vk) {
            std::cout << "InterpreterWantError: want " << vk << ", meed " << v->kind << std::endl;
            exit(-1);
        }
    }

    virtual Value* right_move(Value*) { operator_not_supposed_err(">>"); }

    virtual Value* is_eq(Value*) { operator_not_supposed_err("=="); }

    virtual Value* not_eq_(Value*) { operator_not_supposed_err("!="); }

    virtual Value* cond_not() { operator_not_supposed_err("!"); }

    virtual Value* big(Value*) { operator_not_supposed_err(">"); }

    virtual Value* less(Value*) { operator_not_supposed_err("<"); }

    virtual Value* big_or_eq(Value*) { operator_not_supposed_err(">="); }

    virtual void element_set(Value*, Value*) { operator_not_supposed_err("[]="); }

    virtual Value* less_or_eq(Value*) { operator_not_supposed_err("<="); }

    virtual Value* mod(Value*) { operator_not_supposed_err("%"); }

    virtual Value* _pow(Value*) { operator_not_supposed_err("**"); }

    virtual Value* element_get(Value*) { operator_not_supposed_err("[]"); }

    virtual Value* call(Value*) { operator_not_supposed_err("()"); }

    virtual Value* bit_not(Value*) { operator_not_supposed_err("~"); }

    virtual Value* bit_or(Value*) { operator_not_supposed_err("|"); }

    virtual Value* bit_and(Value*) { operator_not_supposed_err("&"); }

    virtual Value* __call__() { operator_not_supposed_err("()"); }

    virtual Value* cond_or(Value*) { operator_not_supposed_err("||"); }

    virtual Value* cond_and(Value*) { operator_not_supposed_err("&&"); }

    void operator_not_supposed_err(std::string op) {
        std::cout << "OperatorNotOverloadError: '" << op << "'" << std::endl;
        exit(-1);
    }
};

class RTResult : public Value {
public:
    enum SignalKind {
        S_BREAK, S_CONTINUE, S_RETURN, S_NONE
    } kind;

    RTResult(SignalKind kind) : Value(V_RT_RESULT) {
        this->kind = kind;
    }
};

class Null : public Value {
public:
    Null(): Value(V_NULL) {}

    Value* copy() override {
        return new Null();
    }

    std::string str() override { return "Null"; }
};

class Bool : public Value {
public:
    bool b;
    Bool(std::string bl) : Value(V_BOOL) {
        b = (bl == "true");
    }

    Value* copy() override {
        return new Bool(b);
    }

    std::string str() override {
        if (b) return "True";
        return "False";
    }

    Bool(bool b) : Value(V_BOOL) {
        this->b = b;
    }

    void set(Value *v) override {
        expect(v, V_BOOL);
        this->b = ((Bool*)v)->b;
    }

    Value* cond_not() override {
        return new Bool(!b);
    }

    Value* cond_and(Value* other) override {
        return new Bool(b && ((Bool*)other)->b);
    }

    Value* cond_or(Value* other) override {
        return new Bool(b || ((Bool*)other)->b);
    }
};

class Integer : public Value {
public:
    std::string number;
    Integer(std::string number) : Value(V_INT) {
        this->number = number;
    }

    std::string str() override {
        return number;
    }

    Value* copy() override  {
        return new Integer(number);
    }

    Value* bit_not() override {
        return new Integer(std::to_string(~std::stoi(number)));
    }

    Value* is_eq(Value* other) override {
        return new Bool(this->number == ((Integer*)other)->number && this->kind == other->kind);
    }

    Value* not_eq_(Value* other) override {
        return this->is_eq(other)->cond_not();
    }

    Value* big(Value* other) override {
        return new Bool(std::stoi(this->number) > std::stoi(((Integer*)other)->number));
    }

    Value* less(Value* other) override {
        return new Bool(std::stoi(this->number) < std::stoi(((Integer*)other)->number));
    }

    Value* less_or_eq(Value* other) override {
        return new Bool(std::stoi(this->number) <= std::stoi(((Integer*)other)->number));
    }

    Value* big_or_eq(Value* other) override {
        return new Bool(std::stoi(this->number) >= std::stoi(((Integer*)other)->number));
    }

    Value* left_move(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stoi(this->number) << std::stoi(((Integer*)other)->number)
                )
        );
    }

    Value* right_move(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stoi(this->number) >> std::stoi(((Integer*)other)->number)
                )
        );
    }

    Value* mod(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stoi(this->number) % std::stoi(((Integer*)other)->number)
                )
        );
    }

    Value* bit_or(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stoi(this->number) | std::stoi(((Integer*)other)->number)
                )
        );
    }

    Value* bit_and(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stoi(this->number) && std::stoi(((Integer*)other)->number)
                )
        );
    }

    Value* add(Value* other) override {
        return new Integer(
                std::to_string(
                            std::stoi(this->number) + std::stoi(((Integer*)other)->number)
                        )
                );
    }

    Value* div(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stoi(this->number) / std::stoi(((Integer*)other)->number)
                )
        );
    }

    Value* sub(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stoi(this->number) - std::stoi(((Integer*)other)->number)
                )
        );
    }

    Value* mul(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stoi(this->number) * std::stoi(((Integer*)other)->number)
                )
        );
    }

    void set(Value* other) override {
        if (other->kind == V_FLOAT) {
            number = std::to_string(std::stoi(((Integer*)other)->number));
            return;
        }
        expect(other, V_INT);
        this->number = ((Integer*)other)->number;
    }

};

class Float : public Value {
public:
    std::string number;
    Float(std::string number) : Value(V_FLOAT) {
        this->number = number;
    }

    void set(Value* other) override {
        if (other->kind == V_INT) {
            number = std::to_string(std::stod(((Integer*)other)->number));
            return;
        }
        expect(other, V_FLOAT);
        this->number = ((Float*)other)->number;
    }

    Value* copy() override {
        return new Float(number);
    }

    std::string str() override {
        return number;
    }

    Value* bit_and(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stod(this->number) && std::stod(((Integer*)other)->number)
                )
        );
    }

    Value* is_eq(Value* other) override {
        return new Bool(this->number == ((Integer*)other)->number && this->kind == other->kind);
    }

    Value* not_eq_(Value* other) override {
        return this->is_eq(other)->cond_not();
    }

    Value* big(Value* other) override {
        return new Bool(std::stod(this->number) > std::stod(((Integer*)other)->number));
    }

    Value* less(Value* other) override {
        return new Bool(std::stod(this->number) < std::stod(((Integer*)other)->number));
    }

    Value* less_or_eq(Value* other) override {
        return new Bool(std::stod(this->number) <= std::stod(((Integer*)other)->number));
    }

    Value* big_or_eq(Value* other) override {
        return new Bool(std::stod(this->number) >= std::stod(((Integer*)other)->number));
    }

    Value* add(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stod(this->number) * std::stod(((Integer*)other)->number)
                )
        );
    }

    Value* div(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stod(this->number) / std::stod(((Integer*)other)->number)
                )
        );
    }

    Value* sub(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stod(this->number) - std::stod(((Integer*)other)->number)
                )
        );
    }

    Value* mul(Value* other) override {
        return new Integer(
                std::to_string(
                        std::stod(this->number) * std::stod(((Integer*)other)->number)
                )
        );
    }
};

class Array : public Value {
public:
    std::vector<Value*> elements;
    Array(std::vector<Value*> elements) : Value(V_ARRAY) {
        this->elements = elements;
    }

    Value* copy() override {
        return new Array(elements);
    }

    void set(Value* val) override {
        expect(val, V_ARRAY);
        this->elements = ((Array*)val)->elements;
    }

    std::string str() override {
        std::string s = "[";
        for (auto i : elements) s += i->str();
        s += "]";
        return s;
    }

    void element_set(Value* position, Value* value) override {
        int pos = std::stoi(((Integer*)position)->number);
        this->elements[pos] = value;
    }

    Value* element_get(Value* position) override {
        if (position->kind != ValueKind::V_INT) {
            std::cout << "Not a number\n";
            exit(-1);
        }
        int pos = std::stoi(((Integer*)position)->number);
        return elements[pos];
    }
};

class String : public Value {
public:
    std::string basicString;
    String(std::string str) : Value(V_STRING) {
        this->basicString = str;
    }

    Value* copy() override {
        return new String(basicString);
    }

    void set(Value* value) override {
        expect(value, V_STRING);
        this->basicString = ((String*)value)->basicString;
    }

    std::string str() override {
        return basicString;
    }

    void element_set(Value* position, Value* value) override {
        int pos = std::stoi(((Integer*)position)->number);
        this->basicString[pos] = ((String*)value)->basicString[0];
    }

    Value* element_get(Value* position) override {
        return new String(std::to_string(this->basicString[std::stoi(((Integer*)position)->number)]));
    }

    Value* add(Value* other) override {
        switch (other->kind) {
            case Value::V_STRING: return new String(this->basicString + ((String*)other)->basicString);
            case Value::V_FLOAT: case Value::V_INT: return new String(this->basicString + ((Integer*)other)->number);
            default: this->operator_not_supposed_err("+");
        }
    }
};

class Function : public Value {
public:
    enum FunctionKind {
        F_USER_DEFINE,
        F_BUILD_IN
    } fun_kind;

    std::string name;

    Function(FunctionKind k, std::string name) : Value(V_FUNC) {
        this->fun_kind = k;
    }
};

class UserDefineFunction : public Function {
public:
    std::vector<std::string> args;
    std::vector<AST*> body;
    UserDefineFunction(std::string name, std::vector<std::string> args, std::vector<AST*> body) : Function(F_USER_DEFINE, name) {
        this->args = args;
        this->body = body;
    }
};

class Interpreter;

using MF = Value*(Interpreter::*)(std::vector<Value*>);

class Context {
public:
    Context* parent_context;
    std::unordered_map<std::string, Value*> table;
    std::string display_name;

    Context(std::string display_name, Context* parent = nullptr){
        this->display_name = display_name;
        this->parent_context = parent;
    }

    Context* get_global() {
        if (parent_context)
            return parent_context->get_global();
        return this;
    }

    bool is_exist_in_this_level(std::string name) {
        return table.find(name) != table.end();
    }

    Value* get(std::string name) {
        if (!is_exist_in_this_level(name) && parent_context)
            return parent_context->get(name);
        ce(name);
        return table[name];
    }

    void ce(std::string name) {
        if (!is_exist_in_this_level(name)) {
            std::cout << "Name '" << name << "' is not define in scope '" << display_name << "'\n";
            exit(-1);
        }
    }

    void check(std::string name) {
        if (is_exist_in_this_level(name)) {
            std::cout << "Name '" << name << "' double define in scope '" << display_name << "'\n";
            exit(-1);
        }
    }

    void add(std::string name, Value* value) {
        check(name);
        table[name] = value;
    }

    void set(std::string name, Value* value) {
        if (!parent_context && !is_exist_in_this_level(name)) {
            std::cout << "Name '" << name << "' is not define\n";
            exit(-1);
        }
        if (is_exist_in_this_level(name)) {
            table[name]->set(value);
            return;
        }
        parent_context->set(name, value);
    }
};

class BasicObject : public Value {
public:
    std::unordered_map<std::string, Value*> members;
    BasicObject(std::unordered_map<std::string, Value*> members) : Value(V_OBJECT) {
        this->members = members;
    }

    BasicObject() : Value(V_OBJECT) { }

    bool is_exist(std::string name) { return members.find(name) != members.end(); }

    void add(std::string name, Value* value) {
        check(name);
        members[name] = value;
    }

    void check(std::string name) {
        if (is_exist(name)) {
            std::cout << "Name '" << name << "' is double define\n";
            exit(-1);
        }
    }

    Value* get(std::string name) {
        if (!is_exist(name)){
            std::cout << "Name '" << name << "' is double define\n";
            exit(-1);
        }
        return members[name];
    }

    void set(std::string name, Value* value) {
        get(name)->set(value);
    }
};

class BuildInFunctions : public Function {
public:
    MF member_function;
    std::vector<Value*> args;
    Interpreter* ip;
    BuildInFunctions(std::string name, MF mf) : Function(F_BUILD_IN, name) {
        this->member_function = mf;
    }

    Value* __call__(Interpreter* ip, std::vector<Value*> args) {
        this->ip = ip;
        this->args = args;
        return (ip->*member_function)(args);
    }
};

class Interpreter {
public:
    Interpreter(std::string fn_name, std::vector<AST*> opers, Context* context = nullptr) {
        this->opers = opers;
        this->global = (context)? context: new Context("<Program>");
        this->execute_result = new Null();
        setup_build_in_functions();
        execute_all();
    }

    Interpreter(std::string fn_name, Context* context = nullptr) {
        this->global = (context)? context: new Context("<Program>");
        this->execute_result = new Null();
        setup_build_in_functions();
    }

    Value* execute_result;
    Context* global;

    void execute_all() {
        for (auto i : opers) {
            auto tmp = visit_node(i);
            if (tmp->kind == Value::V_RT_RESULT) {
                auto t = (RTResult*) tmp;
                if (t->kind == RTResult::S_RETURN)
                    return;
            }
        }
    }

    void setup_build_in_functions() {
        global->add("Print", new BuildInFunctions("print", &Interpreter::system_print));
        global->add("Println", new BuildInFunctions("println", &Interpreter::system_println));
        global->add("StringToInt", new BuildInFunctions("StringToInt", &Interpreter::system_str_to_int));
        global->add("IntToString", new BuildInFunctions("system_int_to_str", &Interpreter::system_int_to_str));
        global->add("FloatToString", new BuildInFunctions("system_flo_to_str", &Interpreter::system_flo_to_str));
        global->add("StringToFloat" ,new BuildInFunctions("system_str_to_flo", &Interpreter::system_str_to_flo));
        global->add("Length", new BuildInFunctions("Length", &Interpreter::system_length));
        global->add("Input", new BuildInFunctions("Input", &Interpreter::system_input));
    }
private:
    std::vector<AST*> opers;

    void create_scope(std::string name) {
        global = new Context(name, global);
    }

    void leave_scope() {
        if (global->parent_context)
            global = global->parent_context;
    }

    Value* system_input(std::vector<Value*> args) {
        system_print(args);
        std::string s;
        std::getline(std::cin, s);
        return new String(s);
    }

    Value* system_length(std::vector<Value*> args) {
        if (args.size() != 1) {
            std::cout << "InterpreSystemBuildInFunction 'Length' Needs 1 value\n";
            exit(-1);
        }
        auto tmp = args[0];
        if (tmp->kind == Value::V_STRING) return new Integer(std::to_string(((String*)tmp)->basicString.size()));
        if (tmp->kind == Value::V_ARRAY) return new Integer(std::to_string(((Array*)tmp)->elements.size()));
        std::cout << "TypeError: need a string or array\n";
        exit(-1);
    }

    Value* system_print(std::vector<Value*> args) {
        for (auto i : args)
            std::cout << i->str();
        return new Null();
    }

    Value* system_println(std::vector<Value*> args) {
        system_print(args);
        printf("\n");
        return new Null();
    }

    Value* system_str_to_int(std::vector<Value*> args) {
        if (args.size() != 1) {
            std::cout << "InterpreSystemBuildInFunction 'Length' Needs 1 value\n";
            exit(-1);
        }
        return new Integer(((Integer*)args[0])->number);
    }

    Value* system_int_to_str(std::vector<Value*> args) {
        if (args.size() != 1) {
            std::cout << "InterpreSystemBuildInFunction 'Length' Needs 1 value\n";
            exit(-1);
        }
        return new Integer(((String*)args[0])->basicString);
    }

    Value* system_str_to_flo(std::vector<Value*> args) {
        if (args.size() != 1) {
            std::cout << "InterpreSystemBuildInFunction 'Length' Needs 1 value\n";
            exit(-1);
        }
        return new String(((Float*)args[0])->number);
    }

    Value* system_flo_to_str(std::vector<Value*> args) {
        if (args.size() != 1) {
            std::cout << "InterpreSystemBuildInFunction 'Length' Needs 1 value\n";
            exit(-1);
        }
        return new Float(((String*)args[0])->basicString);
    }

    Value* visit_node(AST* a) {
        switch (a->kind) {
            case AST::A_IF: return visit_if(a);
            case AST::A_BLOCK: return visit_block(a);
            case AST::A_ARRAY: case AST::A_STRING: case AST::A_INT: case AST::A_FLO: case AST::A_TRUE: case AST::A_FALSE: return visit_value(a);
            case AST::A_WHILE: return visit_while(a);
            case AST::A_FOR: return visit_for(a);
            case AST::A_CLASS: visit_class(a); break;
            case AST::A_RETURN: return visit_return(a);
            case AST::A_BREAK: return visit_break();
            case AST::A_CONTINUE: return visit_continue();
            case AST::A_BIN_OP: return visit_bin_op(a);
            case AST::A_BIT_NOT: return visit_bit_not(a);
            case AST::A_MEMBER_ACCESS: return visit_member_access(a);
            case AST::A_ID: return visit_member_access(a);
            case AST::A_ELEMENT_GET: return visit_element_get(a);
            case AST::A_CALL: return visit_call(a);
            case AST::A_NOT: return visit_not(a);
            case AST::A_SELF_INC: return visit_self_inc(a);
            case AST::A_SELF_DEC: return visit_self_dec(a);
            case AST::A_VAR_DEF: visit_var_define(a); break;
            case AST::A_FUNC_DEFINE: visit_function(a); break;
            case AST::A_SELF_OPERA: visit_self_opera(a); break;
        }
        return new Null();
    }

    Value* visit_self_opera(AST* a) {
        auto sp = (SelfOperator*) a;
        std::string op = sp->op;
        auto id = visit_member_access(sp->target);
        auto val = visit_value(sp->value);
        if (op == "+=") id->set(id->add(val));
        if (op == "-=") id->set(id->sub(val));
        if (op == "*=") id->set(id->mul(val));
        if (op == "/=") id->set(id->div(val));
        if (op == ">>=") id->set(id->right_move(val));
        if (op == "<<=") id->set(id->left_move(val));
        if (op == "%=") id->set(id->mod(val));
        if (op == "=") id->set(val->copy());
        return new Null();
    }

    Value* visit_var_define(AST* a) {
        auto n = (VarDefineNode*) a;
        std::string name = n->name;
        Value* init_val = (n->init_value)? visit_value(n->init_value): new Null();
        global->add(name, init_val);
        return new Null();
    }

    Value* visit_self_inc(AST* a) {
        auto tmp = (SelfIncNode*) a;
        auto addr = visit_member_access(tmp->id);
        if (tmp->ipre == pre) {
            auto res = addr->copy();
            addr->set(addr->add(new Float("1.0")));
            return res;
        }
        addr->set(addr->add(new Float("1.0")));
        return addr->copy();
    }

    Value *visit_self_dec(AST* a) {
        auto tmp = (SelfIncNode*) a;
        auto addr = visit_member_access(tmp->id);
        if (tmp->ipre == pre) {
            auto res = addr->copy();
            addr->set(addr->add(new Float("1.0")));
            return res;
        }
        addr->set(addr->sub(new Float("1.0")));
        return addr->copy();
    }

    Value* visit_block(AST* a) {
        auto bn = (Block*) a;
        for (auto i : bn->codes) {
            auto _res = visit_node(i);
            if (_res->kind == Value::V_RT_RESULT) {
                auto res = (RTResult*) _res;
                if (res->kind == RTResult::S_BREAK || res->kind == RTResult::S_CONTINUE ||
                    res->kind == RTResult::S_RETURN)
                    return res;
            }
        }
        return new RTResult(RTResult::S_NONE);
    }

    Value* visit_return(AST* a) {
        auto ret = (ReturnNode*)a ;
        this->execute_result = visit_value(ret->value);
        return new RTResult(RTResult::S_RETURN);
    }

    Value *visit_break() {
        return new RTResult(RTResult::S_BREAK);
    }

    Value* visit_continue() {
        return new RTResult(RTResult::S_CONTINUE);
    }

    Value *visit_bit_not(AST* a) {
        return visit_value(a)->bit_not();
    }

    Value* visit_not(AST* a) {
        return visit_value(a)->cond_not();
    }

    Value* visit_element_get(AST* a) {
        auto tmp = (ElementGetNode*) a;
        auto id  = visit_member_access(tmp->array_name);
        auto pos = visit_value(tmp->position);
        return id->element_get(pos);
    }

    Value* visit_call(AST* a) {
        auto call_node = (CallNode*) a;
        auto fn_id = call_node->func_name;
        std::vector<Value*> args;
        for (auto i : call_node->args) args.push_back(visit_value(i));
        auto body = (Function*)visit_member_access(fn_id);
        std::string name = (fn_id->kind == AST::A_MEMBER_ACCESS)? ((MemberAccessNode*)fn_id)->member : ((IdNode*)fn_id)->id;
        auto c = new Context(name ,global->get_global());
        if (fn_id->kind == AST::A_MEMBER_ACCESS) {
            auto object_point = visit_member_access(((MemberAccessNode *) fn_id)->parent);
            c->add("this", object_point);
        }
        if (body->fun_kind == Function::F_USER_DEFINE) {
            auto temp = (UserDefineFunction*) body;
            auto args_t = temp->args;
            if (args.size() != args_t.size()) {
                std::cout << "Function '" << name << "' need " << args_t.size() << " values\n";
                throw std::exception();
            }
            for (int i = 0; i < args_t.size(); ++i)
                c->add(args_t[i], args[i]);
            auto interpreter = new Interpreter(name, temp->body, c);
            return interpreter->execute_result;
        } else if (body->fun_kind == Function::F_BUILD_IN) {
            auto temp = (BuildInFunctions*) body;
            auto nip = new Interpreter(name, new Context(name, global));
            return temp->__call__(nip, args);
        }
    }

    Value* visit_if(AST* a) {
        create_scope("<If>");
        auto in = (IfNode*) a;
        if (((Bool*) visit_value(in->condition))->b) {
            auto tmp = visit_block(in->if_true);
            leave_scope();
            return tmp;
        } else if (in->if_false) {
            auto tmp = visit_block(in->if_false);
            leave_scope();
            return tmp;
        }
        leave_scope();
        return new RTResult(RTResult::S_NONE);
    }

    Value* visit_function(AST* a) {
        auto fnode = (FunctionNode*) a;
        std::vector<std::string> args;
        for (auto i : fnode->args) args.push_back(((VarDefineNode*)i)->name);
        global->add(fnode->name, new UserDefineFunction(fnode->name, args, ((Block*)fnode->body)->codes));
        return new Null();
    }

    Value* visit_for(AST* a) {
        create_scope("<For-Loop-Condition>");
        auto for_node = (ForNode*) a;
        auto init = for_node->init;
        visit_var_define(init);
        auto is_loop = (Bool*)visit_value(for_node->is_continue);
        auto change = for_node->change;
        auto body = ((Block*)for_node->body);
        while (is_loop->b) {
            create_scope("<For-Loop-Frame>");
            auto res = visit_block(body);
            leave_scope();
            if (res->kind == Value::V_RT_RESULT) {
                auto tmp = (RTResult*) res;
                if (tmp->kind == RTResult::S_CONTINUE) continue;
                if (tmp->kind == RTResult::S_BREAK) break;
                if (tmp->kind == RTResult::S_RETURN) return tmp;
            }
            visit_node(change);
            is_loop = (Bool*) visit_value(for_node->is_continue);
        }
        leave_scope();
        return new RTResult(RTResult::S_NONE);
    }

    void reset_scope() {
        std::string name = global->display_name;
        leave_scope();
        create_scope(name);
    }

    Value* visit_while(AST* a) {
        create_scope("<While-Loop-Condition>");
        auto for_node = (WhileNode*) a;
        auto is_loop = (Bool*)visit_value(for_node->condition);
        auto body = ((Block*)for_node->body);
        while (is_loop->b) {
            create_scope("<While-Loop-Frame>");
            auto res = visit_block(body);
            leave_scope();
            if (res->kind == Value::V_RT_RESULT) {
                auto tmp = (RTResult*) res;
                if (tmp->kind == RTResult::S_NONE || tmp->kind == RTResult::S_CONTINUE) continue;
                if (tmp->kind == RTResult::S_BREAK) break;
                if (tmp->kind == RTResult::S_RETURN) return tmp;
            }
            is_loop = (Bool*) visit_call(for_node->condition);
        }
        leave_scope();
        return new RTResult(RTResult::S_NONE);
    }

    void visit_class(AST* a) {

    }

    Value* visit_member_access(AST* a) {
        if (a->kind == AST::A_ELEMENT_GET)
            return visit_element_get(a);
        if (a->kind == AST::A_CALL)
            return visit_call(a);
        if (a->kind == AST::A_ARRAY)
            return visit_array(a);
        if (a->kind == AST::A_STRING || a->kind == AST::A_INT || a->kind == AST::A_FLO)
            return visit_value(a);
        if (a->kind == AST::A_ID)
            return global->get(((IdNode*)a)->id);
        return ((BasicObject*)visit_member_access(((MemberAccessNode*)a)->parent))->get(((MemberAccessNode*)a)->member);
    }

    Value* visit_array(AST* a) {
        auto arr = (ArrayNode*) a;
        std::vector<Value*> tmp;
        for (auto i : arr->elements)
            tmp.push_back(visit_value(i));
        return new Array(tmp);
    }

    Value* visit_value(AST* a) {
        if (a->kind == AST::A_BIN_OP)
            return visit_bin_op(a);
        if (a->kind == AST::A_STRING)
            return new String(((StringNode*)a)->str);
        if (a->kind == AST::A_ID || a->kind == AST::A_MEMBER_ACCESS)
            return visit_member_access(a);
        if (a->kind == AST::A_FALSE)
            return new Bool(false);
        if (a->kind == AST::A_CALL)
            return visit_call(a);
        if (a->kind == AST::A_TRUE)
            return new Bool(true);
        if (a->kind == AST::A_ELEMENT_GET)
            return visit_element_get(a);
        if (a->kind == AST::A_INT)
            return new Integer(((IntegerNode*)a)->number);
        if (a->kind == AST::A_FLO)
            return new Float(((FloatNode*)a)->number);
        if (a->kind == AST::A_ARRAY) {
            std::vector<Value*> values;
            for (auto i : ((ArrayNode*)a)->elements)
                values.push_back(visit_bin_op(i));
            return new Array(values);
        } else {
            std::cout << "Unknown tree: " << a->kind << "\n";
            exit(-1);
        }
    }

    Value* visit_bin_op(AST* a) {
        if (a->kind != AST::A_BIN_OP)
            return visit_value(a);
        auto left = visit_bin_op(((BinOpNode*)a)->left);
        auto right = visit_bin_op(((BinOpNode*)a)->right);
        auto op = ((BinOpNode*)a)->op;
        // ======= Comp
        if (op == "==") return left->is_eq(right);
        if (op == ">=") return left->big_or_eq(right);
        if (op == "<=") return left->less_or_eq(right);
        if (op == ">") return left->big(right);
        if (op == "<") return left->less(right);
        // ======= Calc
        if (op == "+") return left->add(right);
        if (op == "-") return left->sub(right);
        if (op == "*") return left->mul(right);
        if (op == "/") return left->div(right);
        if (op == "%") return left->mod(right);
        if (op == "&") return left->bit_and(right);
        if (op == "|") return left->bit_or(right);
        if (op == "<<") return left->left_move(right);
        if (op == ">>") return left->right_move(right);
        // ======= Condition
        if (op == "&&") return left->cond_and(right);
        if (op == "||") return left->cond_or(right);
        std::cout << "Unknown operator '" << op << "' when the program running\n";
        exit(-1);
    }
};

#endif