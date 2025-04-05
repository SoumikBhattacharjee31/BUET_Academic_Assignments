#include<bits/stdc++.h>
#include<chrono>
using namespace std;


struct Node{
    int value;
    int height;
    Node* left;
    Node* right;


    Node(int value, int height=0, Node* left=NULL, Node* right=NULL){
        this->value = value;
        this->height=height;
        this->left = left;
        this->right = right;
    }
};


class AVL_Tree{
    Node* root=NULL;


    int getheight(Node* cur){
        if(cur==NULL)
            return 0;
        int l,r;
        l=cur->left==NULL?0:cur->left->height;
        r=cur->right==NULL?0:cur->right->height;
        return max(l,r)+1;
    }


    void setheight(Node* cur){
        if(cur!=NULL)
            cur->height=getheight(cur);
    }


    int balance(Node* cur){
        if(cur==NULL)
            return 0;
        return getheight(cur->left)-getheight(cur->right);
    }


    Node* setLeftMax(Node* cur){
        if(cur==NULL)
            return cur;
        else if(cur->right==NULL)
            return cur;
        else
            return setLeftMax(cur->right);
    }


    Node* setRightMin(Node* cur){
        if(cur==NULL)
            return cur;
        else if(cur->left==NULL)
            return cur;
        else
            return setRightMin(cur->left);
    }


    Node* LLrotation(Node* cur){
        Node* cur1=cur;
        Node* cur2=cur->left;
        Node* cur3=cur->left->left;
        cur1->left=cur2->right;
        cur2->right=cur1;
        setheight(cur1);
        setheight(cur2);
        return cur2;
    }


    Node* LRrotation(Node* cur){
        Node* cur1=cur;
        Node* cur2=cur->left;
        Node* cur3=cur->left->right;
        cur1->left=cur3->right;
        cur2->right=cur3->left;
        cur3->left=cur2;
        cur3->right=cur1;
        setheight(cur1);
        setheight(cur2);
        setheight(cur3);
        return  cur3;
    }


    Node* RRrotation(Node* cur){
        Node* cur1=cur;
        Node* cur2=cur->right;
        Node* cur3=cur->right->right;
        cur1->right=cur2->left;
        cur2->left=cur1;
        setheight(cur1);
        setheight(cur2);
        return cur2;
    }


    Node* RLrotation(Node* cur){
        Node* cur1=cur;
        Node* cur2=cur->right;
        Node* cur3=cur->right->left;
        cur1->right=cur3->left;
        cur2->left=cur3->right;
        cur3->right=cur2;
        cur3->left=cur1;
        setheight(cur1);
        setheight(cur2);
        setheight(cur3);
        return cur3;
    }


    Node* rotate(Node* cur){
        if(balance(cur)>1 && balance(cur->left)>=0)
            cur=LLrotation(cur);
        else if(balance(cur)>1 && balance(cur->left)<0)
            cur=LRrotation(cur);
        else if(balance(cur)<-1 && balance(cur->right)<=0)
            cur=RRrotation(cur);
        else if(balance(cur)<-1 && balance(cur->right)>0)
            cur=RLrotation(cur);
        return cur;
    }


    Node* setNode(int value, Node* cur){
        if(cur==NULL)
            return new Node(value,1);
        else if(cur->value>value)
            cur->left=setNode(value,cur->left);
        else if(cur->value<value)
            cur->right=setNode(value,cur->right);
        else if(cur->value==value)
            return cur;
        setheight(cur);
        cur=rotate(cur);
        return cur;
    }
    

    Node* removeNode(int value, Node* cur){ 
        if(cur==NULL)
            return NULL;
        else if(cur->value>value)
            cur->left=removeNode(value,cur->left);
        else if(cur->value<value)
            cur->right=removeNode(value,cur->right);
        else if(cur->value==value){
            Node* temp;
            if(cur->left==NULL)
                temp=cur->right;
            else if(cur->right==NULL)
                temp=cur->left;
            else{
                // int tempval=setLeftMax(cur->left)->value;
                // cur->left=removeNode(tempval,cur->left);
                int tempval=setRightMin(cur->right)->value;
                cur->right=removeNode(tempval,cur->right);
                cur->value=tempval;
                setheight(cur);
                cur=rotate(cur);
                return cur;
            }
            delete cur;
            cur=temp;
        }
        setheight(cur);
        cur=rotate(cur);
        return cur;
    }


