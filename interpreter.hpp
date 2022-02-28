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

// global envirmonment created
ENV *globalEnv = emptyEnv();

// Interpreter class: evaluates expressions and returns output
class Interpreter {
    private:
        // define singleton nil and true values
        S_EXP *nil = new S_EXP("()");
        S_EXP *TRUE = new S_EXP("TRUE");

        // evalList: takes expression list as input and evaluates each expression recursively
        VALUELIST *evalList(EXPLIST *el, ENV *rho) {
            // recursive break statement
            if (el == NULL)
                return NULL;
            
            S_EXP *h = eval(el->head, rho); // evaluate head
            VALUELIST *t = evalList(el->tail, rho); // evaluate rest of list
            return new VALUELIST(h, t);
        }

        // applyCtrlOp: evaluate if, while, set, and begin operations
        S_EXP *applyCtrlOp(Token controlOP, EXPLIST *args, ENV *rho) {
            S_EXP *s = nil;
            string varble;

            // switch statement on TokenType
            switch (controlOP.getType()) {
                case IF:
                    // if conditional statement evaluates to true
                    if (isTrueVal(eval(args->head, rho))){
                        return eval(args->tail->head, rho); // return true branch
                    }
                    else{
                        return eval(args->tail->tail->head, rho); // else return false branch
                    }
                    break;
                case WHILE:
                    s = eval(args->head, rho);
                    // while conditional statement does not evaluate to nil
                    while (s->toString() != "()") {
                        s = eval(args->tail->head, rho); // evaluate expression branch
                        s = eval(args->head, rho); // evaluate conditional branch
                    }
                    return s;
                    break;
                case SET:
                    varble = ((VAREXP *)(args->head))->varble;

                    // evaluate set argument properly based on type of expression
                    if (args->tail->head->name == "valexp") s = eval(args->tail->head, rho);
                    else if (args->tail->head->name == "strexp") s = eval(args->tail->head, rho);
                    else if (args->tail->head->name == "varexp"){
                        // check if var exists in local environment
                        if (isBound(((VAREXP *)(args->tail->head))->varble, rho)){
                            s = eval(args->tail->head, rho); // updates if exists
                        }
                        // check if var exists in global environment
                        else if (isBound(((VAREXP *)(args->tail->head))->varble, globalEnv)){
                            s = eval(args->tail->head, globalEnv); 
                        }
                        // create new var since it doesn't exist
                        else s = new SYM_SXP(((VAREXP *)(args->tail->head))->varble);
                    }
                    else if (args->tail->head->name == "apexp") {
                        s = eval(args->tail->head, rho);
                    }
                    else {
                        // we really should never be here
                        cout << "please don't be here" << endl;
                    }
                    if (isBound(varble, rho))
                        assign(varble, s, rho); // updates if exists
                    else if (isBound(varble, globalEnv))
                        assign(varble, s, globalEnv); // updates if exists
                    else
                        bindVar(varble, s, rho); // bind variable since didn't exist
                    return s;
                    break;
                case BEGIN:
                    // while there are args, evaluate each expression
                    while (args != NULL) {
                        s = eval(args->head, rho);
                        args = args->tail;
                    }
                    return s; // return result of last expression
                    break;
                default:
                    break;
            }
            return s;
        }

        // applyArithOp: evaluate int arithmetic operations
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
            
        // applyArithOp: evaluate double arithmetic operations
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

        // applyRelOp: apply relational operations on integers
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

        // applyRelOp: apply relational operations on integers
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

