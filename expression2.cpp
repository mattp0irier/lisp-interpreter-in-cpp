#include <iostream>
#include <vector>
#include <string>
#include "token.cpp"
#include "boost/any.hpp"

using namespace std;

class Visitor {
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

};

class AssignVisitor: public Visitor {
    public:
    boost::any visitAssignExpr(Assign expr){

    }
};

class BinaryVisitor: public Visitor {
    public:
    boost::any visitBinaryExpr(Binary expr){
        
    }
};

class CallVisitor: public Visitor {
    public:
    boost::any visitCallExpr(Call expr){
        
    }
};

class GetVisitor: public Visitor {
    public:
    boost::any visitGetExpr(Get expr){
        
    }
};

class GroupingVisitor: public Visitor {
    public:
    boost::any visitGroupingExpr(Grouping expr){
        
    }
};

class LiteralVisitor: public Visitor {
    public:
    boost::any visitLiteralExpr(Literal expr){
        
    }
};

class LogicalVisitor: public Visitor {
    public:
    boost::any visitLogicalExpr(Logical expr){
        
    }
};

class SetVisitor: public Visitor {
    public:
    boost::any visitSetExpr(Set expr){
        
    }
};

class ThisVisitor: public Visitor {
    public:
    boost::any visitThisExpr(This expr){
        
    }
};

class UnaryVisitor: public Visitor {
    public:
    boost::any visitUnaryExpr(Unary expr){
        
    }
};

class VariableVisitor: public Visitor {
    public:
    boost::any visitVariableExpr(Variable expr){
        
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

    boost::any accept(AssignVisitor visitor){
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
    boost::any accept(BinaryVisitor visitor){
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
    boost::any accept(AssignVisitor visitor){
        return visitor.visitAssignExpr(*this);
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
};

  
class Grouping: public Expr {
    public:
        Grouping(Expr expression){
            expression = expression;
        }

     
        Expr expression;
};


class Literal: public Expr {
    public:
        Literal(boost::any value){
            value = value;
        }

     
        boost::any value;
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