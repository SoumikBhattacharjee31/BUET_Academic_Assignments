#ifndef myQueue_hpp
#define myQueue_hpp
#include "arraylist.hpp"
#include "linkedlist.hpp"
using namespace std;
template<class T>
class myQiu{
    LL<T>* llist;
    public:
    myQiu(int x=X){
        llist=new LL<T>(x);
    }
    void clear(){
        llist->clear();
    }
    void enqueue(T item){
        llist->pushBack(item);
    }
    T dequeue(){
        llist->setToBegin();
        return llist->erase();
    }
    T length(){
        return llist->size();
    }
    T frontValue(){
        llist->setToBegin();
        return llist->getValue();
    }
    T rearValue(){
        llist->setToEnd();
        return llist->getValue();
    }
    T leaveQueue(){
        llist->setToEnd();
        return llist->erase();
    }
    template <class T2>
    friend ostream& operator<< (ostream& out,const myQiu<T2>& qiu);
};
template <class T>
ostream& operator<<(ostream& out,const myQiu<T>& qiu){
    qiu.llist->setToBegin();
    out<<*qiu.llist;
    return out;
}
#endif
