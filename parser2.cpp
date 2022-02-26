#include <string>
#include <vector>
#include <iostream>
#include "token.cpp"
#include "expression3.cpp"

using namespace std;

#ifndef _PARSER_
#define _PARSER_

class Parser {
    private:
        vector<Token> tokenList;
        int pos;
        
        // string parseDef() {
        //     expect(Token.OPEN_PAR, "parseDef");
        //     expect(Token.DEFINE, "parseDef");
        //     string fname = parseName(); // function name expect(Token.OPEN_PAR,"parseDef"); // skip ( NAMELIST nl = parseNL();
        //     ;                           // formal parameters
        //     EXP e = parseExp();         // body expect(Token.CLOSE_PAR,"parseDef"); // skip ) newFunDef(fname, nl, e);
        //     return fname;
        // }

        string parseName() {
            cout << "in parse name" << endl;
            // needed to broaden the definition of name to include builtins
            if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT) {
                // ERROR("Expected name, instead read :", userinput(pos, "parseName"));
                return "error";
            }
            string name = tokenList[pos].getVal();
            pos++;
            // install(name);
            return name;
        }

        NAMELIST *parseNL() {
            string nm;
            NAMELIST *nl;
            if (tokenList[pos].getType() == RIGHT_PAREN) {
                pos++;
                return NULL;
            }
            else {
                nm = parseName();
                nl = parseNL();
                return new NAMELIST(nm, nl);
            }
        }

        EXP *parseExp() {
            cout << "in parseExp with pos " << pos << endl;
            string nm;
            EXPLIST *el;
            if (tokenList[pos].getType() == END_OF_FILE) {
                cout << "found EOF at pos " << pos << endl;
                return NULL;
            }
            else if (tokenList[pos].getType() == LEFT_PAREN) { // APEXP
                cout << "found left paren" << endl;
                pos++;
                if (tokenList[pos].getType() == RIGHT_PAREN) { // NIL
                    cout << "found nil" << endl;
                    pos++;
                    S_EXP nil = S_EXP("()");
                    return NULL;
                }
                nm = parseName();
                el = parseEL();
                return new APEXP(tokenList[pos], el);
            }
            else if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT || tokenList[pos].getType() == T || tokenList[pos].getType() == LEFT_PAREN || tokenList[pos].getType() == RIGHT_PAREN) {
                // cout << "found val exp at token " << tokenList[pos].getVal() << endl;
                return new VALEXP(parseVal());
            }
            else {
                cout << "found var exp" << endl;
                // return new VAREXP(parseName());
                return NULL;
            }
        }

        EXPLIST *parseEL() {
            // cout << "in parse expression list" << endl;
            if (tokenList[pos].getType() == END_OF_FILE) {
                // ERROR("Expected ) found EOF");
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
            int value;
            if (tokenList[pos].getType() == MINUS && tokenList[pos+1].getType() == INTEGER) {
                value = -1 * tokenList[pos+1].getIntVal();
                pos += 2;
            }
            else if (tokenList[pos].getType() == MINUS && tokenList[pos+1].getType() == FLOAT) {
                value = -1 * tokenList[pos+1].getFloatVal();
                pos += 2;
            }
            else if (tokenList[pos].getType() == FLOAT) {
                value = tokenList[pos].getFloatVal();
                pos += 1;
            }
            else {
                value = tokenList[pos].getIntVal();
                pos += 1;
            }
            return new NUM_SXP(value);
        }

        S_EXP *parseSym() {
            string symbolname = parseName();
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

    public:
        Parser(vector<Token> tokenList) {
            this->tokenList = tokenList;
            pos = 0;
        }

        EXP* getNextExpression() {
            return parseExp();
        }

};

#endif