    bool searchNode(int value, Node* cur){
        if(cur==NULL)
            return false;
        else if(cur->value>value)
            return searchNode(value,cur->left);
        else if(cur->value<value)
            return searchNode(value,cur->right);
        else
            return true;
    }


    string intraverse(Node* cur){
        if(cur==NULL)
            return "";
        string s="";
        s+=intraverse(cur->left);
        s+=to_string(cur->value)+" ";
        s+=intraverse(cur->right);
        return s;
    }


    string print(Node* cur){
        if(cur==NULL)
            return "";
        string m=to_string(cur->value);
        string l=print(cur->left);
        string r=print(cur->right);
        if(l!=""||r!="")
            return m+"("+l+","+r+")";
        return m;
    }


    public:
    AVL_Tree insertNode(int value){ 
        root=setNode(value,root);
        return *this;
    }


    AVL_Tree deleteNode(int value){
        root=removeNode(value,root);
        return *this;
    }


    bool findNode(int value){
        return searchNode(value,root);
    }


    string traverseNode(){
        return intraverse(root);
    }


    friend ostream& operator << (ostream& out, AVL_Tree tree);
};


ostream& operator << (ostream& out, AVL_Tree tree){
    out<<tree.print(tree.root)<<endl;
    return out;
}


int main(){
    auto start=chrono::high_resolution_clock::now();
    ifstream in("in.txt");
    ofstream out("out_avl.txt");
    ofstream out2("report_avl.txt");
    double itime=0, dtime=0, ftime=0, ttime=0;
    AVL_Tree tree;
    
    while(!in.eof()){
        char operation;
        int value;
        in>>operation;
        if(in.eof())
            break;

        switch(operation){
            case 'I':{
                in>>value;
                auto istart=chrono::high_resolution_clock::now();
                out<<tree.insertNode(value);
                auto iend=chrono::high_resolution_clock::now();
                auto iduration=chrono::duration_cast<chrono::nanoseconds>(iend-istart).count();
                itime+=iduration/1000000.0;
                break;
            }

            case 'D':{
                auto istart=chrono::high_resolution_clock::now();
                in>>value;
                out<<tree.deleteNode(value);
                auto iend=chrono::high_resolution_clock::now();
                auto iduration=chrono::duration_cast<chrono::nanoseconds>(iend-istart).count();
                dtime+=iduration/1000000.0;
                break;
            }

            case 'F':{
                in>>value;
                auto istart=chrono::high_resolution_clock::now();
                out<<(tree.findNode(value)?"found":"not found")<<endl;
                auto iend=chrono::high_resolution_clock::now();
                auto iduration=chrono::duration_cast<chrono::nanoseconds>(iend-istart).count();
                ftime+=iduration/1000000.0;
                break;
            }

            case 'T':{
                auto istart=chrono::high_resolution_clock::now();
                out<<tree.traverseNode()<<endl;
                auto iend=chrono::high_resolution_clock::now();
                auto iduration=chrono::duration_cast<chrono::nanoseconds>(iend-istart).count();
                ttime+=iduration/1000000.0;
                break;
            }
        }
    }

    auto end=chrono::high_resolution_clock::now();
    double duration=chrono::duration_cast<chrono::nanoseconds>(end-start).count()/1000000.0;

    out2<<"operation time(ms)"<<endl;
    out2<<"insert "<<itime<<endl;
    out2<<"delete "<<dtime<<endl;
    out2<<"search "<<ftime<<endl;
    out2<<"trav "<<ttime<<endl;
    // out2<<"total "<<duration<<endl;
    out2<<"total "<<(itime+dtime+ftime+ttime)<<endl;
    return 0;
}