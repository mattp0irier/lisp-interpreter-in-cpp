#include <string>
using namespace std;
//trevor
class S_EXP {
    public:
        string type;

        S_EXP(){
            type = "empty type";
        }

        S_EXP(string type){
            type = type;
        }

        string toString(){
            return type;
        }
};

class NUM_SXP: public S_EXP {
    public:
        int intVal;
    
        NUM_SXP(int intVal){
            type = "Number";
            intVal = intVal;
        }

        string toString(){
            return to_string(intVal);
        }
};

class SYM_SXP: public S_EXP {
    public:
        string symVal;

        SYM_SXP(string symVal){
            type = "Symbol";
            symVal = symVal;
        }

        string toString(){
            return symVal;
        }
};

class LIST_SXP: public S_EXP {
    public:
        S_EXP *carVal, *cdrVal;

        LIST_SXP(S_EXP *carVal, S_EXP *cdrVal) {
            type = "List";
            this->carVal = carVal;
            this->cdrVal = cdrVal;
        }

        string toString(){
            string list = "(" + carVal->toString();
            S_EXP *temp = cdrVal;
            while (temp->type == "List") {
                LIST_SXP *temp2 = (LIST_SXP *)temp;
                list += " " + temp2->carVal->toString();
                temp = temp2->cdrVal;
            }
            list += ")";
            return list;
        }
};

//matt

class EXP {
};

class VALEXP: public EXP {
    S_EXP sxp;
    VALEXP(S_EXP sxp) {
        this->sxp = sxp;
    }
};

class VAREXP: public EXP {
    string varble;
    VAREXP(string variable) {
        this->varble = variable;
    }
};

class APEXP: public EXP {
    private:
        string optr;
        EXPLIST args;
    public:
        APEXP(string optr, EXPLIST args) {
            this->optr = optr;
            this->args = args;
        }
};

class EXPLIST {
    private:
        EXP head;
        EXPLIST *tail;
    public:
        EXPLIST() {
            
        }

        EXPLIST(EXP head, EXPLIST *tail) {
            this->head = head;
            this->tail = tail;
        }

        EXPLIST(const EXPLIST &src){
            this->head = src.head;
            this->tail = src.tail;
        }

        EXPLIST &operator= (const EXPLIST &src) {
            this->head = src.head;
            this->tail = src.tail;
            return *this;
        }

};