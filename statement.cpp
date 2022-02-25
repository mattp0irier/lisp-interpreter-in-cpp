#include <iostream>
#include <vector>
#include <string>
#include "token.cpp"
#include "expression2.cpp"
#include "boost/any.hpp"

using namespace std;

class Stmt {
    public:
        virtual boost::any acceptIf(If visitor);
        virtual boost::any acceptWhile(While visitor);
        virtual boost::any acceptSet(Set visitor);
        virtual boost::any acceptBegin(Begin visitor);
        virtual boost::any acceptCons(Cons visitor);
        virtual boost::any acceptCar(Car visitor);
        virtual boost::any acceptCdr(Cdr visitor);
        virtual boost::any acceptExpression(Expression visitor);
        virtual boost::any acceptIsNum(IsNum visitor);
        virtual boost::any acceptIsSym(IsSym visitor);
        virtual boost::any acceptIsList(IsList visitor);
        virtual boost::any acceptIsNull(IsNull visitor);
        virtual boost::any acceptPrint(Print visitor);
};

// class Expression extends Stmt {
//     Expression(Expr visitor) {
//         this.expression = visitor;
//     }

//     virtual boost::any acceptExpression(Expression visitor) {
//         return visitor.visitExpressionStmt(this);
//     }

//     Expr expression;
// };

class If: public Stmt {
    If(Expr condition, Stmt trueBranch, Stmt falseBranch) {
        this->condition = condition;
        this->trueBranch = trueBranch;
        this->falseBranch = falseBranch;
    }
    virtual boost::any acceptIf(IfVisitor visitor) {
        return visitor.visitIfStmt(*this);
    }

    Expr condition;
    Stmt trueBranch;
    Stmt falseBranch;
};

class While: public Stmt {
    While(Expr condition, Stmt body) {
        this->condition = condition;
        this->body = body;
    }

    virtual boost::any acceptWhile(WhileVisitor visitor) {
        return visitor.visitWhileStmt(*this);
    }

    Expr condition;
    Stmt body;
};

class Set: public Stmt {
    Set(Token name, Expr initializer) {
        this->name = name;
        this->initializer = initializer;
    }

    virtual boost::any acceptSet(SetVisitor visitor) {
        return visitor.visitSetStmt(*this);
    }

    Token name;
    Expr initializer;
};

//FIX
class Begin: public Stmt {
    Begin()  {
    }

    virtual boost::any acceptBegin(BeginVisitor visitor) {
        return visitor.visitBeginStmt(*this);
    }

};

class Cons: public Stmt {
    Cons(Expr expr1, Expr expr2)  {
        this->expr1 = expr1;
        this->expr2 = expr2;
    }

    virtual boost::any acceptCons(ConsVisitor visitor) {
        return visitor.visitConsStmt(*this);
    }
    
    Expr expr1;
    Expr expr2;
};

class Car: public Stmt {
    Car(Expr expression)  {
        this->expression = expression;
    }

    virtual boost::any acceptCar(CarVisitor visitor) {
        return visitor.visitCarStmt(*this);
    }

    Expr expression;
};

class Cdr: public Stmt {
    Cdr(Expr expression)  {
        this->expression = expression;
    }

    virtual boost::any acceptCdr(CdrVisitor visitor) {
        return visitor.visitCdrStmt(*this);
    }

    Expr expression;
};

class IsNum: public Stmt {
    IsNum(Expr expression)  {
        this->expression = expression;
    }

    virtual boost::any acceptIsNum(IsNumVisitor visitor) {
        return visitor.visitIsNumStmt(*this);
    }

    Expr expression;
};

class IsSym: public Stmt {
    IsSym(Expr expression)  {
        this->expression = expression;
    }

    virtual boost::any acceptIsSym(IsSymVisitor visitor) {
        return visitor.visitIsSymStmt(*this);
    }

    Expr expression;
};

class IsList: public Stmt {
    IsList(Expr expression)  {
        this->expression = expression;
    }

    virtual boost::any acceptIsList(IsListVisitor visitor) {
        return visitor.visitIsListStmt(*this);
    }

    Expr expression;
};

class IsNull: public Stmt {
    IsNull(Expr expression)  {
        this->expression = expression;
    }

    virtual boost::any acceptIsNull(IsNullVisitor visitor) {
        return visitor.visitIsNullStmt(*this);
    }

    Expr expression;
};

class Print: public Stmt
{
    Print(Expr expression) {
        this->expression = expression;
    }

    virtual boost::any acceptPrint(PrintVisitor visitor) {
        return visitor.visitPrintStmt(*this);
    }

    Expr expression;
};