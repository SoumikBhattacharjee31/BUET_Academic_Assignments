#ifndef linkedlist_hpp
#define linkedlist_hpp
#include"myList.hpp"
using namespace std;
template <class T>
class Node{
    T value;
    Node<T>* next;
    Node<T>* prev;
public:
    Node(T value=-1, Node<T>* next=NULL, Node<T>*prev=NULL){
        this->value=value;
        this->next=next;
        this->prev=prev;
    }
    void setValue(T value){
        this->value=value;
    }
    T getValue(){
        return value;
    }
    void setNext(Node<T>* next){
        this->next=next;
    }
    Node<T>* getNext(){
        return next;
    }
    void setPrev(Node<T>* prev){
        this->prev=prev;
    }
    Node<T>* getPrev(){
        return prev;
    }
};


template <class T>
class LL:public myList<T>{
    int length;
    int curpos;
    Node<T>* pos;
    Node<T>* head;
    Node<T>* tail;
public:
    LL(int x=0){
        tail=pos=head=NULL;
        length=curpos=0;
    }
    LL(LL<T>& llist){
        length=curpos=0;
        head=tail=pos=NULL;
        for(Node<T>* item=llist.head;  item!=NULL; item=item->getNext())
            pushBack(item->getValue());
        setToPos(llist.currPos());
    }
    LL(const T llist[], int y){
        length=curpos=0;
        head=tail=pos=NULL;
        for(int i=0;i<y;i++)
            pushBack(llist[i]);
        setToPos(y/2);
    }
    ~LL(){
        clear();
    }
    int size() {
        return length;
    }
    void push(T item){
        length++;
        Node<T>* newNode=new Node<T>(item,pos);
        if(pos==NULL){
            head=tail=pos=newNode;
            return;
        }
        newNode->setPrev(pos->getPrev());
        pos->setPrev(newNode);
        if(pos==head)
            head=newNode;
        else
            newNode->getPrev()->setNext(newNode);
        pos=newNode;
    }
    void pushBack(T item){
        length++;
        Node<T>* newNode=new Node<T>(item,NULL,tail);
        if(tail==NULL)
            head=tail=pos=newNode;
        else{
            tail->setNext(newNode);
            tail=newNode;
        }
    }
    T erase(){
        Node<T>* item=pos;
        if(length==1){
            T temp=getValue();
            clear();
            return temp;
        }
        if(length<=0)head=tail=pos=NULL;
        if(pos==NULL)
            return -1;
        length--;
        T ret=pos->getValue();
        if(pos!=head)
            pos->getPrev()->setNext(pos->getNext());
        else{
            head=pos->getNext();
            head->setPrev(NULL);
        }
        if(pos!=tail){
            pos->getNext()->setPrev(pos->getPrev());
            pos=pos->getNext();
        }
        else{
            pos=tail=pos->getPrev();
            tail->setNext(NULL);
            curpos--;
        }
        delete item;
        if(length<=0)
            head=tail=pos=NULL;
        return ret;
    }
    void setToBegin(){
        pos=head;
        curpos=0;
    }
    void setToEnd(){
        pos=tail;
        curpos=length-1;
        if(curpos<0)
            curpos=0;
    }
    void prev(){
        if(pos!=head){
            pos=pos->getPrev();
            curpos--;
        }
    }
    void next(){
        if(pos!=tail){
            pos=pos->getNext();
            curpos++;
        }
    }
    int currPos(){
        if(head==NULL)
            return -1;
        return curpos;
    }
    void setToPos(int pos){
        if(pos>=0&&pos<length){
            curpos=pos;
            int i=0;
            Node<T>* item=head;
            for(;i<pos;i++,item=item->getNext());
            this->pos=item;
        }
    }
    T getValue(){
        if(pos!=NULL)
            return pos->getValue();
        return -1;
    }
    int find(T it){
        int i=0;
        Node<T>* item=head;
        for(;item!=NULL;i++,item=item->getNext()){
            if(item->getValue()==it)return i;
        }
        return -1;
    }
    void clear(){
        Node<T>* temp=head;
        while(temp!=NULL){
            head=head->getNext();
            delete temp;
            temp=head;
        }
        length=curpos=0;
        head=tail=pos=NULL;
    }
    template <class T2>
    friend ostream& operator<< (ostream& out, const LL<T2>& llist);
};
template <class T>
ostream& operator<<(ostream& out, const LL<T>& llist){
    out<<"<";
    for(Node<T>* item=llist.head;item!=NULL;item=item->getNext()){
        if(llist.pos==item)
            out<<"| ";
        out<<item->getValue();
        if(item->getNext()!=NULL)
            out<<" ";
    }
    out<<">"<<endl;
    return out;
}
#endif //linkedlist_hpp
