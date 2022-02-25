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
        
        string parseDef() {
            expect(Token.OPEN_PAR, "parseDef");
            expect(Token.DEFINE, "parseDef");
            String fname = parseName(); // function name expect(Token.OPEN_PAR,"parseDef"); // skip ( NAMELIST nl = parseNL();
            ;                           // formal parameters
            EXP e = parseExp();         // body expect(Token.CLOSE_PAR,"parseDef"); // skip ) newFunDef(fname, nl, e);
            return fname;
        }

        string parseName() {
            // needed to broaden the definition of name to include builtins
            if (userinput(pos, "parseName").type == Token.NUMBER) {
                ERROR("Expected name, instead read :", userinput(pos, "parseName"));
                return "error";
                String name = userinput(pos, "parseName").text;
                pos++;
                // install(name);
                return name;
            }
            NAMELIST parseNL() {
                String nm;
                NAMELIST nl;
                if (userinput(pos, "parseNL") == Token.CLOSE_PAR) {
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
                String nm;
                EXPLIST el;
                if (userinput(pos, "parseExp") == Token.OPEN_PAR) { // APEXP
                    pos++;
                    if (userinput(pos, "parseExp") == Token.CLOSE_PAR) { // NIL
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
                if (userinput(pos, "parseEL") == Token.EOF) {
                    ERROR("Expected ) found EOF");
                    return null;
                }
                if (userinput(pos, "parseEL") == Token.CLOSE_PAR) {
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
