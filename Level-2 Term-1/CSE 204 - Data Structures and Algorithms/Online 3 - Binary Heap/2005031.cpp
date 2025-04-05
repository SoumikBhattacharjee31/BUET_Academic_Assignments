#include<fstream>
#include<iostream>
#include<string>
using namespace std;
ifstream in;
ofstream out;
class myheap{
    int* heapArr;
    int heapArrSize;
    int forbidden;
    int initsize;
    int pressize;
    void swap(int&a,int &b){
        int temp=a;
        a=b;
        b=temp;
    }
    int minnext(int i){
        return (heapArr[2*i+1]<heapArr[2*i+2]?2*i+1:2*i+2);
    }
public:
    myheap(){
        forbidden=-1;
        initsize=2;
        heapArr=new int[initsize];
        pressize=initsize;
        heapArrSize=0;
    }
    myheap(const myheap& hp){
        forbidden=hp.forbidden;
        initsize=hp.initsize;
        pressize=hp.pressize;
        heapArr=new int[pressize];
        heapArrSize=hp.heapArrSize;
        for(int i=0;i<heapArrSize;i++)
            heapArr[i]=hp.heapArr[i];
    }
    int FindMin(){
        if(heapArrSize)
            return heapArr[0];
        return forbidden;
    }
    int ExtractMin(){
        if(!heapArrSize)
            return forbidden;
        int ret=heapArr[0];
        heapArr[0]=heapArr[--heapArrSize];
        for(int i=0;heapArr[i]>heapArr[minnext(i)]&&minnext(i)<heapArrSize;i=minnext(i))
            swap(heapArr[i],heapArr[minnext(i)]);
        if(heapArrSize<pressize/2&&heapArrSize>initsize){
            pressize/=2;
            int* temp=new int[pressize];
            int* temp2=heapArr;
            for(int i=0;i<heapArrSize;i++)
                temp[i]=temp2[i];
            heapArr=temp;
            delete[] temp2;
        }
        return ret;
    }
    void InsertKey(int key){
        if(heapArrSize>=pressize){
            pressize*=2;
            int* temp=new int[pressize];
            int* temp2=heapArr;
            for(int i=0;i<heapArrSize;i++)
                temp[i]=temp2[i];
            heapArr=temp;
            delete[] temp2;
        }
        heapArr[heapArrSize++]=key;
        for(int i=heapArrSize-1;i>0&&heapArr[(i-1)/2]>heapArr[i];i=(i-1)/2)
            swap(heapArr[i],heapArr[(i-1)/2]);
    }
    void DecreaseKey(int prevKey, int newKey){
        if(prevKey<newKey)
            return;
        for(int i=0;i<heapArrSize;i++)
            if(prevKey==heapArr[i]){
                heapArr[i]=newKey;
                for(int j=i;j>0&&heapArr[j]<heapArr[(j-1)/2];j=(j-1)/2)
                    swap(heapArr[j],heapArr[(j-1)/2]);
                return;
            }
    }
    void print(){
        out<<"Printing the binary heap ..."<<endl;
        for(int i=0;i<20;i++)
            out<<"- ";
        for(int i=0;i<heapArrSize;i++){
            int k=0,j=1;
            for(;j<=heapArrSize;k++,j*=2)
                if(j-1==i){
                    out<<endl<<"Level "<<k<<": ";
                    break;
                }
            out<<heapArr[i]<<" ";
        }
        out<<endl;
        for(int i=0;i<20;i++)
            out<<"- ";
        out<<endl;
    }
    void heapsort(int arr[], int size){
        int tempsize=initsize;
        int temp=heapArrSize;
        initsize=pressize+1;
        
        for(int i=0;i<size;i++)
            InsertKey(arr[i]);
        for(int i=0;i<size;i++){
            swap(heapArr[0],heapArr[heapArrSize]);
            ExtractMin();
        }
        heapArrSize=temp;
        initsize=tempsize;
    }
};
int main(){
    myheap hp;
    int arr[]={7,2,8,4,9,6};
    hp.heapsort(arr,6);
    hp.print();
    // in.open("in.txt");
    // out.open("out.txt");
    // myheap hp;
    // while(!in.eof()){
    //     string s;
    //     int a,b;
    //     in>>s;
    //     if(!s.compare("INS")){
    //         in>>a;
    //         hp.InsertKey(a);
    //         out<<"INSERTED "<<a<<endl;
    //     }
    //     else if(!s.compare("PRI"))
    //         hp.print();
    //     else if(!s.compare("DEC")){
    //         in>>a>>b;
    //         hp.DecreaseKey(a,b);
    //         out<<a<<" decreased to "<<b<<endl;
    //     }
    //     else if(!s.compare("FIN"))
    //         out<<"FindMin returned "<<hp.FindMin()<<endl;
    //     else if(!s.compare("EXT"))
    //         out<<"ExtractMin returned "<<hp.ExtractMin()<<endl;
    // }
    // in.close();
    // out.close();
    return 0;
}
