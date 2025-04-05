#ifndef arraylist_h
#define arraylist_h
#include"myList.h"
using namespace std;
#define X 10

template <class T>
class Arr:public myList<T>{
    int length;
    int maxSize;
    int initialSize;
    int pos;
    T* values;

    void resize(int cond){
        if(cond)maxSize*=2;
        else maxSize/=2;
        T* values=new T[maxSize];
        for(int i=0;i<length;i++)
            values[i]=this->values[i];
        delete[]  this->values;
        this->values=values;
    }

public:
    Arr(int maxSize=X){
        length=pos=0;
        this->maxSize=initialSize=maxSize;
        values=new T[maxSize];
    }

    Arr(Arr<T>& llist){
        length=pos=0;
        maxSize=llist.maxSize;
        initialSize=llist.initialSize;
        values=new T[maxSize];
        for(int i=0;i<llist.length;i++)
            pushBack(llist.values[i]);
        setToPos(llist.currPos());
    }
    Arr(T llist[], int y, int x=X){
        length=pos=0;
        maxSize=initialSize=x;
        values=new T[maxSize];
        for(int i=0;i<y;i++)
            pushBack(llist[i]);
        setToPos(y/2);
    }

    ~Arr(){delete[] values;}

    int size() {return length;}

    void push(T item){
        if(length==maxSize)
            resize(1);
        for(int i=length;i>pos;i--)
            values[i]=values[i-1];
        values[pos]=item;
        length++;
    }

    void pushBack(T item){
        int pos=this->pos;
        this->pos=length;
        push(item);
        this->pos=pos;
    }

    T erase(){
        if(length==0) return -1;
        T ret=values[pos];
        for(int i=pos;i<length-1;i++)
            values[i]=values[i+1];
        length--;
        if(pos==length&&pos!=0)
            pos--;
        if(length<=maxSize/2&&maxSize>initialSize)
            resize(0);
        return ret;
    }

    void setToBegin(){pos=0;}

    void setToEnd(){pos=length-1;}

    void prev(){
        if(pos!=0)
            pos--;
    }

    void next(){
        if(pos==length-1||length==0)
            return;
        pos++;
    }

    int currPos(){return pos;}

    void setToPos(int pos){
        if(pos>=0&&pos<length)
            this->pos=pos;
    }

    T getValue(){
        if(length!=0)
            return values[pos];
        else
            return -1;
    }

    int find(T item){
        int i;
        for(i=0;i<length;i++)
            if(values[i]==item)
                return i;
        return -1;
    }

    void clear(){
        delete[]values;
        length=pos=0;
        maxSize=initialSize;
        values=new T[maxSize];
    }

    template <class T2>
    friend ostream& operator<< (ostream& out,const Arr<T2>& llist);
};

template <class T>
ostream& operator<<(ostream& out,const Arr<T>& llist){
    out<<"<";
    for(int i=0;i<llist.length;i++){
        if(i==llist.pos)
            out<<"| ";
        out<<llist.values[i];
        if(i!=llist.length-1)
        out<<" ";
    }
    out<<">"<<endl;
    return out;
}
#endif //arrarlist_h
