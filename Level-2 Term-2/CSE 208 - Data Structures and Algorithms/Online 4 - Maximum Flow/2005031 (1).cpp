#include<bits/stdc++.h>
using namespace std;
int stickerss;

struct TeamInfo{
    string teamName;
    int win;
    int loss;
    int left;
    vector<int> leftOwnDivision;
    TeamInfo(string teamName, int win, int loss, int left, vector<int> leftOwnDivision){
        this->teamName=teamName;
        this->win=win;
        this->loss=loss;
        this->left=left;
        this->leftOwnDivision=leftOwnDivision;
    }
};


struct Edge{
    int child;
    int capacity;
    Edge(int child, int capacity){
        this->child=child;
        this->capacity=capacity;
    }
};


/*struct Vertex{
    vector<Edge> list;
    vector<Edge> revlist;
    bool visited=false;
    int teamIndex;
    int teamIndex2;
    int parent=-1;
    Vertex(int teamIndex,int teamIndex2=INT_MAX){
        this->teamIndex=teamIndex;
        this->teamIndex2=teamIndex2;
        list.clear();
        revlist.clear();
    }
    void push_back(int child, int capacity=INT_MAX){
        Edge newEdge(child,capacity);
        list.push_back(newEdge);
    }
    void revpush_back(int child, int capacity=INT_MAX){
        Edge newEdge(child,capacity);
        revlist.push_back(newEdge);
    }
};*/
struct Vertex{
    vector<Edge> list;
    bool visited=false;
    // int teamIndex;
    // int teamIndex2;
    int parent=-1;
    // Vertex(int teamIndex,int teamIndex2=INT_MAX){
    // Vertex(){
        // this->teamIndex=teamIndex;
        // this->teamIndex2=teamIndex2;
        // list.clear();
    // }
    void push_back(int child, int capacity=INT_MAX){
        Edge newEdge(child,capacity);
        list.push_back(newEdge);
    }
};


void printTeams(vector<TeamInfo>teams){
    for(auto x:teams){
        cout<<x.teamName<<':';
        for(auto y:x.leftOwnDivision){
            cout<<y<<' ';
        }
        cout<<endl;
    }
}


/*vector<TeamInfo> input(){
    int numberOfTeams;
    vector<TeamInfo> teams;
    cin>>numberOfTeams;
    for(int i=0;i<numberOfTeams;i++){
        string teamName;
        int win,loss,left;
        cin>>teamName>>win>>loss>>left;
        vector<int> leftOwnDivision;
        for(int j=0;j<numberOfTeams;j++){
            int leftOwnDiv;
            cin>>leftOwnDiv;
            leftOwnDivision.push_back(leftOwnDiv);
        }
        TeamInfo teamInfo(teamName,win,loss,left,leftOwnDivision);
        teams.push_back(teamInfo);
    }
    return teams;
}*/
vector<Vertex> input(){
    //input
    int friends, sticker;
    cin>>friends>>sticker;
    stickerss=sticker
    vector<vector<int>> chart(friends+1,vector<int>(sticker,0));
    for(int i=0;i<friends;i++){
        int count;
        cin>>count;
        for(int j=0;j<count;j++){
            int c;
            cin>>c;
            chart[i][c]++;
        }
    }
    //input
    // 0 source
    // 1-sticker sticker
    // sticker+1 - friends+1+sticker friend
    //makegraph
    vector<Vertex> graph;
    Vertex source;
    graph.push_back(source);
    for(int i=0;i<sticker;i++){
        Vertex a;
        graph.push_back(a);
    }
    for(int i=0;i<friends;i++){
        Vertex a;
        graph.push_back(a);
    }
    Vertex sink;
    graph.push_back(sink);
    for(int i=0;i<sticker;i++)
        graph[0].push_back(i+1,chart[0][i]);
    for(int i=0;i<=sticker;i++)
        for(int j=0;j<friends;j++){
            if(chart[j+1][i]==0)
                graph[i].push_back(sticker+j+1,1);
            else
                graph[sticker+j+1].push_back(i,chart[j+1][i]-1);
        }
    for(int i=1;i<=sticker;i++){
        graph[i].push_back(graph.size()-1);
    }
    //makegraph
    return graph;
}


// map<string,int> teamNameMapping(vector<TeamInfo> teams){
//     map<string,int> teamMap;
//     for(int i=0;i<sticker;i++)
//         teamMap[teams[i].teamName]=i;
//     return teamMap;
// }


// map<int,string> teamNameReverseMapping(vector<TeamInfo> teams){
//     map<int,string> teamMap;
//     for(int i=0;i<sticker;i++)
//         teamMap[i]=teams[i].teamName;
//     return teamMap;
// }


// vector<Vertex> makeGraph(vector<TeamInfo> teams){
    // 0-n-1: teams
    // n: source
    // n+1: destination
    // n+2-rest: matches
