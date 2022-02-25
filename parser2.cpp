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
                // return "error";
                string name = tokenList[pos].getVal();
                pos++;
                // install(name);
                return name;
            }

            NAMELIST parseNL() {
                string nm;
                NAMELIST nl;
                if (tokenList[pos].getType() == RIGHT_PAREN) {
                    pos++;
                    return null;
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
                        return new VALEXP(NIL);
                    }
                    nm = parseName();
                    el = parseEL();
                    return new APEXP(nm, el);
                }
                else if (isValue(pos, "parseExp"))
                    return VALEXP(parseVal());
                else
                    return VAREXP(parseName());
            }

            EXPLIST parseEL() {
                if (tokenList[pos].getType() == END_OF_FILE) {
                    ERROR("Expected ) found EOF");
                    return null;
                }
                if (tokenList[pos].getType() == RIGHT_PAREN) {
                    pos++;
                    return null;
                }
                else {
                    EXP e = parseExp();
                    EXPLIST el = parseEL();
                    return new EXPLIST(e, el);
                }
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
