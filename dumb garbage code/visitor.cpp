#include "any.hpp"
#include "expression2.cpp"

#ifndef _VISCLASS_
#define _VISCLASS_

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
        
    }

    boost::any visitGetExpr(Get expr){
        
    }

    boost::any visitGroupingExpr(Grouping expr){
        return evaluate(expr.expression);
    }

    boost::any visitLiteralExpr(Literal expr){
        return expr.value;
    }

    boost::any visitLogicalExpr(Logical expr){
        
    }

    boost::any visitSetExpr(Set expr){
        
    }

    boost::any visitThisExpr(This expr){
        
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
        
    }

};
#endif