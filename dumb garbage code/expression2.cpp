#include <iostream>
#include <vector>
#include <string>
#include "token.cpp"
#include "any.hpp"

using namespace std;

#ifndef _EXPRCLASS_
#define _EXPRCLASS_

class Visitor;
class Interpreter;
class Expr;
class Assign;
class Binary;
class Call;
class Get;
class Grouping;
class Literal;
class Logical;
class Set;
class This;
class Unary;
class Variable;


class Visitor {
    public:
    virtual boost::any visitAssignExpr(Assign expr);
    virtual boost::any visitBinaryExpr(Binary expr);
    virtual boost::any visitCallExpr(Call expr);
    virtual boost::any visitGetExpr(Get expr);
    virtual boost::any visitGroupingExpr(Grouping expr);
    virtual boost::any visitLiteralExpr(Literal expr);
    virtual boost::any visitLogicalExpr(Logical expr);
    virtual boost::any visitSetExpr(Set expr);
    virtual boost::any visitThisExpr(This expr);
    virtual boost::any visitUnaryExpr(Unary expr);
    virtual boost::any visitVariableExpr(Variable expr);

    boost::any evaluate(Expr expr){
        return expr.accept(*this);
    }
};

class Interpreter: public Visitor {
    public:
    boost::any visitAssignExpr(Assign expr){
        return NULL;
    }

    boost::any visitBinaryExpr(Binary expr){
        boost::any left = evaluate(expr.left);
        boost::any right = evaluate(expr.right);

        switch(expr.op.getType()) {
            case MINUS:
                return boost::any_cast<double>(left) - boost::any_cast<double>(right);
                break;
            case MULTIPLY:
                return boost::any_cast<double>(left) * boost::any_cast<double>(right);
                break;
            case DIVIDE:
                return boost::any_cast<double>(left) / boost::any_cast<double>(right);
                break;
            case PLUS:
                return boost::any_cast<double>(left) + boost::any_cast<double>(right);
                break;
            default:
                cout << "This should be unreachable" << endl;
                return NULL;
        }
    }

    boost::any visitCallExpr(Call expr){
        return NULL;
    }

    boost::any visitGetExpr(Get expr){
        return NULL;
    }

    boost::any visitGroupingExpr(Grouping expr){
        return evaluate(expr.expression);
    }

    boost::any visitLiteralExpr(Literal expr){
        return expr.value;
    }

    boost::any visitLogicalExpr(Logical expr){
        return NULL;
    }

    boost::any visitSetExpr(Set expr){
        return NULL;
    }

    boost::any visitThisExpr(This expr){
        return NULL;
    }

    boost::any visitUnaryExpr(Unary expr){
        boost::any right = evaluate(expr.right);

        switch(expr.op.getType()) {
            case MINUS:
                return -1 * boost::any_cast<double>(right);
        }
        return NULL;
    }

    boost::any visitVariableExpr(Variable expr){
        return NULL;
    }

};


class Expr {
    public:

    virtual boost::any accept(Visitor visitor);  
    
};

  
class Assign: public Expr {
    public:
        Assign(Token name, Expr value){
            name = name;
            value = value;
        }

        Token name;
        Expr value;

    boost::any accept(Interpreter visitor){
        return visitor.visitAssignExpr(*this);
    }
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
    boost::any accept(Interpreter visitor){
        return visitor.visitBinaryExpr(*this);
    }
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
    boost::any accept(Interpreter visitor){
        return visitor.visitCallExpr(*this);
    }
};

  
class Get: public Expr {
    public:
        Get(Expr obj, Token name){
            obj = obj;
            name = name;
        }

     
        Expr obj;
        Token name;
    boost::any accept(Interpreter visitor){
        return visitor.visitGetExpr(*this);
    }
};

  
class Grouping: public Expr {
    public:
        Grouping(Expr expression){
            expression = expression;
        }

     
        Expr expression;
    boost::any accept(Interpreter visitor){
        return visitor.visitGroupingExpr(*this);
    }
};


class Literal: public Expr {
    public:
        Literal(boost::any value){
            value = value;
        }

     
        boost::any value;
    boost::any accept(Interpreter visitor){
        return visitor.visitLiteralExpr(*this);
    }
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
    boost::any accept(Interpreter visitor){
        return visitor.visitLogicalExpr(*this);
    }
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
    boost::any accept(Interpreter visitor){
        return visitor.visitSetExpr(*this);
    }
};

  
class This: public Expr {
    public:
        This(Token keyword){
            keyword = keyword;
        }

     
        Token keyword;
    boost::any accept(Interpreter visitor){
        return visitor.visitThisExpr(*this);
    }
};

  
class Unary: public Expr {
    public:
        Unary(Token op, Expr right){
            op = op;
            right = right;
        }

     
        Token op;
        Expr right;
    boost::any accept(Interpreter visitor){
        return visitor.visitUnaryExpr(*this);
    }
};

  
class Variable: public Expr {
    public:
        Variable(Token name){
            name = name;
        }
     
        Token name;
    boost::any accept(Interpreter visitor){
        return visitor.visitVariableExpr(*this);
    }
};


#endif