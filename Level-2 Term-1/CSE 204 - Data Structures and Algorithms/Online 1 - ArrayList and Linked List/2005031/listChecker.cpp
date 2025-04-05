#include<fstream>
#include"arraylist.h"
#include"linkedlist.h"
int main(){
    ifstream in;
    ofstream out;
    in.open("list_input.txt");
    out.open("list_output.txt");
    int k,x;
    in>>k>>x;
    int initialArray[k];
    for(int i=0;i<k;i++)
        in>>initialArray[i];
    LL<int> llist(initialArray,k);
    int q;
    in>>q;
    out<<llist;
    for(int i=0;i<q;i++){
        int f,p;
        in>>f>>p;
        switch(f){
            case 1:{
                out<<llist<<llist.size()<<endl;
                break;
            }
            case 2:{
                llist.push(p);
                out<<llist<<-1<<endl;
                break;
            }
            case 3:{
                llist.pushBack(p);
                out<<llist<<-1<<endl;
                break;
            }
            case 4:{
                int temp=llist.erase();
                out<<llist<<temp<<endl;
                break;
            }
            case 5:{
                llist.setToBegin();
                out<<llist<<-1<<endl;
                break;
            }
            case 6:{
                llist.setToEnd();
                out<<llist<<-1<<endl;
                break;
            }
            case 7:{
                llist.prev();
                out<<llist<<-1<<endl;
                break;
            }
            case 8:{
                llist.next();
                out<<llist<<-1<<endl;
                break;
            }
            case 9:{
                out<<llist<<llist.currPos()<<endl;
                break;
            }
            case 10:{
                llist.setToPos(p);
                out<<llist<<-1<<endl;
                break;
            }
            case 11:{
                out<<llist<<llist.getValue()<<endl;
                break;
            }
            case 12:{
                out<<llist<<llist.find(p)<<endl;
                break;
            }
            case 13:{
                llist.clear();
                out<<llist<<-1<<endl;
                break;
            }
        }
    }
    in.close();
    out.close();
    return 0;
}
