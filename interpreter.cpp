#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <string.h>
#include "token.cpp"
#include "expression3.cpp"

using namespace std;

ENV *globalEnv = emptyEnv();

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return is_base_of<Base, T>::value;
}

class Interpreter {
    private:
        // SEXP applyUserFun(String nm, VALUELIST actuals) {
        //     FUNDEF fun = fetchFun(nm);
        //     if (fun == null) {
        //         ERROR("Undefined function: " + nm);
        //         return nil;
        //     }
        //     if (lengthNL(fun.formals) != lengthVL(actuals)) {
        //         ERROR("Wrong number of arguments to: " + nm);
        //         return nil;
        //     }
        //     ENV rho = new ENV(fun.formals, actuals);
        //     return eval(fun.body, rho);
        // }
        S_EXP *nil = new S_EXP("()");
        S_EXP *TRUE = new S_EXP("TRUE");

        VALUELIST *evalList(EXPLIST *el, ENV *rho) {
            if (el == NULL)
                return NULL;
            S_EXP *h = eval(el->head, rho);
            VALUELIST *t = evalList(el->tail, rho);
            return new VALUELIST(h, t);
        }

        S_EXP *applyCtrlOp(Token controlOP, EXPLIST *args, ENV *rho) {
            S_EXP *s = nil;
            string varble;
            switch (controlOP.getType()) {
                case IF:
                    if (isTrueVal(eval(args->head, rho)))
                        return eval(args->tail->head, rho);
                    else
                        return eval(args->tail->tail->head, rho);
                    break;
                case WHILE:
                    s = eval(args->head, rho);
                    while (s->toString() != "()") {
                        s = eval(args->tail->head, rho);
                        s = eval(args->head, rho);
                    }
                    return s;
                    break;
                case SET:
                    s = eval(args->tail->head, rho);
                    varble = ((VAREXP *)(args->head))->varble;
                    if (isBound(varble, rho))
                        assign(varble, s, rho);
                    else if (isBound(varble, globalEnv))
                        assign(varble, s, globalEnv);
                    else
                        bindVar(varble, s, globalEnv);
                    return s;
                    break;
                case BEGIN: 
                    while (args->tail != NULL) {
                        s = eval(args->head, rho);
                        args = args->tail;
                    }
                    // very strange this
                    s = eval(args->head, rho);
                    return s;
                    break;
            }
            return s;
        }

        NUM_SXP *applyArithOp(Token op, int n1, int n2) {
            int result = 0;
            switch (op.getType()) {
                case PLUS:
                    result = n1 + n2;
                    break;
                case MINUS:
                    result = n1 - n2;
                    break;
                case MULTIPLY:
                    result = n1 * n2;
                    break;
                case DIVIDE:
                    result = n1 / n2;
                    break;
                // case '%':
                //     result = n1 % n2;
                //     break;
                default:
                    break;
            }
            return new NUM_SXP(result);
        }
            
        S_EXP *applyRelOp(Token op, int n1, int n2) {
            bool result;
            switch (op.getVal()[0]) {
                case '<':
                    if (op.getVal().length() > 1 && op.getVal()[1] == '=') result = n1 <= n2;
                    else result = n1 < n2;
                    break;
                case '>':
                    if (op.getVal().length() > 1 && op.getVal()[1] == '=') result = n1 >= n2;
                    else result = n1 > n2; 
                    break;;
                case '=':
                    if (op.getVal().length() > 1 && op.getVal()[1] == '=') result = n1 == n2;
                    else result = nil;
                    break;
                default:
                    result = nil;
                    break;
            }
            if (result)
                return new S_EXP("TRUE");
            else
                return nil;
        }

        S_EXP *applyValueOp(Token op, VALUELIST *vl) {
            S_EXP *result = nil;
            S_EXP *s1 = nil;
            S_EXP *s2 = nil;
            if (op.getArity() != 0 && op.getArity() != lengthVL(vl)) {
                ERROR("Wrong number of arguments to " + op.getVal() + " expected " + to_string(op.getArity()) + " but found " + to_string(lengthVL(vl)));
                return nil;
            }
            s1 = vl->head; // 1st actual
            if (op.getArity() == 2)
                s2 = vl->tail->head; // 2nd actual
                if (op.getType() == PLUS || op.getType() == MINUS || op.getType() == MULTIPLY || op.getType() == DIVIDE ||
                    op.getType() == EQUAL || op.getType() == LESS_THAN || op.getType() == GREATER_THAN) {
                    if (s1->type == "Number" && s2->type == "Number") {
                        NUM_SXP *n1 = (NUM_SXP*) s1;
                        NUM_SXP *n2 = (NUM_SXP*) s2;

                        if (op.getType() == PLUS || op.getType() == MINUS || op.getType() == MULTIPLY || op.getType() == DIVIDE) {
                            result = applyArithOp(op, n1->intVal, n2->intVal);
                        }
                        else
                            result = applyRelOp(op, n1->intVal, n2->intVal);
                    }
                    else {
                        ERROR("Non-arithmatic arguments to " + op.getVal());
                    }
            }
            else if (op.getArity() == 2) {
                result = apply(op, s1, s2);
            }
            else {
                result = apply(op, s1);
            }
            return result;
        }

