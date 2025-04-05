#ifndef myList_h
#define myList_h
template <class T>
class myList{
public:
    virtual int size()=0;
    virtual void push(T)=0;
    virtual void pushBack(T)=0;
    virtual T erase()=0;
    virtual void setToBegin()=0;
    virtual void setToEnd()=0;
    virtual void prev()=0;
    virtual void next()=0;
    virtual int currPos()=0;
    virtual void setToPos(int)=0;
    virtual T getValue()=0;
    virtual int find(T)=0;
    virtual void clear()=0;
};
#endif // myList_h
