#include<fstream>
#include<iostream>
#include "myStack.hpp"
#include "myQueue.hpp"
using namespace std;
#define MAX_PLAY 4
int main(){
    ifstream in;
    ofstream out;
    in.open("consolein.txt");
    out.open("consoleout.txt");
    int n,c;
    in>>n>>c;
    int stp[n],dur[n],id[n];
    for(int i=0;i<n;i++){
        int s,d,j;
        in>>s>>d;
        for(j=i;j>0&&stp[j-1]>s;j--){
            stp[j]=stp[j-1];
            dur[j]=dur[j-1];
            id[j]=id[j-1];
        }
        stp[j]=s;
        dur[j]=d;
        id[j]=i+1;
    }
    myQiu<int> qiustp,qiudur,qiuid,ansstp,ansdur,ansid;
    myStk<int> stkstp,stkdur,stkid;
    int curtime=0,stkcond=1,curtime2=0;
    for(int i=0;i<n;i++){cout<<endl<<i<<" "<<curtime<<" "<<curtime2<<endl<<qiustp<<qiudur<<qiuid<<stkstp<<stkdur<<stkid<<ansstp<<ansdur<<ansid;
        while(qiustp.length()>0&&qiustp.frontValue()+qiudur.frontValue()<=stp[i]&&curtime+qiudur.frontValue()<=stp[i]){
            cout<<"k";
            if(curtime<qiustp.frontValue())
                curtime=qiustp.frontValue();
            ansstp.enqueue(curtime);
            curtime+=qiudur.frontValue();
            ansdur.enqueue(qiudur.dequeue());
            ansid.enqueue(qiuid.dequeue());
            qiustp.dequeue();
        }
        while(stkstp.length()>0&&stkstp.topValue()+stkdur.topValue()<=stp[i]&&curtime2+stkdur.topValue()<=stp[i]){
            cout<<"j";
            if(curtime2<stkstp.topValue())
                curtime2=stkstp.topValue();
            ansstp.enqueue(curtime2);
            curtime2+=stkdur.topValue();
            ansdur.enqueue(stkdur.pop());
            ansid.enqueue(stkid.pop());
            stkstp.pop();
        }
        if(qiustp.length()<MAX_PLAY){
            qiustp.enqueue(stp[i]);
            qiudur.enqueue(dur[i]);
            qiuid.enqueue(id[i]);
        }
        else{
            if(stkstp.length()==0&&curtime2<=stp[i]){
                curtime2=stp[i];
                ansstp.enqueue(curtime2);
                curtime2+=dur[i];
                ansdur.enqueue(dur[i]);
                ansid.enqueue(id[i]);
                continue;
            }
            stkstp.push(stp[i]);
            stkdur.push(dur[i]);
            stkid.push(id[i]);
        }
    }
    while(qiustp.length()>0){
        if(curtime<qiustp.frontValue())curtime=qiustp.frontValue();
        ansstp.enqueue(curtime);
        curtime+=qiudur.frontValue();
        ansdur.enqueue(qiudur.dequeue());
        ansid.enqueue(qiuid.dequeue());
        qiustp.dequeue();
    }
    while(stkstp.length()>0){
            if(curtime2<stkstp.topValue())curtime2=stkstp.topValue();
            ansstp.enqueue(curtime2);
            curtime2+=stkdur.topValue();
            ansdur.enqueue(stkdur.pop());
            ansid.enqueue(stkid.pop());
            stkstp.pop();
    }
    int ans[n],finalid[n];
    for(int i=0;i<n;i++){
        if(ansstp.frontValue()>c){
            ans[i]=-1;
            ansstp.dequeue();
            ansdur.dequeue();
        }
        else{
            ans[i]=ansstp.dequeue()+ansdur.dequeue();
            if(ans[i]>c)
                ans[i]=c;
        }
        finalid[i]=ansid.dequeue();
    }
    for(int i=1;i<n;i++){
        for(int j=i-1;j>=0&&finalid[j+1]<finalid[j];j--){
            int temp=finalid[j+1];
            finalid[j+1]=finalid[j];
            finalid[j]=temp;
            temp=ans[j+1];
            ans[j+1]=ans[j];
            ans[j]=temp;
        }
    }
    for(int i=0;i<n;i++){
        out<<finalid[i]<<">";
        if(ans[i]==-1)
            out<<"C"<<endl;
        else
            out<<ans[i]<<endl;
    }
    in.close();
    out.close();
    return 0;
}
