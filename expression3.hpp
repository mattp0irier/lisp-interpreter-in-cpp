#include <string>
#include <typeinfo>
#include <memory>

using namespace std;

#ifndef _EXPR_
#define _EXPR_


//trevor
class S_EXP {
    public:
        string type;

        S_EXP(){
            this->type = "()";
        }

        S_EXP(string type){
            this->type = type;
        }

        string toString(){
            return type;
        }
};

class NUM_SXP: public S_EXP {
    public:
        int intVal;
        double doubleVal;
        string type2;
    
        NUM_SXP(int intVal){
            this->type = "Number";
            this->type2 = "Integer";
            this->intVal = intVal;
        }

        NUM_SXP(double doubleVal){
            this->type = "Number";
            this->type2 = "Float";
            this->doubleVal = doubleVal;
        }

        string toString(){
            if (type2 == "Integer")
                return to_string(intVal);
            else if (type2 == "Float")
                return to_string(doubleVal);
            else{
                cout << "This number isn't an integer or a float. what?" << endl;
                return "oops";
            }
        }
};

class SYM_SXP: public S_EXP {
    public:
        string symVal;

        SYM_SXP(string symVal){
            this->type = "Symbol";
            this->symVal = symVal;
        }

        string toString(){
            return symVal;
        }
};

class LIST_SXP: public S_EXP {
    public:
        S_EXP *carVal, *cdrVal;

        LIST_SXP(S_EXP *carVal, S_EXP *cdrVal) {
            this->type = "List";
            this->carVal = carVal;
            this->cdrVal = cdrVal;
        }

        string toString(){
            cout << carVal->type << " " << cdrVal->type << endl;
            string list = "(";
            if (carVal->type == "Number"){
                list += to_string(((NUM_SXP *)carVal)->intVal);
            }
            else if (carVal->type == "Symbol"){
                list += ((SYM_SXP *)carVal)->symVal;
            }
            if (cdrVal->type == "()") {
                list += ")";
                return list;
            }
            else if (cdrVal->type == "List") {
                LIST_SXP *temp = (LIST_SXP *)cdrVal;
                list += " " + temp->toString();
            }
            else if (cdrVal->type == "Number"){
                list += " " + to_string(((NUM_SXP *)cdrVal)->intVal);
            }
            else if (carVal->type == "Symbol"){
                list += " " + ((SYM_SXP *)cdrVal)->symVal;
            }
            list += ")";
            return list;
        }
};

//matt

class EXP {
    public:
    string name;
    EXP(){
        name = "()";
    }
    EXP(string name){
        this->name = name;
    }
};

class VALEXP: public EXP {
    public:
        S_EXP *sxp;
        VALEXP(S_EXP *sxp) {
            this->sxp = sxp;
            this->name = "valexp";
        }
};

class VAREXP: public EXP {
    public:
        string varble;
        VAREXP(string varble) {
            this->varble = varble;
            this->name = "varexp";
        }
};

class STREXP: public EXP {
    public:
        string strVal;

        STREXP(string strVal){
            this->name = "strexp";
            this->strVal = strVal;
        }
};

class EXPLIST {
    public:
        EXP *head;
        EXPLIST *tail;

        EXPLIST() {
            this->head = NULL;
            this->tail = NULL;
        }

        EXPLIST(EXP *head, EXPLIST *tail) {
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

        void toString() {
            EXPLIST *cur = this;
            while (cur != NULL){
                cout << cur->head->name << endl;
                if (cur->tail != NULL && cur->tail->head != NULL){
                    cur = tail;
                }
                else break;
            }
            //cout << "done" << endl;
        }

};

class APEXP: public EXP {
    public:
        Token op;
        EXPLIST *args;

        APEXP(Token op, EXPLIST *args) {
            this->op = op;
            this->args = args;
            this->name = "apexp";
        }
};

class VALUELIST {
    private:
        
    public:
        S_EXP *head;
        VALUELIST *tail;

        VALUELIST() {
            
        }

        VALUELIST(S_EXP *head, VALUELIST *tail) {
            this->head = head;
            this->tail = tail;
        }

        VALUELIST(const VALUELIST &src){
            this->head = src.head;
            this->tail = src.tail;
        }

        VALUELIST &operator= (const VALUELIST &src) {
            this->head = src.head;
            this->tail = src.tail;
            return *this;
        }

};

class NAMELIST {
    private:

    public:
        string head;
        NAMELIST *tail;

        NAMELIST() {

        }

        NAMELIST(string head, NAMELIST *tail) {
            this->head = head;
            this->tail = tail;
        }

        NAMELIST(const NAMELIST &src){
            this->head = src.head;
            this->tail = src.tail;
        }

        NAMELIST &operator= (const NAMELIST &src) {
            this->head = src.head;
            this->tail = src.tail;
            return *this;
        }

};

class ENV {
    public:
        NAMELIST *vars;
        VALUELIST *values;

        ENV(NAMELIST *vars, VALUELIST *values){
            this->vars = vars;
            this->values = values;
        }
};

ENV *emptyEnv(){
    return new ENV(NULL, NULL);
}


VALUELIST *findVar(string name, ENV *rho) {
    NAMELIST *nl = rho->vars;
    VALUELIST *values = rho->values;
    bool found = false;
    while (nl != NULL && !found){
        if (nl->head == name){
            found = true;
        }
        else {
            nl = nl->tail;
            values = values->tail;
        }
    }
    return values;
}

S_EXP *fetch(string name, ENV *rho) {
    VALUELIST *vl = findVar(name, rho);
    return vl->head;
}

void bindVar(string name, S_EXP *s, ENV *rho) {
    rho->vars = new NAMELIST(name, rho->vars);
    rho->values = new VALUELIST(s, rho->values);
}

void assign(string name, S_EXP *s, ENV *rho) {
    VALUELIST *location = findVar(name, rho);
    location->head = s;
}

int lengthVL(VALUELIST *vl){
    int len = 0;
    while (vl != NULL){
        vl = vl->tail;
        len++;
    }
    return len;
}

int lengthNL(NAMELIST *nl){
    int len = 0;
    while (nl != NULL) {
        nl = nl->tail;
        len++;
    }
    return len;
}

#endif