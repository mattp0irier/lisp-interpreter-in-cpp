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