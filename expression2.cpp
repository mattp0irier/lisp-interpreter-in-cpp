#include <iostream>
#include <vector>
#include <string>
#include "token.cpp"

using namespace std;


template <typename T>
class Expr {
    public:
    /*
    these might not actually be necessary??
    virtual T visitAssignExpr(Assign expr);
    virtual T visitBinaryExpr(Binary expr);
    virtual T visitCallExpr(Call expr);
    virtual T visitGetExpr(Get expr);
    virtual T visitGroupingExpr(Grouping expr);
    virtual T visitLiteralExpr(Literal expr);
    virtual T visitLogicalExpr(Logical expr);
    virtual T visitSetExpr(Set expr);
    virtual T visitThisExpr(This expr);
    virtual T visitUnaryExpr(Unary expr);
    virtual T visitVariableExpr(Variable expr);

    virtual T accept();  
    */      
};

template <typename T>
class Assign: public Expr {
    public:
        Assign(Token name, Expr value){
            name = name;
            value = value;
        }

        Token name final;
        Expr value final;
};

template <typename T>
class Binary: public Expr {
    public:
        Binary(Expr left, Token op, Expr right){
            left = left;
            op = op;
            right = right;
        }

     
        Expr left final;
        Token op final;
        Expr right final;
};

template <typename T>
class Call: public Expr {
    public:
        Call(Expr callee, Token paren, vector<Expr> args){
            callee = callee;
            paren = paren;
            args = args;
        }

     
        Expr callee final;
        Token paren final;
        vector<Expr> final;
};

template <typename T>
class Get: public Expr {
    public:
        Get(Expr obj, Token name){
            obj = obj;
            name = name;
        }

     
        Expr obj final;
        Token name final;
};

template <typename T>
class Grouping: public Expr {
    public:
        Grouping(Expr expression){
            expression = expression;
        }

     
        Expr expression final;
};

template <typename T>
class Literal: public Expr {
    public:
        Literal(T value){
            value = value;
        }

     
        T value final;
};

template <typename T>
class Logical: public Expr {
    public:
        Logical(Expr left, Token op, Expr right){
            left = left;
            op = op;
            right = right;
        }

     
        Expr left final;
        Token op final;
        Expr right final;
};

template <typename T>
class Set: public Expr {
    public:
        Set(Expr object, Token name, Expr value){
            object = object;
            name = name;
            value = value;
        }

     
        Expr object final;
        Token name final;
        Expr value final;
};

template <typename T>
class This: public Expr {
    public:
        This(Token keyword){
            keyword = keyword;
        }

     
        Token keyword final;
};

template <typename T>
class Unary: public Expr {
    public:
        Unary(Token op, Expr right){
            op = op;
            right = right;
        }

     
        Token op final;
        Expr right final;
};

template <typename T>
class Variable: public Expr {
    public:
        Variable(Token name){
            name = name;
        }

     
        Token name final;
};