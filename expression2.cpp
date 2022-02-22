#include <iostream>
#include <vector>
#include <string>
#include "token.cpp"

using namespace std;


  
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

  
class Assign: public Expr {
    public:
        Assign(Token name, Expr value){
            name = name;
            value = value;
        }

        Token name;
        Expr value;
};

  
class Binary: public Expr {
    public:
        Binary(Expr left, Token op, Expr right){
            left = left;
            op = op;
            right = right;
        }

     
        Expr left;
        Token op;
        Expr right;
};

  
class Call: public Expr {
    public:
        Call(Expr callee, Token paren, vector<Expr> args){
            callee = callee;
            paren = paren;
            args = args;
        }

     
        Expr callee;
        Token paren;
        vector<Expr> args;
};

  
class Get: public Expr {
    public:
        Get(Expr obj, Token name){
            obj = obj;
            name = name;
        }

     
        Expr obj;
        Token name;
};

  
class Grouping: public Expr {
    public:
        Grouping(Expr expression){
            expression = expression;
        }

     
        Expr expression;
};

template <typename T>
class Literal: public Expr {
    public:
        Literal(T value){
            value = value;
        }

     
        T value;
};

  
class Logical: public Expr {
    public:
        Logical(Expr left, Token op, Expr right){
            left = left;
            op = op;
            right = right;
        }

     
        Expr left;
        Token op;
        Expr right;
};

  
class Set: public Expr {
    public:
        Set(Expr object, Token name, Expr value){
            object = object;
            name = name;
            value = value;
        }

     
        Expr object;
        Token name;
        Expr value;
};

  
class This: public Expr {
    public:
        This(Token keyword){
            keyword = keyword;
        }

     
        Token keyword;
};

  
class Unary: public Expr {
    public:
        Unary(Token op, Expr right){
            op = op;
            right = right;
        }

     
        Token op;
        Expr right;
};

  
class Variable: public Expr {
    public:
        Variable(Token name){
            name = name;
        }

     
        Token name;
};