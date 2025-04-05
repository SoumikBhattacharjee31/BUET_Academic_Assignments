#ifndef lrucache_h
#define lrucache_h
#include "arraylist.h"
#include"linkedlist.h"
using namespace std;


template<class T1,class T2>
class LRUCache{public:
    int Capacity;
    myList<T1>* keys;
    myList<T2>* values;
public:
    LRUCache(int Capacity){
        this->Capacity=Capacity;
        keys=new Arr<T1>(Capacity);
        values=new Arr<T2>(Capacity);
    }
    ~LRUCache(){
        delete keys;
        delete values;
    }
    T2 get(T1 key){
        int pos=keys->find(key);
        if(pos==-1) return -1;
        values->setToPos(pos);
        keys->setToPos(pos);
        T1 k=keys->getValue();
        T2 v=values->getValue();
        keys->erase();
        values->erase();
        keys->pushBack(k);
        values->pushBack(v);
        return v;
    }
    void put(T1 key, T2 value){
        int pos=keys->find(key);
        if(pos!=-1){
            values->setToPos(pos);
            values->erase();
            values->pushBack(value);
            keys->setToPos(pos);
            keys->erase();
            keys->pushBack(key);
        }
        else if(keys->size()==Capacity){
            keys->setToBegin();
            keys->erase();
            keys->pushBack(key);
            values->setToBegin();
            values->erase();
            values->pushBack(key);
        }
        else{
            keys->pushBack(key);
            values->pushBack(value);
        }
    }
};

#endif //lrucache_h
