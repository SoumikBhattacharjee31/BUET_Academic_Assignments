#include<bits/stdc++.h>
using namespace std;


// each row of graph
class row{
    public:
    vector<int> list;
    char color;
    int rank;
    row(){
        color='w';
        rank=0;
    }
};


// recursive function of dfs
void dfsrec(row graph[], int source, vector<int>& ans){
    graph[source].color='g';
    for(int i:graph[source].list){
        if(graph[i].color=='w'){
            graph[i].rank=graph[source].rank+1;
            dfsrec(graph,i,ans);
        }
    }
    ans.push_back(source);
}


// initialization and output of dsa
void dfs(row graph[],int n){
    // initialization
    vector<int> ans;
    vector<pair<int,int>> gm;
    int presgm=-1;

    // control
    for(int i=0;i<n;i++){
        if(graph[i].color=='w')
            dfsrec(graph,i,ans);
    }

    // subordinate count
    for(int i=ans.size()-1;i>=0;i--){
        if(graph[ans[i]].rank==0){
            presgm++;
            int init=0;
            gm.push_back(make_pair(ans[i],init));
        }
        else{
            gm[presgm].second++;
        }
    }

    //print
    presgm=-1;
    for(int i=ans.size()-1;i>=0;i--){
        if(graph[ans[i]].rank==0){
            presgm++;
            cout<<endl<<ans[i]<<" "<<gm[presgm].second<<endl;
        }
        else
            cout<<ans[i]<<" "<<graph[ans[i]].rank<<endl;
    }
}


int main(){
    freopen("input.txt","r",stdin);
    // freopen("output.txt","w",stdout);

    // input
    int n,m;
    cin>>n>>m;
    row graph[n];
    for(int i=0;i<m;i++){
        int x,y;
        cin>>x>>y;
        graph[x].list.push_back(y);
    }

    // dfs function
    dfs(graph,n);
    return 0;
}