#include <string>
#include <typeinfo>

using namespace std;

#ifndef _EXPR_
#define _EXPR_

//trevor
class S_EXP {
    public:
        string type;

        S_EXP(){
            type = "NIL";
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
    public:
        S_EXP sxp;
        VALEXP(S_EXP sxp) {
            this->sxp = sxp;
        }
};

class VAREXP: public EXP {
    public:
        string varble;
        VAREXP(string variable) {
            this->varble = variable;
        }
};

class EXPLIST {
    public:
        EXP head;
        EXPLIST *tail;

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

class APEXP: public EXP {
    private:
        string optr;
        EXPLIST *args;
    public:
        APEXP(string optr, EXPLIST *args) {
            this->optr = optr;
            this->args = args;
        }
};

class VALUELIST {
    private:
        
    public:
        S_EXP head;
        VALUELIST *tail;

        VALUELIST() {
            
        }

        VALUELIST(S_EXP head, VALUELIST *tail) {
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

#endif