//     vector<Vertex> graph;
//     for(int i=0;i<sticker;i++){
//         Vertex newVertex(i);
//         newVertex.push_back(sticker+1);
//         graph.push_back(newVertex);
//     }
//     Vertex source(sticker);
//     Vertex destination(sticker+1);
//     graph.push_back(source);
//     graph.push_back(destination);
//     for(int i=0;i<sticker;i++)
//         for(int j=i+1;j<sticker;j++)
//             if(teams[i].leftOwnDivision[j]){
//                 Vertex newVertex(i,j);
//                 newVertex.push_back(i);
//                 newVertex.push_back(j);
//                 graph.push_back(newVertex);
//             }
//     for(int i=sticker+2;i<graph.size();i++)
//         graph[sticker].push_back(i);
//     return graph;
// }


// void printGraph(vector<Vertex> graph){
//     for(auto vertex:graph){
//         cout<<vertex.teamIndex<<' '<<vertex.teamIndex2<<endl;
//         for(auto element:vertex.list)
//             cout<<"Ch: "<<element.child<<", Cap: "<<element.capacity<<endl;
//         cout<<endl<<endl;
//     }
// }


void edmondKarp(vector<Vertex> graph){
    int sticker = stickerss;
    bool doneIterating=false;
    while(!doneIterating){
        for(int i=0;i<graph.size();i++){
            graph[i].parent=-1;
            graph[i].visited=false;
        }
        doneIterating=true;
        queue<int> q;
        q.push(sticker);
        graph[sticker].visited=true;
        while(!q.empty()){
            int u=q.front();
            q.pop();
            for(int i=0;i<graph[u].list.size();i++){
                int v=graph[u].list[i].child;
                if(graph[v].visited||graph[u].list[i].capacity<=0)
                    continue;
                graph[v].visited=true;
                graph[v].parent=u;
                q.push(v);
                if(v==sticker+1){
                    doneIterating=false;
                    int minflow=INT_MAX;
                    int index=sticker+1;
                    while(graph[index].parent!=-1){
                        for(int j=0;j<graph[graph[index].parent].list.size();j++)
                            if(index==graph[graph[index].parent].list[j].child){
                                minflow=min(minflow,graph[graph[index].parent].list[j].capacity);
                                break;
                            }
                        index=graph[index].parent;
                    }
                    index=sticker+1;
                    while(graph[index].parent!=-1){
                        for(int j=0;j<graph[graph[index].parent].list.size();j++)
                            if(index==graph[graph[index].parent].list[j].child){
                                graph[graph[index].parent].list[j].capacity-=minflow;
                                break;
                            }
                        bool found=false;
                        int  where;
                        for(where=0;where<graph[index].list.size();where++)
                            if(graph[index].list[where].child==graph[index].parent){
                                found=true;
                                break;
                            }
                        if(found)
                            graph[index].list[where].capacity+=minflow;
                        else
                            graph[index].push_back(graph[index].parent,minflow);
                        index=graph[index].parent;
                    }
                    while(!q.empty())
                        q.pop();
                    break;
                }
            }
        }
    }


    for(int i=0;i<graph.size();i++){
        graph[i].parent=-1;
        graph[i].visited=false;
    }
    vector<int> minCutVertices;
    doneIterating=true;
    queue<int> q;
    q.push(sticker);
    graph[sticker].visited=true;
    while(!q.empty()){
        int u=q.front();
        q.pop();
        for(int i=0;i<graph[u].list.size();i++){
            int v=graph[u].list[i].child;
            if(graph[v].visited||graph[u].list[i].capacity<=0)
                continue;
            if(graph[v].teamIndex2==INT_MAX)
                minCutVertices.push_back(v);
            graph[v].visited=true;
            graph[v].parent=u;
            q.push(v);
        }
    }

    // get ans
    bool yes=true;
    for(auto x:graph[sticker].list){
        if(x.capacity>0){
            yes=false;
            break;
        }
    }
    if(!yes){
        cout<<teamNameRevMap[currentPred]<<" is eliminated.\n";
        cout<<"They can win at most "<<teams[currentPred].win<<" + "<<teams[currentPred].left<<" = "<<teams[currentPred].win+teams[currentPred].left<<" games.\n";
        int total=0;
        for(int i=0;i<minCutVertices.size();i++){
            cout<<teamNameRevMap[minCutVertices[i]];
            if(i==minCutVertices.size()-2)
                cout<<" and ";
            else if(i!=minCutVertices.size()-1)
                cout<<", ";
            total+=teams[minCutVertices[i]].win;
        }
        int gamewithin=0;
        for(int i=0;i<minCutVertices.size();i++)
            for(int j=i+1;j<minCutVertices.size();j++)
                gamewithin+=teams[minCutVertices[i]].leftOwnDivision[minCutVertices[j]];
        cout<<" has won a total of "<<total<<" games.\n";
        cout<<"They play each other "<<gamewithin<<" times.\n";
        cout<<"So on average, each of the teams in this group wins "<<total+gamewithin<<"/"<<minCutVertices.size()<<" = "<<(total+gamewithin)/(1.0*minCutVertices.size())<<" games.\n\n";
        // done=true;
    }
    // -get ans
}


int main(){
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    auto graph=input();
    // auto teamNameMap=teamNameMapping(teams);
    // auto teamNameRevMap=teamNameReverseMapping(teams);
    // auto graph=makeGraph(teams);
    // printGraph(graph);
    edmondKarp(graph);
    return 0;
}