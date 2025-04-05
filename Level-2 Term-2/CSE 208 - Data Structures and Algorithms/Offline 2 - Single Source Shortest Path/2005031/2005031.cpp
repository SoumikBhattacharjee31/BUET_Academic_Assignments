#include<bits/stdc++.h>
using namespace std;


// weighted edge structure
struct edge{
    int parent, child, weight;
    edge(int parent, int child, int weight){
        this->parent=parent;
        this->child=child;
        this->weight=weight;
    }
};


// vertex structure
struct vertex{
    vector<pair<int,int>> list;
    bool notvisited;
    int parent;
    int distance;
    int value;
    bool notvisited2;
    vertex(int i){
        notvisited=true;
        parent=-1;
        distance=INT_MAX;
        value=i;
        notvisited2=true;
    }
};


// vertex and distance pair
struct mypair{
    int vertex;
    int distance;
    mypair(int v, int d){
        vertex=v;
        distance=d;
    }
};


// mypair comparator
struct comparemypair{
    bool operator () (mypair v1, mypair v2){
        return v1.distance>v2.distance;
    }
};


// input edges
void input_weighted_edges(int&no_of_vertices, int&no_of_edges, vector<edge>&edges){
    cin>>no_of_vertices>>no_of_edges;
    for(int i=0;i<no_of_edges;i++){
        int parent, child, weight;
        cin>>parent>>child>>weight;
        edge newedge(parent,child,weight);
        edges.push_back(newedge);
    }
}


// make a directed graph from input edges
void make_directed_graph(vector<edge>edges, int no_of_vertices, vector<vertex>&graph){
    for(int i=0;i<no_of_vertices;i++){
        vertex newvertex(i);
        graph.push_back(newvertex);
    }
    for(auto it=edges.begin();it!=edges.end();it++)
        graph[it->parent].list.push_back(make_pair(it->child,it->weight));
}


// bellmanford recursive part
void recbellmanford(vector<vertex>& graph, int source){
    if(graph[source].notvisited==false)
        return;

    graph[source].notvisited=false;

    for(int i=0;i<graph[source].list.size();i++){
        int& prevedge=graph[graph[source].list[i].first].distance;
        int newedge=graph[source].distance+graph[source].list[i].second;
        if(prevedge>newedge){
            prevedge=newedge;
            graph[graph[source].list[i].first].parent=source;
        }
    }

    for(int i=0;i<graph[source].list.size();i++)
        recbellmanford(graph,graph[source].list[i].first);
}


// bellmanford algorithm implementation
void bellmanford(vector<vertex>& graph, int source){
    graph[source].distance=0;
    for(int i=0;i<graph.size()-1;i++){
        for(int i=0;i<graph.size();i++)
            graph[i].notvisited=true;
        recbellmanford(graph, source);
    }
}

void reachabledfs(vector<vertex>& graph, int source, bool& destvar,int destination){
    if(source==destination)
        destvar=true;
    if(graph[source].notvisited2==false)
        return;
    graph[source].notvisited2=false;
    for(int i=0;i<graph[source].list.size();i++)
        reachabledfs(graph,graph[source].list[i].first,destvar,destination);
}

void recbellmanford_cyclecheck(vector<vertex>& graph, int source, bool& destvar, bool& cycle, int destination){
    if(graph[source].notvisited==false)
        return;

    graph[source].notvisited=false;

    for(int i=0;i<graph[source].list.size();i++){
        int& prevedge=graph[graph[source].list[i].first].distance;
        int newedge=graph[source].distance+graph[source].list[i].second;
        if(prevedge>newedge){
            cycle=true;
            prevedge=newedge;
            graph[graph[source].list[i].first].parent=source;
            reachabledfs(graph,source,destvar,destination);
        }
    }

    for(int i=0;i<graph[source].list.size();i++)
        recbellmanford_cyclecheck(graph,graph[source].list[i].first,destvar,cycle,destination);
}


// checks negative cycle
pair<bool,bool> check_cycle(vector<vertex>graph, int source, int destination){
    bool destvar=false;
    bool cycle=false;
    for(int i=0;i<graph.size();i++)
        graph[i].notvisited=true;
    recbellmanford_cyclecheck(graph,source,destvar,cycle,destination);
    return make_pair(destvar,cycle);
}

// bool check_cycle(vector<vertex>graph, int source){
//     for(auto vertex:graph){
//         for(auto adj: vertex.list){
//             if(graph[adj.first].distance>vertex.distance+adj.second)
//                 return true;
//         }
//     }
//     return false;
// }


// dijkstra algorithm implementation
void dijkstra(vector<vertex>& graph, int source) {
    graph[source].distance = 0;
    priority_queue<mypair, vector<mypair>, comparemypair> pq;
    mypair src(source,0);
    pq.push(src);

    while (!pq.empty()) {
        int parent = pq.top().vertex;
        pq.pop();
        if (!graph[parent].notvisited)
            continue;
        graph[parent].notvisited = false;

        for (auto adj:graph[parent].list) {
            int child=adj.first;
            int weight=adj.second;

            if (graph[child].notvisited && ((graph[parent].distance + weight) < graph[child].distance)) {
                graph[child].distance = graph[parent].distance + weight;
                graph[child].parent = parent;
                mypair cld(child,graph[child].distance);
                pq.push(cld);
            }
        }
    }
}


// resets graph so that dijkstra can be applied
void reset_graph(vector<vertex>&graph){
    for(int i=0;i<graph.size();i++){
        graph[i].notvisited=true;
        graph[i].parent=-1;
        graph[i].distance=INT_MAX;
        for(int j=0;j<graph[i].list.size();j++)
            graph[i].list[j].second=abs(graph[i].list[j].second);
    }
}


// gets path from graph parents
vector<int> getpath(vector<vertex>graph, int d){
    int parent=d;
    vector<int> path;
    while(parent!=-1){
        path.push_back(parent);
        parent=graph[parent].parent;
    }
    return path;
}


// prints input path
void print(vector<vertex>graph, vector<int> path){
    if(graph[path[0]].distance==INT_MAX){
        cout<<"INF"<<endl;
        return;
    }
    cout<<graph[path[0]].distance<<endl;
    for(int i=path.size()-1;i>=0;i--)
        cout<<path[i]<<(i==0?"\n":" -> ");
}


int main(){
    // Variable Declaration
    freopen("in.txt","r",stdin);
    int n, m, c, d;
    vector<edge> edges;
    vector<vertex> graph;

    // Make graph
    input_weighted_edges(n,m,edges);
    cin>>c>>d;
    make_directed_graph(edges,n,graph);

    // Bellman-Ford Algorithm
    bellmanford(graph,c);
    cout<<"Bellman Ford Algorithm:"<<endl;
    pair<bool,bool> cond=check_cycle(graph,c,d);
    if(cond.second)
        cout<<"Negative weight cycle present"<<endl;
    if(!cond.first){
        vector<int> path=getpath(graph,d);
        print(graph,path);
    }
    cout<<endl;

    // resets graph so that dijkstra can be used
    reset_graph(graph);

    // Dijkstra Algorithm
    cout<<"Dijkstra Algorithm:"<<endl;
    dijkstra(graph,c);
    vector<int> path=getpath(graph,d);
    print(graph,path);

    return 0;
}