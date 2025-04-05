#include<fstream>
#include "linkedlist.hpp"
using namespace std;


template<class T>
class TreeNode{
    T value;
    TreeNode<T>* left;
    TreeNode<T>* right;

public:
    TreeNode(T value=-1, TreeNode<T>*left=NULL, TreeNode<T>*right=NULL){
        this->value=value;
        this->left=left;
        this->right=right;
    }

    void setValue(T value){
        this->value=value;
    }

    void setLeft(TreeNode<T>* left){
        this->left=left;
    }

    void setRight(TreeNode<T>* right){
        this->right=right;
    }

    T getValue(){
        return value;
    }

    TreeNode<T>* getLeft(){
        return left;
    }

    TreeNode<T>* getRight(){
        return right;
    }
};


template<class T>
class BST{
    TreeNode<T>* head;
    bool invalidDelete;
    void pre(TreeNode<T>* node,LL<T>& llist)const{
        if(node==NULL)
            return;
        llist.pushBack(node->getValue());
        pre(node->getLeft(),llist);
        pre(node->getRight(),llist);
    }

    void in(TreeNode<T>* node,LL<T>& llist)const{
        if(node==NULL)
            return;
        in(node->getLeft(),llist);
        llist.pushBack(node->getValue());
        in(node->getRight(),llist);
    }

    void post(TreeNode<T>* node,LL<T>& llist)const{
        if(node==NULL)
            return;
        post(node->getLeft(),llist);
        post(node->getRight(),llist);
        llist.pushBack(node->getValue());
    }

    void cpyInsert(T value){
        TreeNode<T>* node=new TreeNode<T>(value);
        if(head==NULL)
            head=node;
        else{
            TreeNode<T>*temp=head;
            while(true){
                if(temp->getValue()>value){
                    if(temp->getLeft()==NULL){
                        temp->setLeft(node);
                        break;
                    }
                    else
                        temp=temp->getLeft();
                }
                else if(temp->getValue()<value){
                    if(temp->getRight()==NULL){
                        temp->setRight(node);
                        break;
                    }
                    else
                        temp=temp->getRight();
                }
            }
        }
    }

    void recDelete(TreeNode<T>* node){
        if(node==NULL)
            return;
        if(node->getLeft()!=NULL)
            recDelete(node->getLeft());
        if(node->getRight()!=NULL)
            recDelete(node->getRight());
    }

public:
    BST(){
        invalidDelete=true;
        head=NULL;
    }

    BST(const BST<T>& bst){
        invalidDelete=bst.invalidDelete;
        head=NULL;
        LL<T> llist;
        bst.pre(bst.head,llist);
        llist.setToBegin();
        for(int i=0;i<llist.size();i++,llist.next())
            cpyInsert(llist.getValue());
    }

    ~BST(){
         recDelete(head);
         if(head!=NULL)
             delete head;
    }

    BST<T> Insert(T value){
        TreeNode<T>* node=new TreeNode<T>(value);
        if(head==NULL)
            head=node;
        else{
            TreeNode<T>*temp=head;
            while(true){
                if(temp->getValue()>value){
                    if(temp->getLeft()==NULL){
                        temp->setLeft(node);
                        break;
                    }
                    else
                        temp=temp->getLeft();
                }
                else if(temp->getValue()<value){
                    if(temp->getRight()==NULL){
                        temp->setRight(node);
                        break;
                    }
                    else
                        temp=temp->getRight();
                }
            }
        }
        return *this;
    }

    BST<T> Delete(T value){
        invalidDelete=true;
        TreeNode<T>*temp=head;
        TreeNode<T>*tempP=NULL;
        char pos='l';
        if(temp==NULL){
            invalidDelete=false;
            return *this;
        }
        if(temp->getValue()==value)
            if(temp->getRight()==NULL&&temp->getLeft()==NULL){
                delete head;
                head=NULL;
                return *this;
            }
            else{
                invalidDelete=false;
                return *this;
            }
        while(true){
            if(temp->getValue()==value)
                if(temp->getRight()==NULL&&temp->getLeft()==NULL){
                    delete temp;
                    if(pos=='l')
                        tempP->setLeft(NULL);
                    else
                        tempP->setRight(NULL);
                    break;
                }
                else{
                    invalidDelete=false;
                    break;
                }
            else if(temp->getValue()>value)
                if(temp->getLeft()==NULL){
                    invalidDelete=false;
                    break;
                }
                else{
                    tempP=temp;
                    pos='l';
                    temp=temp->getLeft();
                }
            else
                if(temp->getRight()==NULL){
                    invalidDelete=false;
                    break;
                }
                else{
                    tempP=temp;
                    pos='r';
                    temp=temp->getRight();
                }
        }
        return *this;
    }

    bool Find(T value){
        TreeNode<T>* node=head;
        if(head==NULL)
            return false;
        TreeNode<T>*temp=head;
        while(true){
            if(temp->getValue()==value)
                return true;
            else if(temp->getValue()>value)
                if(temp->getLeft()==NULL)
                    return false;
                else
                    temp=temp->getLeft();
            else if(temp->getValue()<value)
                if(temp->getRight()==NULL)
                    return false;
                else
                    temp=temp->getRight();
        }
    }

    LL<T> Traversal(char c){
        LL<T> llist;
        switch(c){
            case 'n':{
                in(head,llist);
                break;
            }
            case 'r':{
                pre(head,llist);
                break;
            }
            case 'o':{
                post(head,llist);
                break;
            }
        }
        return llist;
    }
    template <class T2>
    friend ostream& operator<< (ostream& out,  BST<T2> bst);
};


template <class T>
ostream& operator<< (ostream& out,  BST<T> bst){
    if(!bst.invalidDelete){
        bst.invalidDelete=true;
        return out<<"Invalid Operation"<<endl;
    }
    return recprint(out,bst.head,false,bst.head);
}

template <class T>
ostream& recprint(ostream& out,TreeNode<T>* node, bool empty,TreeNode<T>* head){
    if(node==NULL){
        if(empty)
            out<<"()";
        return out;
    }
    if(node!=head)
        out<<"(";
    out<<node->getValue();
    if(node->getLeft()==NULL&&node->getRight()!=NULL)
        recprint(out,node->getLeft(),true,head);
    else
        recprint(out,node->getLeft(),false,head);
    if(node->getLeft()!=NULL&&node->getRight()==NULL)
        recprint(out,node->getRight(),true,head);
    else
        recprint(out,node->getRight(),false,head);
    if(node!=head)
        out<<")";
    else
        out<<endl;
    return out;
}


int main(){
    ifstream in;
    ofstream out;
    in.open("input.txt");
    out.open("output.txt");
    BST<int> bst;
    bool cond=true;
    while(cond&&!in.eof()){
        int n;
        char c;
        char str[4];
        in>>c;
        switch(c){
            case 'I':{
                in>>n;
                out<<bst.Insert(n);
                break;
            }
            case 'F':{
                in>>n;
                out<<(bst.Find(n)?"True":"False")<<endl;
                break;
            }
            case 'T':{
                in>>str;
                out<<bst.Traversal(str[1]);
                break;
            }
            case 'D':{
                in>>n;
                out<<bst.Delete(n);
                break;
            }
            default:{
                cond=false;
            }
        }
    }
    in.close();
    out.close();
    return 0;
}
