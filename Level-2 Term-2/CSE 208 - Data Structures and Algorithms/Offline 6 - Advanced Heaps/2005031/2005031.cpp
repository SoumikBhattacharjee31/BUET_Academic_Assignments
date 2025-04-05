#include <bits/stdc++.h>
using namespace std;

struct Node{
    int value;
    Node *leftchild;
    Node *rightsibling;
    int order;
    Node(int value, Node *leftchild = NULL, Node *rightsibling = NULL){
        this->value = value;
        this->leftchild = leftchild;
        this->rightsibling = rightsibling;
        this->order = 0;
    }
};

struct BinomialHeap{
    Node *bheap = NULL;
    void heapunion(BinomialHeap& heap2, BinomialHeap& heap1){
        if(heap1.bheap==NULL){
            return;
        }
        if(heap2.bheap==NULL){
            heap2.bheap=heap1.bheap;
            return;
        }
        Node *cur1=heap1.bheap, *cur2=heap2.bheap, *prev1=NULL;
        while(cur2!=NULL){
            for(;cur1!=NULL&&cur1->order<cur2->order;prev1=cur1,cur1=cur1->rightsibling);
            if(cur1==NULL){
                prev1->rightsibling=cur2;
                break;
            }
            else if(cur1->order>cur2->order){
                Node* temp=cur2->rightsibling;
                if(prev1!=NULL)
                    prev1->rightsibling=cur2;
                else
                    heap1.bheap=cur2;
                cur2->rightsibling=cur1;
                prev1=cur2;
                cur2=temp;
            }
            else{
                Node* temp=cur2->rightsibling;
                if(prev1!=NULL)
                    prev1->rightsibling=cur2;
                else
                    heap1.bheap=cur2;
                cur2->rightsibling=cur1;
                cur1=cur2;
                cur2=temp;
                while (cur1->rightsibling!=NULL&&cur1->order == cur1->rightsibling->order){
                    if (cur1->value < cur1->rightsibling->value){
                        Node *temp = cur1->rightsibling->rightsibling;
                        Node *temp2 = cur1->rightsibling;
                        if(prev1!=NULL)
                            prev1->rightsibling=cur1->rightsibling;
                        else
                            heap1.bheap=cur1->rightsibling;
                        cur1->rightsibling->rightsibling = cur1;
                        cur1->rightsibling = temp;
                        cur1=temp2;
                    }
                    Node *temp = cur1->rightsibling->leftchild;
                    Node *temp2 = cur1->rightsibling;
                    cur1->rightsibling->leftchild = cur1;
                    cur1->rightsibling = temp;
                    cur1 = temp2;
                    if(prev1!=NULL)
                        prev1->rightsibling=cur1;
                    else
                        heap1.bheap=cur1;
                    cur1->order++;
                }
            }
        }
        heap2.bheap=heap1.bheap;
    }

    void heapunion2(BinomialHeap& heap1, BinomialHeap& heap2){
        BinomialHeap heap;
        Node* cur1=heap1.bheap, *cur2=heap2.bheap, *cur=NULL;
        while(cur1!=NULL&&cur2!=NULL)
            if(cur1==NULL){
                if(heap.bheap==NULL){
                    heap.bheap=cur=cur2;
                    break;
                }
                else{
                    cur->rightsibling=cur2;
                    break;
                }
            }
            else if(cur2==NULL){
                if(heap.bheap==NULL){
                    heap.bheap=cur=cur1;
                    break;
                }
                else{
                    cur->rightsibling=cur1;
                    break;
                }
            }
            else{
                if(cur1->order>=cur2->order){
                    if(heap.bheap==NULL)
                        heap.bheap=cur=cur2;
                    else
                        cur->rightsibling=cur2;
                    cur=cur2;
                    cur2=cur2->rightsibling;
                    cur->rightsibling=NULL;
                }
                else if(cur2->order>=cur1->order){
                    if(heap.bheap==NULL)
                        heap.bheap=cur=cur1;
                    else
                        cur->rightsibling=cur1;
                    cur=cur1;
                    cur1=cur1->rightsibling;
                    cur->rightsibling=NULL;
                }
            }
        cur=heap.bheap;
        Node* prev=NULL;
        while(cur!=NULL&&cur->rightsibling!=NULL){
            if(cur->order==cur->rightsibling->order){
                if(cur->rightsibling->rightsibling!=NULL&&cur->rightsibling->rightsibling->order==cur->order){
                    prev=cur;
                    cur=cur->rightsibling;
                }
                else{
                    Node* next=cur->rightsibling;
                    if(prev==NULL)
                        heap.bheap=next;
                    else
                        prev->rightsibling=next;
                    cur->rightsibling=next->leftchild;
                    next->leftchild=cur;
                    cur=next;
                }
            }
        }
        heap1.bheap=heap.bheap;
    }

