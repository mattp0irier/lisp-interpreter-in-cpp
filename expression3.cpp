//trevor

//matt

class EXP {
};

class VALEXP: public EXP {
    SEXP sxp;
    VALEXP(SEXP sxp) {
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
        EXPLIST(EXP head, EXPLIST *tail) {
            this->head = head;
            this->tail = tail;
        }
};