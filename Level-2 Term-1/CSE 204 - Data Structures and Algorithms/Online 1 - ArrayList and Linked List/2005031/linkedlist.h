#ifndef linkedlist_h
#define myList_h
#include"myList.h"
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
    Node(const Node<T>& newNode){
        value=newNode.value;
        next=newNode.next;
        prev=newNode.prev;
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
    Node<T>* pos;
    Node<T>* head;
    Node<T>* tail;
    void deleteList(Node<T>* node){
        if(node->getNext()==NULL)
            return;
        deleteList(node->getNext());
        delete node;
    }
public:
    LL(int x=0){
        tail=pos=head=NULL;
        length=0;
    }
    LL(LL<T>& llist){
        length=0;
        head=tail=pos=NULL;
        for(Node<T>* item=llist.head;  item!=NULL; item=item->getNext())
            pushBack(item->getValue());
        setToPos(llist.currPos());
    }
    LL(const T llist[], int y){
        length=0;
        head=tail=pos=NULL;
        for(int i=0;i<y;i++)
            pushBack(llist[i]);
        setToPos(y/2);
    }
    ~LL(){
        if(head!=NULL){
            deleteList(head);
            delete head;
        }
    }
    int size() {
        return length;
    }
    void push(T item){
        if(find(item)!=-1)
            return;
        length++;
        Node<T>* newNode=new Node<T>(item,pos);
        if(pos==NULL){
            head=tail=pos=newNode;
            return;
        }
        newNode->setPrev(pos->getPrev());
        pos->setPrev(newNode);
        newNode->setNext(pos);
        if(pos==head)
            head=newNode;
        else
            newNode->getPrev()->setNext(newNode);
        pos=newNode;
    }
    void pushBack(T item){
        if(find(item)!=-1)
            return;
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
        T ret;
        Node<T>* item=pos;
        if(pos==NULL)
            return ret;
        length--;
        ret=pos->getValue();
        if(pos!=head)
            pos->getPrev()->setNext(pos->getNext());
        else
            head=pos->getNext();
        if(pos!=tail){
            pos->getNext()->setPrev(pos->getPrev());
            pos=pos->getNext();
        }
        else{
            tail=pos->getPrev();
            pos=pos->getPrev();
        }
        delete item;
        return ret;
    }
    void setToBegin(){
        pos=head;
    }
    void setToEnd(){
        pos=tail;
    }
    void prev(){
        if(pos!=head)
            pos=pos->getPrev();
    }
    void next(){
        if(pos!=tail)
            pos=pos->getNext();
    }
    int currPos(){
        if(head==NULL)
            return -1;
        int i;
        Node<T>* item;
        for(i=0,item=head;item!=pos;item=item->getNext(),i++);
        return i;
    }
    void setToPos(int pos){
        if(pos>=0&&pos<length){
            int i=0;
            Node<T>* item=head;
            for(;i<pos;i++,item=item->getNext());
            this->pos=item;
        }
    }
    T getValue(){
        T temp;
        if(pos!=NULL)
            return pos->getValue();
        return temp;
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
        deleteList(head);
        delete head; length=0;
        pos=tail=head=NULL;
    }
    void sorting(LL<T> llist){
        if(llist.length<2)
            return;
        Node<T>* temp1=llist.head;
        Node<T>* temp2=llist.head->getNext();
        for(;temp1->getNext()->getNext()!=NULL;temp1=temp1->getNext())
            for(temp2=temp1->getNext();temp2->getNext()!=NULL;temp2=temp2->getNext())
                if(temp1->getValue()<temp2->getValue()){
                    Node<T> tempNode(*temp1);
                    temp1.setValue(temp2.getValue());
                    temp2.setValue(tempNode.getValue());
                }

    }
    T kThLargest(int k){
        LL llist(*this);
        sorting(llist);
        if(k<0||k>=length)
            return -1;
        llist.setToPos(k);
        return llist.getValue();

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
#endif //linkedlist_h
