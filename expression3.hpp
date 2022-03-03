#include <string>
#include <typeinfo>
#include <memory>

using namespace std;

#ifndef _EXPR_
#define _EXPR_

// General S-Expression Class
class S_EXP {
    public:
        string type;

        // Default type is ()
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

// Number S-Expression stores a number
class NUM_SXP: public S_EXP {
    public:

        // May be integer or floating point
        int intVal;
        double doubleVal;
        // type2 denotes what kind of number is stored, must be checked before accessing
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

        // toString handles either kind of number
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

//Symbol S-Expression stores symbols and string literals
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

// List S-Expression
class LIST_SXP: public S_EXP {
    public:
        // Every list has a car and cdr
        S_EXP *carVal, *cdrVal;
        bool isCons;

        LIST_SXP(S_EXP *carVal, S_EXP *cdrVal, bool isCons) {
            this->type = "List";
            this->carVal = carVal;
            this->cdrVal = cdrVal;
            this->isCons = isCons;
        }

        // Combine list into a string
        string toString(){
            string list;
            if(isCons) {
                list = "(";
                // car must be an atom: either Number or Symbol
                if (carVal->type == "Number"){
                    if (((NUM_SXP *)carVal)->type2 == "Float") list += to_string(((NUM_SXP *)carVal)->doubleVal);
                    else list += to_string(((NUM_SXP *)carVal)->intVal);
                }
                else if (carVal->type == "Symbol"){
                    list += ((SYM_SXP *)carVal)->symVal;
                }

                // cdr may be nil, another List, or an atom
                if (cdrVal->type == "()") {
                    list += ")";
                    return list;
                }

                // Necessary to cast pointer into the specific S-Expression derived class before accessing data
                else if (cdrVal->type == "Number"){
                    if (((NUM_SXP *)cdrVal)->type2 == "Float") list += " . " + to_string(((NUM_SXP *)cdrVal)->doubleVal);
                    else list += " . " + to_string(((NUM_SXP *)cdrVal)->intVal);
                }
                else if (cdrVal->type == "Symbol"){
                    list += " . " + ((SYM_SXP *)cdrVal)->symVal;
                }
                else if (cdrVal->type == "List"){
                    list += " . " + ((LIST_SXP *)cdrVal)->toString();
                }
                list += ")";
            }
            else {
                list = "(";

                // car must be an atom: either Number or Symbol
                // if list, it is a nested list
                if (carVal->type == "Number"){
                    if (((NUM_SXP *)carVal)->type2 == "Float") list += to_string(((NUM_SXP *)carVal)->doubleVal);
                    else list += to_string(((NUM_SXP *)carVal)->intVal);
                }
                else if (carVal->type == "Symbol"){
                    list += ((SYM_SXP *)carVal)->symVal;
                }
                else if (carVal->type == "List") {
                    LIST_SXP *temp = (LIST_SXP *)carVal;
                    list += temp->toString();
                }

                // cdr may be nil, another List, or an atom
                if (cdrVal->type == "()") {
                    list += ")";
                    return list;
                }

                // Necessary to cast pointer into the specific S-Expression derived class before accessing data
                else if (cdrVal->type == "List") {
                    LIST_SXP *temp = (LIST_SXP *)cdrVal;
                    list += " " + temp->toString();
                }
                else if (cdrVal->type == "Number"){
                    if (((NUM_SXP *)cdrVal)->type2 == "Float") list += to_string(((NUM_SXP *)cdrVal)->doubleVal);
                    else list += to_string(((NUM_SXP *)cdrVal)->intVal);
                }
                else if (carVal->type == "Symbol"){
                    list += " " + ((SYM_SXP *)cdrVal)->symVal;
                }
                list += ")";
            }
            
            return list;
        }
};

// General Expression Class
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

// Value Expression Class: contains an S-Expression for a number or some literal value
class VALEXP: public EXP {
    public:
        S_EXP *sxp;
        VALEXP(S_EXP *sxp) {
            this->sxp = sxp;
            this->name = "valexp";
        }
};

// Variable Expression Class: contains the name of some variable
class VAREXP: public EXP {
    public:
        string varble;
        VAREXP(string varble) {
            this->varble = varble;
            this->name = "varexp";
        }
};

// String Expression Class: contains a string literal
class STREXP: public EXP {
    public:
        string strVal;

        STREXP(string strVal){
            this->name = "strexp";
            this->strVal = strVal;
        }
};

// Expression List Class: Used to store arguments for user functions or some other series of expressions
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

        // this prints the expression list
        void toString() {
            EXPLIST *cur = this;
            while (cur != NULL){
                cout << cur->head->name << endl;
                if (cur->tail != NULL && cur->tail->head != NULL){
                    cur = tail;
                }
                else break;
            }
        }

};


// Arithmetic Progression Expression: stores some math operation
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

// Value lists: used to store variable values in an environment
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

// Name lists: used to store variable names in an environment
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

// Environment Class: stores variable names and values
class ENV {
    public:
        NAMELIST *vars;
        VALUELIST *values;

        ENV(NAMELIST *vars, VALUELIST *values){
            this->vars = vars;
            this->values = values;
        }
};

// Create an empty environment
ENV *emptyEnv(){
    return new ENV(NULL, NULL);
}

// findVar: return the value of a given variable name, or null if it isn't in the environment
VALUELIST *findVar(string name, ENV *rho) {
    // pull names and values from environment
    NAMELIST *nl = rho->vars;
    VALUELIST *values = rho->values;
    bool found = false;

    // iterate through the namelist
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

// fetch: get the value of a named variable
S_EXP *fetch(string name, ENV *rho) {
    VALUELIST *vl = findVar(name, rho);
    return vl->head;
}

// bindVar: declare a new variable in environment rho
void bindVar(string name, S_EXP *s, ENV *rho) {
    rho->vars = new NAMELIST(name, rho->vars);
    rho->values = new VALUELIST(s, rho->values);
}

// assign: update a variable in environment rho
void assign(string name, S_EXP *s, ENV *rho) {
    VALUELIST *location = findVar(name, rho);
    location->head = s;
}

// lengthVL: return the length of a valuelist
int lengthVL(VALUELIST *vl){
    int len = 0;
    while (vl != NULL){
        vl = vl->tail;
        len++;
    }
    return len;
}

// lengthNL: return the length of a namelist
int lengthNL(NAMELIST *nl){
    int len = 0;
    while (nl != NULL) {
        nl = nl->tail;
        len++;
    }
    return len;
}

// LIST_EXP: For holding an expression in a list
class LIST_EXP: public S_EXP {
    public:
        // Every list has a car and cdr
        EXP* car;
        S_EXP* cdr;

        LIST_EXP(EXP *car, S_EXP *cdr) {
            this->type = "ExpList";
            this->car = car;
            this->cdr = cdr;
        }
};

#endif