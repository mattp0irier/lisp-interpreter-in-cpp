#include <string>
#include <vector>
#include <iostream>
#include "token.hpp"
#include "expression3.hpp"

using namespace std;

#ifndef _PARSER_
#define _PARSER_

// global nil Expression
EXP *nil = new EXP();

// Function Definition Class: holds a user-defined function
class FUNDEF {
    public:
        string funName;
        NAMELIST *formals;
        EXP *body;
        FUNDEF *next;

        FUNDEF(string funName, NAMELIST *formals, EXP *body, FUNDEF *next){
            this->funName = funName;
            this->formals = formals;
            this->body = body;
            this->next = next;
        }
};

// Global list of available user functions
FUNDEF *fundefs = NULL;

// fetchFun: finds the definition of a named function in the global list, 
//           or returns null if not found
FUNDEF *fetchFun(string name){
    FUNDEF *f = fundefs;
    while (f != NULL){
        if (f->funName == name){
            return f;
        }
        f = f->next;
    }
    return NULL;
}

// Parser Class: Can parse and evaluate a vector of tokens
class Parser {
    public:
        vector<Token> tokenList;
        int pos;
        
        // parseDef: used for function definitions
        string parseDef() {
            if(tokenList[pos].getType() == LEFT_PAREN) {
                // statement starts with parentheses
                pos++;
                if(tokenList[pos].getType() == DEFINE) {
                    // now it's ready to define
                    pos++;

                    // parse first argument, the function name
                    string fname = tokenList[parseName()].getVal();

                    // arguments are given in list form
                    if(tokenList[pos].getType() == LEFT_PAREN) pos++;
                    else {
                        ERROR("expected (");
                        return "error";
                    }

                    // expected arguments written into a namelist
                    NAMELIST* nl = parseNL();  

                    // function definition is parsed into an expression  
                    EXP* e = parseExp();
                    
                    cout << "parsed expression ending at pos " << pos << endl;

                    // define call ends with )
                    if(tokenList[pos].getType() == RIGHT_PAREN) pos++;
                    else {
                        cout << "Error at pos " << pos << " got " << tokenList[pos].getType() << endl;
                        ERROR("expected ) ");
                        return "error";
                    }
                    
                    // add new function to function list
                    newFunDef(fname, nl, e);
                    return fname;
                }
            }
            else {
                // improper syntax
                ERROR("Parse define error");
            }
            return "error";
        }