    void insert(int value){
        Node *newnode = new Node(value);
        BinomialHeap heap;
        heap.bheap=newnode;
        heapunion(*this,heap);
    }

    Node* findmin(){
        if (bheap==NULL)
            return NULL;
        Node *cur = bheap;
        Node *minnode = NULL;
        int min = INT_MAX;
        for (; cur != NULL; cur = cur->rightsibling)
            if (min > cur->value){
                min = cur->value;
                minnode=cur;
            }
        return minnode;
    }

    int extractmin(){
        if (bheap==NULL)
            return INT_MAX;
        Node* min = findmin();
        int val=min->value;
        Node *cur = bheap, *prev=NULL;
        for (; cur != NULL; prev=cur, cur = cur->rightsibling)
            if (min == cur){
                BinomialHeap heap;
                heap.bheap=cur->leftchild;
                if(prev!=NULL)
                    prev->rightsibling=cur->rightsibling;
                else
                    bheap=cur->rightsibling;
                delete cur;
                vector<Node*>nodelist;
                for(Node* node=heap.bheap;node!=NULL;node=node->rightsibling)
                    nodelist.push_back(node);
                for(int i=nodelist.size()-1;i>0;i--)
                    nodelist[i]->rightsibling=nodelist[i-1];
                if(nodelist.size()!=0){
                    nodelist[0]->rightsibling=NULL;
                    heap.bheap=(nodelist[nodelist.size()-1]);
                }
                else
                    heap.bheap=NULL;
                heapunion(*this,heap);
                break;
            }
        return val;
    }

    void newunion(vector<int> v){
        static int i=0;
        BinomialHeap newheap;
        for(auto x:v){
            newheap.insert(x);
        }
        cout<<newheap;
        i++;
        heapunion(*this,newheap);
    }
    friend ostream& operator<<(ostream& out,BinomialHeap& heap);
};

// ostream& operator<<(ostream& out,BinomialHeap& heap){
//     out<<"Printing Binomial Heap...\n";
//         for(Node* cur=heap.bheap;cur!=NULL;cur=cur->rightsibling){
//             out<<"Binomial Tree, B"<<cur->order<<endl;
//             int level=0;
//             for(Node* cur2=cur;cur2!=NULL;cur2=cur2->leftchild,level++){
//                 out<<"Level "<<level<<" : ";
//                 for(Node* cur3=cur2;cur3!=NULL;cur3=cur3->rightsibling){
//                     out<<cur3->value<<" ";
//                     if(cur==cur3)
//                         break;
//                 }
//                 out<<endl;
//             }
//         }
//     return out;
// }
ostream& operator<<(ostream& out,BinomialHeap& heap){
    out<<"Printing Binomial Heap...\n";
        for(Node* cur=heap.bheap;cur!=NULL;cur=cur->rightsibling){
            out<<"Binomial Tree, B"<<cur->order<<endl;
            Node* leftnode=cur;
            int level=0;
            queue<Node*> q;
            q.push(cur);
            while(q.size()){
                Node* cur2=q.front();
                q.pop();
                if(cur2==leftnode){
                    out<<"Level "<<level<<" : ";
                    level++;
                    leftnode=leftnode->leftchild;
                }
                out<<cur2->value<<" ";
                for(Node* cur3=cur2->leftchild;cur3!=NULL;cur3=cur3->rightsibling){
                    q.push(cur3);
                }
                if(q.front()==leftnode||q.empty())
                    out<<endl;
            }
        }
    return out;
}

void choice(BinomialHeap &heap, char operation, ifstream &in, ofstream &out){
    int value;
    switch(operation){
            case 'I':{
                in>>value;
                heap.insert(value);
                break;
            }

            case 'E':{
                out<<"Extract-Min returned "<<heap.extractmin()<<endl;
                break;
            }

            case 'F':{
                out<<"Find-Min returned "<<heap.findmin()->value<<endl;;
                break;
            }

            case 'P':{
                out<<heap;
                break;
            }

            case 'U':{
                vector<int> v;
                string s="";
                char charfound=0;
                while(in>>s){
                    if(s=="I"||s=="E"||s=="F"||s=="P"||s=="U"){
                        charfound=s[0];
                        break;
                    }
                    v.push_back(stoi(s));
                }
                heap.newunion(v);
                if(charfound!=0)
                    choice(heap,charfound,in,out);
                break;
            }
        }
}

int main(){
    // ifstream in("in.txt");
    ifstream in("in.txt");
    ofstream out("out.txt");
    BinomialHeap heap;
    bool ubef=true;
    char operation;
    int value;
    
    while(!in.eof()){
        in>>operation;
        cout<<operation;
        if(!in.eof())
            choice(heap,operation,in,out);
    }
    return 0;
}
