#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <string.h>
#include "token.hpp"
#include "expression3.hpp"
#include "parser2.hpp"

using namespace std;

#ifndef _INTERPRETER_
#define _INTERPRETER_

ENV *globalEnv = emptyEnv();

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return is_base_of<Base, T>::value;
}

class Interpreter {
    private:
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
                    if (isTrueVal(eval(args->head, rho))){
                        return eval(args->tail->head, rho);
                    }
                    else{
                        return eval(args->tail->tail->head, rho);
                    }
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
                    varble = ((VAREXP *)(args->head))->varble;
                    if (args->tail->head->name == "valexp") s = eval(args->tail->head, rho);
                    else if (args->tail->head->name == "strexp") s = eval(args->tail->head, rho);
                    else if (args->tail->head->name == "varexp"){
                        if (isBound(((VAREXP *)(args->tail->head))->varble, rho)){
                            s = eval(args->tail->head, rho);
                        }
                        else if (isBound(((VAREXP *)(args->tail->head))->varble, globalEnv)){
                            s = eval(args->tail->head, globalEnv);
                        }
                        else s = new SYM_SXP(((VAREXP *)(args->tail->head))->varble);
                    }
                    else if (args->tail->head->name == "apexp") {
                        s = eval(args->tail->head, rho);
                    }
                    else {
                        cout << "please don't be here" << endl;
                    }
                    if (isBound(varble, rho))
                        assign(varble, s, rho);
                    else if (isBound(varble, globalEnv))
                        assign(varble, s, globalEnv);
                    else
                        bindVar(varble, s, globalEnv);
                    return s;
                    break;
                case BEGIN:
                    while (args != NULL) {
                        s = eval(args->head, rho);
                        cout << "output: " << s->toString() << endl;
                        args = args->tail;
                    }
                    return s;
                    break;
                default:
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
                case MOD:
                    result = n1 % n2;
                    break;
                default:
                    break;
            }
            return new NUM_SXP(result);
        }
            
        NUM_SXP *applyArithOp(Token op, double n1, double n2) {
            double result = 0;
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
                case MOD:
                    ERROR("Cannot perform modular division on floating point");
                    result = -1.0;
                    break;
                default:
                    break;
            }
            return new NUM_SXP(result);
        }

        S_EXP *applyRelOp(Token op, int n1, int n2) {
            bool result;
            switch (op.getType()) {
                case LESS_THAN:
                    result = n1 < n2;
                    break;
                case LTE:
                    result = n1 <= n2;
                    break;
                case GREATER_THAN:
                    result = n1 > n2;
                    break;
                case GTE:
                    result = n1 >= n2;
                    break;
                case EQUAL:
                    result = n1 == n2;
                    break;
                default:
                    result = nil;
                    break;
            }
            if (result)
                return TRUE;
            else
                return nil;
        }

        S_EXP *applyRelOp(Token op, double n1, double n2) {
            bool result;
            switch (op.getType()) {
                case LESS_THAN:
                    result = n1 < n2;
                    break;
                case LTE:
                    ERROR("Warning: equality check is not recommended for floating point values");
                    result = n1 <= n2;
                    break;
                case GREATER_THAN:
                    result = n1 > n2;
                    break;
                case GTE:
                    ERROR("Warning: equality check is not recommended for floating point values");
                    result = n1 >= n2;
                    break;
                case EQUAL:
                    ERROR("Warning: equality check is not recommended for floating point values");
                    result = n1 == n2;
                    break;
                default:
                    result = nil;
                    break;
            }
            if (result)
                return TRUE;
            else
                return nil;
        }

        S_EXP *applyValueOp(Token op, VALUELIST *vl) {
            cout << "in apply val op" << endl;
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
            if (op.getType() == PLUS || op.getType() == MINUS || op.getType() == MULTIPLY || op.getType() == DIVIDE || op.getType() == MOD ||
                op.getType() == EQUAL || op.getType() == LESS_THAN || op.getType() == GREATER_THAN || op.getType() == LTE || op.getType() == GTE) {
                if (s1->type == "Number" && s2->type == "Number") {
                    NUM_SXP *n1 = (NUM_SXP*) s1;
                    NUM_SXP *n2 = (NUM_SXP*) s2;

                    if (op.getType() == PLUS || op.getType() == MINUS || op.getType() == MULTIPLY || op.getType() == DIVIDE || op.getType() == MOD) {
                        if (n1->type2 == n2->type2){
                            if (n1->type2 == "Integer")
                                result = applyArithOp(op, n1->intVal, n2->intVal);
                            else
                                result = applyArithOp(op, n1->doubleVal, n2->doubleVal);
                        }
                        else{
                            if (n1->type2 == "Integer"){
                                result = applyArithOp(op, n1->intVal, (int)(n2->doubleVal));
                            }
                            else
                                result = applyArithOp(op, (int)(n1->doubleVal), n2->intVal);
                        }
                    }
                    else
                        if (n1->type2 == n2->type2){
                            if (n1->type2 == "Integer")
                                result = applyRelOp(op, n1->intVal, n2->intVal);
                            else
                                result = applyRelOp(op, n1->doubleVal, n2->doubleVal);
                        }
                        else{
                            ERROR("Comparing unequal types, converting both values to int");
                            if (n1->type2 == "Integer"){
                                result = applyRelOp(op, n1->intVal, (int)(n2->doubleVal));
                            }
                            else
                                result = applyRelOp(op, (int)(n1->doubleVal), n2->intVal);
                        }
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
            //cout << "applying" << endl;
            S_EXP *result = nil;
            switch (op.getType()) {
                case CONS:
                    cout << "making a new list" << endl;
                    cout << s1->type << " " << s2->type << endl;
                    result = new LIST_SXP(s1, s2);
                    break;
                case EQUAL:
                    if (s1 == nil && s2 == nil) {
                        result = TRUE;
                    } else if (s1->type == "Number" && s2->type == "Number") {
                        NUM_SXP *n1 = (NUM_SXP *)s1;
                        NUM_SXP *n2 = (NUM_SXP *)s2;
                        if (n1->type2 != n2->type2){
                            cout << "Cannot compare float and int" << endl;
                            break;
                        }
                        else {
                            if (n1->type2 == "Integer"){
                                if (n1->intVal == n2->intVal)
                                    result = TRUE;
                            }
                            else {
                                cout << "Warning: Equality comparison between floats" << endl;
                                if (n1->doubleVal == n2->doubleVal)
                                    result = TRUE;
                            }
                        }
                    } else if (s1->type == "Symbol" && s2->type == "Symbol") {
                        SYM_SXP *n1 = (SYM_SXP*)s1;
                        SYM_SXP *n2 = (SYM_SXP*)s2;
                        if (n1->symVal == n2->symVal)
                            result = TRUE;
                    }
                    break;
                default:
                    ERROR("incorrect op type for apply function");
                    break;
            }
            return result;
        }

        S_EXP *apply(Token op, S_EXP *s1) {
            S_EXP *result = nil;
            switch(op.getType()) {
                // case NOT:
                //     if (s1 == nil)
                //         result = TRUE;
                //     break;
                case CAR:
                    if (s1->type == "List") {
                        LIST_SXP *concell = (LIST_SXP *)s1;
                        result = concell->carVal;
                    }
                    else
                        ERROR("car applied to non-list");
                    break;
                case CDR:
                    if (s1->type == "List") {
                        LIST_SXP *concell = (LIST_SXP *)s1;
                        result = concell->cdrVal;
                    }
                    else
                        ERROR("cdr applied to non-list");
                    break;
                case IS_NULL:
                    if (s1 == nil)
                        result = TRUE;
                    break;
                case IS_NUMBER:
                    if (s1->type == "Number")
                        result = TRUE;
                    break;
                case IS_SYMBOL:
                    if (s1->type == "Symbol" || s1->type == "TRUE")
                        result = TRUE;
                    break;
                case IS_LIST:
                    if (s1->type == "List")
                        result = TRUE;
                    break;
                case PRINT:
                    if (s1->type == "Symbol"){
                        cout << ((SYM_SXP *)s1)->symVal << endl;
                    }
                    else {
                        cout << s1->toString() << endl;
                    }
                    result = s1;
                    break;
                default:
                    ERROR("invalid apply");
                    break;
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
            else if (expression->name == "strexp"){
                return new SYM_SXP(((STREXP *)expression)->strVal);
            }
            else if (expression->name == "varexp") {
                VAREXP* exp = (VAREXP*)expression;
                if (isBound(exp->varble, rho)) {
                    return fetch(exp->varble, rho);
                }
                if (isBound(exp->varble, globalEnv)) {
                    return fetch(exp->varble, globalEnv);
                }
                ERROR("Undefined variable " + exp->varble);
                return nil;
                //return new SYM_SXP(exp->varble);
            }
            else if (expression->name == "apexp") {
                APEXP* exp = (APEXP*)expression;
                op = exp->op;
                if (op.getType() == IDENTIFIER) {
                    cout << "USER FUNCTION" << endl;
                    return applyUserFun(op.getVal(), evalList(exp->args, rho));
                }
                else {
                    if (op.getType() == IF || op.getType() == SET || op.getType() == WHILE || op.getType() == BEGIN) {
                        cout << "applyCtrlOp" << endl;
                        return applyCtrlOp(op, exp->args, rho);
                    }
                    else {
                        cout << "applyValueOp" << endl;
                        return applyValueOp(op, evalList(exp->args, rho));
                    }
                }
            }
            cout << "no expression match for " << expression->name << endl;
            return nil;
        }

        bool isTrueVal (S_EXP *s_exp) {
            return !(s_exp->toString() == "()");
        }

        bool isBound (string name, ENV *rho){
            return (findVar(name, rho) != NULL);
        }
        
        S_EXP *applyUserFun(string name, VALUELIST *actuals){
            FUNDEF *fun = fetchFun(name);
            if (fun == NULL){
                ERROR("Undefined function " + name + "\n");
                return nil;
            }
            if (lengthNL(fun->formals) != lengthVL(actuals)) {
                ERROR("Wrong number of arguments to " + name + "\n");
                return nil;
            }
            ENV *rho = new ENV(fun->formals, actuals);
            return eval(fun->body, rho);
        }

};

#endif