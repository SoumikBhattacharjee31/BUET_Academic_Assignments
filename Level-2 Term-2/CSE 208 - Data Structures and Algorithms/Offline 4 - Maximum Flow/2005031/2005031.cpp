#include<bits/stdc++.h>
using namespace std;


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
    int flow=0;
    Edge(int child, int capacity){
        this->child=child;
        this->capacity=capacity;
    }
};


struct Vertex{
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


vector<TeamInfo> input(){
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
}


map<string,int> teamNameMapping(vector<TeamInfo> teams){
    map<string,int> teamMap;
    for(int i=0;i<teams.size();i++)
        teamMap[teams[i].teamName]=i;
    return teamMap;
}


map<int,string> teamNameReverseMapping(vector<TeamInfo> teams){
    map<int,string> teamMap;
    for(int i=0;i<teams.size();i++)
        teamMap[i]=teams[i].teamName;
    return teamMap;
}


vector<Vertex> makeGraph(vector<TeamInfo> teams){
    // 0-n-1: teams
    // n: source
    // n+1: destination
    // n+2-rest: matches
    vector<Vertex> graph;
    for(int i=0;i<teams.size();i++){
        Vertex newVertex(i);
        newVertex.push_back(teams.size()+1);
        graph.push_back(newVertex);
    }
    Vertex source(teams.size());
    Vertex destination(teams.size()+1);
    graph.push_back(source);
    graph.push_back(destination);
    for(int i=0;i<teams.size();i++)
        for(int j=i+1;j<teams.size();j++)
            if(teams[i].leftOwnDivision[j]){
                Vertex newVertex(i,j);
                newVertex.push_back(i);
                newVertex.push_back(j);
                graph.push_back(newVertex);
            }
    for(int i=teams.size()+2;i<graph.size();i++)
        graph[teams.size()].push_back(i);
    return graph;
}


void printGraph(vector<Vertex> graph){
    for(auto vertex:graph){
        cout<<vertex.teamIndex<<' '<<vertex.teamIndex2<<endl;
        for(auto element:vertex.list)
            cout<<"Ch: "<<element.child<<", Cap: "<<element.capacity<<", F:"<<element.flow<<endl;
        cout<<endl<<endl;
    }
}


void edmondKarp(vector<Vertex> graph, vector<TeamInfo> teams, map<int,string>teamNameRevMap){
    for(int currentPred=0;currentPred<teams.size();currentPred++){
        bool done=false;
        for(int i=0;i<teams.size();i++)
            if(teams[currentPred].win+teams[currentPred].left<teams[i].win){
                cout<<teamNameRevMap[currentPred]<<" is eliminated.\n";
                cout<<"They can win at most "<<teams[currentPred].win<<" + "<<teams[currentPred].left<<" = "<<teams[currentPred].win+teams[currentPred].left<<" games.\n";
                cout<<teamNameRevMap[i]<<" has won a total of "<<teams[i].win<<" games.\n";
                cout<<"They play each other 0 times.\n";
                cout<<"So on average, each of the teams in this group wins "<<teams[i].win<<"/1 = "<<teams[i].win<<" games.\n\n";
                done=true;
                break;
            }
        if(done)
            continue;
        // fix graph
        for(int i=0;i<graph.size();i++){
            graph[i].revlist.clear();
            for(int j=0;j<graph[i].list.size();j++){
                graph[i].list[j].flow=0;
                if(i==teams.size()&&(graph[graph[i].list[j].child].teamIndex==currentPred)||(graph[graph[i].list[j].child].teamIndex2==currentPred))
                    graph[i].list[j].capacity=0;
                else if(i==teams.size()){
                    graph[i].list[j].capacity=teams[graph[graph[i].list[j].child].teamIndex].leftOwnDivision[graph[graph[i].list[j].child].teamIndex2];
                }
                else if(graph[i].list[j].child==teams.size()+1)
                    graph[i].list[j].capacity=max(teams[currentPred].win+teams[currentPred].left-teams[graph[i].teamIndex].win,0);
                else
                    graph[i].list[j].capacity=INT_MAX;
            }
        }
        // -fix graph

        bool doneIterating=false;
        while(!doneIterating){
            for(int i=0;i<graph.size();i++){
                graph[i].parent=-1;
                graph[i].visited=false;
            }
            doneIterating=true;
            queue<int> q;
            q.push(teams.size());
            graph[teams.size()].visited=true;
            while(!q.empty()){
                int u=q.front();
                q.pop();
                for(int i=0;i<graph[u].list.size();i++){
                    int v=graph[u].list[i].child;
                    if(graph[v].visited||graph[u].list[i].capacity<=graph[u].list[i].flow)
                        continue;
                    graph[v].visited=true;
                    graph[v].parent=u;
                    q.push(v);
                    if(v==teams.size()+1){
                        doneIterating=false;
                        int parent=u;
                        int gran=graph[u].parent;
                        int minflow=0;
                        for(int i=0;i<graph[teams.size()].list.size();i++)
                            if(graph[teams.size()].list[i].child==gran){
                                minflow=graph[teams.size()].list[i].capacity-graph[teams.size()].list[i].flow;
                                break;
                            }
                        for(int i=0;i<graph[parent].list.size();i++)
                            if(graph[parent].list[i].child==v){
                                minflow=min(minflow,graph[parent].list[i].capacity-graph[parent].list[i].flow);
                                break;
                            }
                        for(int i=0;i<graph[teams.size()].list.size();i++)
                            if(graph[teams.size()].list[i].child==gran){
                                graph[teams.size()].list[i].flow+=minflow;
                                break;
                            }
                        for(int i=0;i<graph[parent].list.size();i++)
                            if(graph[parent].list[i].child==v){
                                graph[parent].list[i].flow+=minflow;
                                break;
                            }
                        for(int i=0;i<graph[gran].list.size();i++)
                            if(graph[gran].list[i].child==parent){
                                graph[gran].list[i].flow+=minflow;
                                break;
                            }
                        // if(currentPred==2){
                        //     printGraph(graph);
                            // for(auto x:graph[teams.size()].list){
                            //     cout<<x.capacity<<' '<<x.flow<<endl;
                            // }
                        // }
                        while(!q.empty())
                            q.pop();
                        break;
                    }
                }
            }
        }

        for(int i=teams.size()+2;i<graph.size();i++){
            int index=0;
            for(int j=0;j<graph[i].list.size();j++)
                if(graph[i].list[j].child==graph[i].teamIndex)
                    index=j;
            graph[graph[i].teamIndex].revpush_back(i,graph[i].list[index].flow);
            for(int j=0;j<graph[i].list.size();j++)
                if(graph[i].list[j].child==graph[i].teamIndex2)
                    index=j;
            graph[graph[i].teamIndex2].revpush_back(i,graph[i].list[index].flow);
        }

        for(int i=0;i<graph.size();i++){
            graph[i].parent=-1;
            graph[i].visited=false;
        }
        vector<int> minCutVertices;
        doneIterating=true;
        queue<int> q;
        q.push(teams.size());
        graph[teams.size()].visited=true;
        while(!q.empty()){
            int u=q.front();
            q.pop();
            for(int i=0;i<graph[u].list.size();i++){
                int v=graph[u].list[i].child;
                if(graph[v].visited||graph[u].list[i].capacity<=graph[u].list[i].flow)
                    continue;
                if(graph[v].teamIndex2==INT_MAX)
                    minCutVertices.push_back(v);
                graph[v].visited=true;
                graph[v].parent=u;
                q.push(v);
            }
            // for(int i=0;i<graph[u].revlist.size();i++){
            //     int v=graph[u].revlist[i].child;
            //     if(graph[v].visited||graph[u].revlist[i].capacity<=graph[u].revlist[i].flow)
            //         continue;
            //     if(graph[v].teamIndex2==INT_MAX)
            //         minCutVertices.push_back(v);
            //     graph[v].visited=true;
            //     graph[v].parent=u;
            //     q.push(v);
            // }
        }


        // if(currentPred==2)printGraph(graph);
        // printGraph(graph);

        // get ans
        bool yes=true;
        for(auto x:graph[teams.size()].list){
            // cout<<x.capacity<<' '<<x.flow<<endl;
            if(x.capacity>x.flow){
                // cout<<"No"<<endl;
                yes=false;
                break;
            }
        }
        if(!yes){
            // cout<<"Yes"<<endl;
            cout<<teamNameRevMap[currentPred]<<" is eliminated.\n";
            cout<<"They can win at most "<<teams[currentPred].win<<" + "<<teams[currentPred].left<<" = "<<teams[currentPred].win+teams[currentPred].left<<" games.\n";
            int total=0;
            for(int i=0;i<minCutVertices.size();i++){
                cout<<teamNameRevMap[minCutVertices[i]];
                if(i!=minCutVertices.size()-1)
                    cout<<" and ";
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
}


int main(){
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    auto teams=input();
    auto teamNameMap=teamNameMapping(teams);
    auto teamNameRevMap=teamNameReverseMapping(teams);
    auto graph=makeGraph(teams);
    // printGraph(graph);
    edmondKarp(graph,teams,teamNameRevMap);
    // cout<<"Hello";
    return 0;
}