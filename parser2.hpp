#include <string>
#include <vector>
#include <iostream>
#include "token.hpp"
#include "expression3.hpp"

using namespace std;

#ifndef _PARSER_
#define _PARSER_

EXP *nil = new EXP();

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

FUNDEF *fundefs = NULL;

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


class Parser {
    public:
        vector<Token> tokenList;
        int pos;
        
        string parseDef() {
            if(tokenList[pos].getType() == LEFT_PAREN) {
                pos++;
                if(tokenList[pos].getType() == DEFINE) {
                    pos++;
                    string fname = tokenList[parseName()].getVal();
                    if(tokenList[pos].getType() == LEFT_PAREN) pos++;
                    else {
                        ERROR("expected (");
                        return "error";
                    }
                    NAMELIST* nl = parseNL();     
                    EXP* e = parseExp();
                    if(tokenList[pos].getType() == RIGHT_PAREN) pos++;
                    else {
                        ERROR("expected )");
                        return "error";
                    }
                    newFunDef(fname, nl, e);
                    cout << "Function list after that call: " << endl;
                    FUNDEF *functions = fundefs;
                    while (functions != NULL){
                        cout << functions->funName << endl;
                        functions = functions->next;
                    }
                    return fname;
                }
            }
            else {
                ERROR("Parse define error");
            }
            return "error";
        }

        int parseName() {
            // cout << "in parse name" << endl;
            // needed to broaden the definition of name to include builtins
            if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT) {
                ERROR("Expected name, instead read :", tokenList[pos]);
                return -1;
            }
            string name = tokenList[pos].getVal();
            pos++;
            // install(name);
            return pos - 1;
        }

        NAMELIST *parseNL() {
            string nm;
            NAMELIST *nl;
            if (tokenList[pos].getType() == RIGHT_PAREN) {
                pos++;
                return NULL;
            }
            else {
                nm = tokenList[parseName()].getVal();
                nl = parseNL();
                return new NAMELIST(nm, nl);
            }
        }

        EXP *parseExp() {
            // cout << "in parseExp with pos " << pos << endl;
            // string nm;
            EXPLIST *el;
            int posOfToken;
            if (tokenList[pos].getType() == END_OF_FILE) {
                return NULL;
            }
            else if (tokenList[pos].getType() == LEFT_PAREN) { // APEXP
                pos++;
                if (tokenList[pos].getType() == RIGHT_PAREN) { // NIL
                    pos++;
                    return nil;
                }
                else if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT || tokenList[pos].getType() == STRING) {
                    return new VALEXP(parseList());
                }
                posOfToken = parseName();
                el = parseEL();
                return new APEXP(tokenList[posOfToken], el);
            }
            else if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT || tokenList[pos].getType() == T || (tokenList[pos].getType() == LEFT_PAREN && tokenList[pos+1].getType() == RIGHT_PAREN)) {
                return new VALEXP(parseVal());
            }
            else if (tokenList[pos].getType() == STRING){
                return new STREXP(tokenList[parseName()].getVal());
            }
            else {
                return new VAREXP(tokenList[parseName()].getVal());
            }
        }

        EXPLIST *parseEL() {
            if (tokenList[pos].getType() == END_OF_FILE) {
                ERROR("Expected ) found EOF");
                return NULL;
            }
            if (tokenList[pos].getType() == RIGHT_PAREN) {
                pos++;
                return NULL;
            }
            else {
                EXP *e = parseExp();
                EXPLIST *el = parseEL();
                return new EXPLIST(e, el);
            }
        }

        S_EXP *parseList() {
            cout << "in parse list" << endl;
            S_EXP *car, *cdr;
            if (tokenList[pos].getType() == RIGHT_PAREN) {
                pos++;
                return new S_EXP();
                // return mkSExp(NILSXP);
            }
            else {
                car = parseSExp();
                cdr = parseList();
                return new LIST_SXP(car, cdr);
            }
        }

        S_EXP *parseInt() {
            double value;
            if (tokenList[pos].getType() == MINUS && tokenList[pos+1].getType() == INTEGER) {
                value = (double)(-1 * tokenList[pos+1].getIntVal());
                pos += 2;
                return new NUM_SXP((int)value);
            }
            else if (tokenList[pos].getType() == MINUS && tokenList[pos+1].getType() == FLOAT) {
                value = -1 * tokenList[pos+1].getFloatVal();
                pos += 2;
                return new NUM_SXP(value);
            }
            else if (tokenList[pos].getType() == FLOAT) {
                value = tokenList[pos].getFloatVal();
                pos += 1;
                return new NUM_SXP(value);
            }
            else {
                value = (double)(tokenList[pos].getIntVal());
                pos += 1;
                return new NUM_SXP((int)value);
            }
            cout << "Should not be in this part of ParseInt()" << endl;
            //return new NUM_SXP(value);
        }

        S_EXP *parseSym() {
            string symbolname = tokenList[parseName()].getVal();
            return new SYM_SXP(symbolname);
        }

        S_EXP *parseSExp() {
            if (tokenList[pos].getType() == T) {
                pos++;
                return new S_EXP("TRUE");
            }
            if (tokenList[pos].getType() == LEFT_PAREN) {
                pos++;
                return parseList();
            }
            if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT)
                return parseInt();
            return parseSym();
        }
        S_EXP *parseVal() {
            // if (userinput(pos, "parseSExp") == Token.TIC)
            //     pos++;
            return parseSExp();
        }


        Parser(vector<Token> tokenList) {
            this->tokenList = tokenList;
            pos = 0;
        }

        EXP* getNextExpression() {
            if (tokenList[pos].getType() == LEFT_PAREN && tokenList[pos+1].getType() == DEFINE) {
                parseDef();
            }
            return parseExp();
        }
        void newFunDef(string name, NAMELIST *nl, EXP *e) {
            if (fetchFun(name) == NULL){
                fundefs = new FUNDEF(name, nl, e, fundefs);
            }
            else ERROR(name + " already installed\n");
        }
};

#endif