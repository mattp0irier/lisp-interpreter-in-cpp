#include <string>
#include <vector>
#include <iostream>
#include "token.cpp"
#include "expression3.cpp"

using namespace std;

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

        EXP parseExp() {
            string nm;
            EXPLIST el;
            if (tokenList[pos].getType() == LEFT_PAREN) { // APEXP
                pos++;
                if (tokenList[pos].getType() == RIGHT_PAREN) { // NIL
                    pos++;
                    S_EXP nil = S_EXP("(");
                    return VALEXP(nil);
                }
                nm = parseName();
                el = parseEL();
                return APEXP(nm, el);
            }
            else if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT || tokenList[pos].getType() == T || tokenList[pos].getType() == LEFT_PAREN || tokenList[pos].getType() == RIGHT_PAREN)
                return VALEXP(parseVal());
            else
                return VAREXP(parseName());
        }

        EXPLIST parseEL() {
            if (tokenList[pos].getType() == END_OF_FILE) {
                // ERROR("Expected ) found EOF");
                return null;
            }
            if (tokenList[pos].getType() == RIGHT_PAREN) {
                pos++;
                return null;
            }
            else {
                EXP e = parseExp();
                EXPLIST el = parseEL();
                return EXPLIST(e, el);
            }
        }

        SEXP parseList() {
            SEXP car, cdr;
            if (tokenList[pos].getType() == RIGHT_PAREN) {
                pos++;
                return NIL;
                // return mkSExp(NILSXP);
            }
            else {
                car = parseSExp();
                cdr = parseList();
                return new LISTSXP(car, cdr);
            }
        }

        SEXP parseInt() {
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
            return new NUMSXP(value);
        }

        SEXP parseSym() {
            string symbolname = parseName();
            return new SYMSXP(symbolname);
        }

        SEXP parseSExp() {
            if (tokenList[pos].getType() == T) {
                pos++;
                return TRUE;
            }
            if (tokenList[pos].getType() == LEFT_PAREN) {
                pos++;
                return parseList();
            }
            if (tokenList[pos].getType() == INTEGER || tokenList[pos].getType() == FLOAT)
                return parseInt();
            return parseSym();
        }
        SEXP parseVal() {
            // if (userinput(pos, "parseSExp") == Token.TIC)
            //     pos++;
            return parseSExp();
        }

    public:
        Parser(vector<Token> tokenList) {
            this->tokenList = tokenList;
            int pos = 0;
        }

        EXP getNextExpression() {
            // do things
        }

};