        // applyValueOp: checks for correct arguments and calls corresponding function to evaluate operation
        S_EXP *applyValueOp(Token op, VALUELIST *vl) {
            S_EXP *result = nil;
            S_EXP *s1 = nil;
            S_EXP *s2 = nil;

            // check for correct number of arguments
            if (op.getArity() != 0 && op.getArity() != lengthVL(vl)) {
                ERROR("Wrong number of arguments to " + op.getVal() + " expected " + to_string(op.getArity()) + " but found " + to_string(lengthVL(vl)));
                return nil;
            }

            s1 = vl->head; // get 1st value
            if (op.getArity() == 2)
                s2 = vl->tail->head; // get 2nd value if operation needs it
            // check for correct operation type
            if (op.getType() == PLUS || op.getType() == MINUS || op.getType() == MULTIPLY || op.getType() == DIVIDE || op.getType() == MOD ||
                op.getType() == EQUAL || op.getType() == LESS_THAN || op.getType() == GREATER_THAN || op.getType() == LTE || op.getType() == GTE) {
                // check for numbers being provided as arguments
                if (s1->type == "Number" && s2->type == "Number") {
                    NUM_SXP *n1 = (NUM_SXP*) s1;
                    NUM_SXP *n2 = (NUM_SXP*) s2;

                    // if arithmatic op, call applyArithOp
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
                    // else relational op, call applyRelOp
                    else {
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
                }
                else {
                    ERROR("Non-arithmatic arguments to " + op.getVal());
                }
            }
            // if non value op, call apply with correct number of parameters
            else if (op.getArity() == 2) {
                result = apply(op, s1, s2);
            }
            else {
                result = apply(op, s1);
            }
            return result;
        }

        // apply: evaluates functions with two params
        S_EXP *apply(Token op, S_EXP* s1, S_EXP* s2) {
            S_EXP *result = nil;

            // switch on operation TokenType
            switch (op.getType()) {
                case CONS:
                    result = new LIST_SXP(s1, s2);
                    break;
                case EQUAL:
                    // if both nil, equal
                    if (s1 == nil && s2 == nil) {
                        result = TRUE;

                    // if both numbers, check equality
                    } else if (s1->type == "Number" && s2->type == "Number") {
                        NUM_SXP *n1 = (NUM_SXP *)s1;
                        NUM_SXP *n2 = (NUM_SXP *)s2;

                        // check for both ints or both floats
                        if (n1->type2 != n2->type2){
                            cout << "Cannot compare float and int" << endl;
                            break;
                        }

                        // otherwise perform comparison
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

        // apply: evaluates functions with one param
        S_EXP *apply(Token op, S_EXP *s1) {
            S_EXP *result = nil;

            // switch on operation TokenType
            switch(op.getType()) {
                case CAR:
                    // check for list type
                    if (s1->type == "List") {
                        LIST_SXP *concell = (LIST_SXP *)s1;
                        result = concell->carVal;
                    }
                    else
                        ERROR("car applied to non-list");
                    break;
                case CDR:
                    // check for list type
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
                    // check for type and cast if necessary
                    if (s1->type == "Symbol"){
                        cout << ((SYM_SXP *)s1)->symVal << endl;
                    }
                    else if (s1->type == "Number"){
                        cout << ((NUM_SXP *)s1)->toString() << endl;
                    }
                    else {
                        cout << s1->toString() << endl;
                    }
                    result = NULL;
                    break;
                default:
                    ERROR("invalid apply");
                    break;
            }
            return result;
        }

    public:
        // Interpreter: empty interpreter constructor
        Interpreter() {

        }

        // eval: evaluates provided EXP and returns S_EXP of result
        S_EXP *eval(EXP *expression, ENV *rho) {
            Token op;

            // if valexp, cast and return
            if (expression->name == "valexp") {
                VALEXP* exp = (VALEXP*)expression;
                return exp->sxp;
            }

            // if strexp (string literal), cast and return
            else if (expression->name == "strexp"){
                return new SYM_SXP(((STREXP *)expression)->strVal);
            }

            // if varexp, lookup variable in local and global environments and get value
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
            }

            // if apexp, check for identifier of userfun, otherwise check control op and value op
            else if (expression->name == "apexp") {
                APEXP* exp = (APEXP*)expression;
                op = exp->op;
                if (op.getType() == IDENTIFIER) {
                    return applyUserFun(op.getVal(), evalList(exp->args, rho));
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
            return nil; // if fall through
        }

        // isTrueVal: checks that S_EXP does not evaluate to () [nil]
        bool isTrueVal (S_EXP *s_exp) {
            return !(s_exp->toString() == "()");
        }

        // isBound: checks if variable exists in lookup table
        bool isBound (string name, ENV *rho){
            return (findVar(name, rho) != NULL);
        }
        
        // applyUserFun: if userfun found and matched, evaluate
        S_EXP *applyUserFun(string name, VALUELIST *actuals){
            FUNDEF *fun = fetchFun(name);

            // check if function defined
            if (fun == NULL){
                ERROR("Undefined function " + name + "\n");
                return nil;
            }

            // check if correct number of arguments
            if (lengthNL(fun->formals) != lengthVL(actuals)) {
                ERROR("Wrong number of arguments to " + name + "\n");
                return nil;
            }

            // evaluate
            ENV *rho = new ENV(fun->formals, actuals);
            return eval(fun->body, rho);
        }

};

#endif