        S_EXP *apply(Token op, S_EXP* s1, S_EXP* s2) {
            S_EXP *result = nil;
            string opValue = op.getVal();
                if (opValue == "CONS")
                    result = new LIST_SXP(s1, s2);
                else if (opValue == "EQ?" || opValue == "=") {
                    if (s1 == nil && s2 == nil) {
                        result = TRUE;
                    } else if (s1->type == "Number" && s2->type == "Number") {
                        NUM_SXP *n1 = (NUM_SXP *)s1;
                        NUM_SXP *n2 = (NUM_SXP *)s2;
                        if (n1->intVal == n2->intVal)
                            result = TRUE;
                    } else if (s1->type == "Symbol" && s2->type == "Symbol") {
                        SYM_SXP *n1 = (SYM_SXP*)s1;
                        SYM_SXP *n2 = (SYM_SXP*)s2;
                        if (n1->symVal == n2->symVal)
                            result = TRUE;
                    }
                }
            return result;
        }

        S_EXP *apply(Token op, S_EXP *s1) {
            S_EXP *result = nil;
            string opVal = op.getVal();

            if (opVal == "NOT") {
                if (s1 == nil)
                    result = TRUE;
            }
            else if (opVal == "CAR"){
                if (s1->type == "List") {
                    LIST_SXP *concell = (LIST_SXP *)s1;
                    result = concell->carVal;
                }
                else
                    ERROR("car applied to non-list");
            }
            else if (opVal == "CDR"){
                if (s1->type == "List") {
                    LIST_SXP *concell = (LIST_SXP *)s1;
                    result = concell->cdrVal;
                }
                else
                    ERROR("cdr applied to non-list");
            }
            else if (opVal == "NIL?" || opVal == "NULL?") {
                if (s1 == nil)
                    result = TRUE;
            }
            else if (opVal == "NUMBER?"){
                if (s1->type == "Number")
                    result = TRUE;
            }
            else if (opVal == "SYMBOL?"){
                if (s1->type == "Symbol")
                    result = TRUE;
            }
            else if (opVal == "LIST?"){
                if (s1->type == "List")
                    result = TRUE;
            }
            else if (opVal == "PRINT"){
                cout << s1->toString() << endl;
                result = s1;
            }

            return result;
        }

    public:
        Interpreter() {

        }

        S_EXP *eval(EXP *expression, ENV *rho) {
            Token op;
            if (expression->name == "valexp") {
                VALEXP* exp = (VALEXP*)expression;
                return exp->sxp;
            }
            else if (expression->name == "varexp") {
                VAREXP* exp = (VAREXP*)expression;
                if (isBound(exp->varble, rho)) {
                    return fetch(exp->varble, rho);
                }
                if (isBound(exp->varble, globalEnv)) {
                    return fetch(exp->varble, globalEnv);
                }
                // ERROR("Undefined variable " + v.varble);
            }
            else if (expression->name == "apexp") {
                APEXP* exp = (APEXP*)expression;
                op = exp->op;
                if (op.getType() == IDENTIFIER) {
                    cout << "USER FUNCTION" << endl;
                    return nil;
                   // return applyUserFun(op.getVal(), evalList(exp->args, rho));
                }
                else {
                    if (op.getType() == IF || op.getType() == SET || op.getType() == WHILE || op.getType() == BEGIN) {
                        return applyCtrlOp(op, exp->args, rho);
                    }
                    else {
                        return applyValueOp(op, evalList(exp->args, rho));
                    }
                }
            }
            cout << "nil" << endl;
            return nil;
        }

        bool isTrueVal (S_EXP *s_exp) {
            return !(s_exp->toString() == "()");
        }

        bool isBound (string name, ENV *rho){
            return (findVar(name, rho) != NULL);
        }
};