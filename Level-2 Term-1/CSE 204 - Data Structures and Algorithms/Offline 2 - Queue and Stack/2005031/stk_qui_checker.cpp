#include<fstream>
#include<iostream>
#include "arraylist.hpp"
#include "linkedlist.hpp"
#include "myQueue.hpp"
#include "myStack.hpp"
using namespace std;
int main(){
    ifstream in;
    ofstream out;
    in.open("input.txt");
    out.open("output.txt");
    int k,x;
    in>>k>>x;
    myQiu<int> qiu(x);
    for(int i=0;i<k;i++){
        int a;
        in>>a;
        qiu.enqueue(a);
    }

    int m,y;
    in>>m>>y;
    myStk<int> stk(y);
    for(int i=0;i<m;i++){
        int a;
        in>>a;
        stk.push(a);
    }

    int q;
    in>>q;
    for(int i=0;i<q;i++){
        int f,p,temp=-1;
        in>>f>>p;
        switch(f){
            case 1:{
                qiu.clear();
                break;
            }
            case 2:{
                qiu.enqueue(p);
                break;
            }
            case 3:{
                temp=qiu.dequeue();
                break;
            }
            case 4:{
                temp=qiu.length();
                break;
            }
            case 5:{
                temp=qiu.frontValue();
                break;
            }
            case 6:{
                temp=qiu.rearValue();
                break;
            }
            case 7:{
                temp=qiu.leaveQueue();
                break;
            }
            case 8:{
                stk.clear();
                break;
            }
            case 9:{
                stk.push(p);
                break;
            }
            case 10:{
                temp=stk.pop();
                break;
            }
            case 11:{
                temp=stk.length();
                break;
            }
            case 12:{
                temp=stk.topValue();
                break;
            }

        }
        out<<qiu;
        out<<stk;
        out<<temp<<endl;
    }
    in.close();
    out.close();
}
