#ifndef _CLASSES_
#define _CLASSES_

#include <vector>

template <typename T>
class atom {
    public:
        atom () { value = (T)NULL; }
        atom (T val) { value = val; }
        T data { return value; }
    private:
        T value = (T)NULL;
};

template <typename T>
class sExpr {
    public:
        
    private:
};

template <typename T>
class consCell {
    public:
         consCell<T>(T val, consCell<T> *rest) { car = new atom<T>(val); cdr = rest; }

         ~consCell<T>() { delete car; }

    private:
        atom<T> *car;
        consCell<T> *cdr = NULL;
};










#endif