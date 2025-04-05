#ifndef myStack_hpp
#define myStack_hpp
#include "arraylist.hpp"
#include "linkedlist.hpp"
using namespace std;
template<class T>
class myStk{
    Arr<T>* llist;
    public:
    myStk(int x=X){
        llist=new Arr<T>(x);
    }
    void clear(){
        llist->clear();
    }
    void push(T item){
        llist->pushBack(item);
    }
    T pop(){
        llist->setToEnd();
        return llist->erase();
    }
    T length(){
        return llist->size();
    }
    T topValue(){
        llist->setToEnd();
        return llist->getValue();
    }
    template <class T2>
    friend ostream& operator<<(ostream& out,const myStk<T2>& stk);
};
template <class T>
ostream& operator<<(ostream& out,const myStk<T>& stk){
    stk.llist->setToBegin();
    out<<*stk.llist;
    return out;
}
# endif