        // parseName: returns the token position of a string and increments pos counter
        int parseName() {
            if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT) {
                ERROR("Expected name, instead read :", tokenList[pos]);
                return -1;
            }
            //string name = tokenList[pos].getVal();
            pos++;
            // install(name);
            return pos - 1;
        }

        // parseNL: creates a namelist object from tokens
        NAMELIST *parseNL() {
            string nm;
            NAMELIST *nl;

            if (tokenList[pos].getType() == RIGHT_PAREN) {
                // empty list bounces out
                pos++;
                return NULL;
            }
            else {
                // recursively construct list of names
                nm = tokenList[parseName()].getVal();
                nl = parseNL();
                return new NAMELIST(nm, nl);
            }
        }

        // parseExp: creates an expression from tokens
        EXP *parseExp() {
            // cout << "in parseExp with pos " << pos << endl;
            // string nm;
            EXPLIST *el;
            int posOfToken;
            if (tokenList[pos].getType() == END_OF_FILE) {
                // EOF does nothing
                return NULL;
            }
            else if (tokenList[pos].getType() == LEFT_PAREN) {
                pos++;
                if (tokenList[pos].getType() == RIGHT_PAREN) { 
                    // NIL
                    pos++;
                    return nil;
                }
                else if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT || tokenList[pos].getType() == STRING || tokenList[pos].getType() == LEFT_PAREN) {
                    // Literal value starts a list
                    return new VALEXP(parseList());
                }

                // value isn't a literal, so it's some sort of name
                posOfToken = parseName();

                // name takes expressions as arguments
                el = parseEL();

                // ap expression found
                return new APEXP(tokenList[posOfToken], el);
            }
            else if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT || tokenList[pos].getType() == T || (tokenList[pos].getType() == LEFT_PAREN && tokenList[pos+1].getType() == RIGHT_PAREN)) {
                // some value found
                return new VALEXP(parseVal());
            }
            else if (tokenList[pos].getType() == STRING){
                // string literal
                return new STREXP(tokenList[parseName()].getVal());
            }
            else {
                // must be a user-defined name
                return new VAREXP(tokenList[parseName()].getVal());
            }
        }

        // parseEL: read in a list of exprsessions
        EXPLIST *parseEL() {
            if (tokenList[pos].getType() == END_OF_FILE) {
                ERROR("Expected ) found EOF");
                return NULL;
            }
            if (tokenList[pos].getType() == RIGHT_PAREN) {
                // right paren ends list
                pos++;
                return NULL;
            }
            else {
                // recursively define list
                EXP *e = parseExp();
                EXPLIST *el = parseEL();
                return new EXPLIST(e, el);
            }
        }

        // parselist: creates List S_Expression
        S_EXP *parseList() {
            cout << "in parse list at pos " << pos << endl;
            S_EXP *car, *cdr;
            if (tokenList[pos].getType() == RIGHT_PAREN) {
                pos++;
                // default S_EXP constructor has value "()"
                return new S_EXP();
            }
            else if(tokenList[pos].getType() == LEFT_PAREN) {
                // cout << "Left paren at pos " << pos << endl;
                pos++;
                if(tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT || tokenList[pos].getType() == T || tokenList[pos].getType() == LEFT_PAREN) {
                    car = parseList();
                    cdr = parseList();
                    return new LIST_SXP(car, cdr, false);
                }
                else {
                    // car is expression, cdr is list
                    EXP* carExp = parseExp();
                    cdr = parseList();
                    return new LIST_EXP(carExp, cdr);
                }
            }
            else {
                // car is an S-Expression, cdr is a list
                car = parseSExp();
                cdr = parseList();
                return new LIST_SXP(car, cdr, false);
            }
        }

        // parseInt: creates a NUM_SXP (this name isn't exactly accurate because it may read a double)
        S_EXP *parseInt() {
            double value;
            if (tokenList[pos].getType() == MINUS && tokenList[pos+1].getType() == INTEGER) {
                // negative integer
                value = (double)(-1 * tokenList[pos+1].getIntVal());
                pos += 2;
                return new NUM_SXP((int)value);
            }
            else if (tokenList[pos].getType() == MINUS && tokenList[pos+1].getType() == FLOAT) {
                // negative float
                value = -1 * tokenList[pos+1].getFloatVal();
                pos += 2;
                return new NUM_SXP(value);
            }
            else if (tokenList[pos].getType() == FLOAT) {
                // non-negative float
                value = tokenList[pos].getFloatVal();
                pos += 1;
                return new NUM_SXP(value);
            }
            else {
                // non-negative integer
                value = (double)(tokenList[pos].getIntVal());
                pos += 1;
                return new NUM_SXP((int)value);
            }
            cout << "Should not be in this part of ParseInt()" << endl;
            //return new NUM_SXP(value);
        }

        // parseSym: creates a SYM_SXP from tokenList[pos]
        S_EXP *parseSym() {
            string symbolname = tokenList[parseName()].getVal();
            return new SYM_SXP(symbolname);
        }

        // parseSExp: creates either a , NUM_SXP, or SYM_SXP
        S_EXP *parseSExp() {
            if (tokenList[pos].getType() == T) {
                // literal true
                pos++;
                return new S_EXP("TRUE");
            }
            if (tokenList[pos].getType() == LEFT_PAREN) {
                // start of list
                pos++;
                return parseList();
            }
            if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT){
                // number
                return parseInt();
            }
            // must be a symbol, then
            return parseSym();
        }

        // parseVal: calls parseSExp
        S_EXP *parseVal() {
            return parseSExp();
        }

        // constructor takes token vector as an argument
        Parser(vector<Token> tokenList) {
            this->tokenList = tokenList;
            pos = 0;
        }

        // getNextExpression: either pulls a define statement or a full expression
        EXP* getNextExpression() {
            while (tokenList[pos].getType() == LEFT_PAREN && tokenList[pos+1].getType() == DEFINE) {
                parseDef();
            }
            return parseExp();
        }

        // newFunDef: installs a new function to global function list
        void newFunDef(string name, NAMELIST *nl, EXP *e) {
            if (fetchFun(name) == NULL){
                fundefs = new FUNDEF(name, nl, e, fundefs);
            }
            else ERROR(name + " already installed\n");
        }
};

#endif