#include<fstream>
#include "lruCache.h"

using namespace std;
int main(){
    ifstream in;
    in.open("lru_input.txt");
    ofstream out;
    out.open("lru_output.txt");
    int c;
    in>>c;
    LRUCache<int,int> cache_Memory(c);
    int q;
    in>>q;
    for(int i=0;i<q;i++){
        int operation,x,y;
        in>>operation;
        switch(operation){
            case 1:{
                in>>x;
                out<<cache_Memory.get(x)<<endl;
                break;
            }
            case 2:{
                in>>x>>y;
                cache_Memory.put(x,y);
            }
        }
    }
    return 0;
}
