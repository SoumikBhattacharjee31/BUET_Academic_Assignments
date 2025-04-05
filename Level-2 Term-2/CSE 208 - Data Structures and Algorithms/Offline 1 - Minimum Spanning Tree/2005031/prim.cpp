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


// edge comparator
struct compareEdge{
    bool operator () (edge edge1, edge edge2){
        return edge1.weight>edge2.weight;
    }
};


// vertex structure
struct vertex{
    vector<pair<int,int>> list;
    bool notvisited=true;
};


// input
void input_weighted_edges(int&no_of_vertices, int&no_of_edges, vector<edge>&edges){
    cin>>no_of_vertices>>no_of_edges;
    for(int i=0;i<no_of_edges;i++){
        int parent, child, weight;
        cin>>parent>>child>>weight;
        edge newedge(parent,child,weight);
        edges.push_back(newedge);
    }
}


// make a graph from input edges
void make_unordered_graph(vector<edge>edges, int no_of_vertices, vector<vertex>&graph){
    for(int i=0;i<no_of_vertices;i++){
        vertex newvertex;
        graph.push_back(newvertex);
    }
    for(auto it=edges.begin();it!=edges.end();it++){
        graph[it->parent].list.push_back(make_pair(it->child,it->weight));
        graph[it->child].list.push_back(make_pair(it->parent,it->weight));
    }
}


// an index of edge with minium weight
int get_minimum_edge_index(vector<edge>edges){
    int min_index=0;
    int min_weight=INT_MAX;
    for(int i=0;i<edges.size();i++)
        if(edges[i].weight<min_weight){
            min_weight=edges[i].weight;
            min_index=i;
        }
    return min_index;
}


// prim's algorithm
void recPrim(vector<vertex>& graph, priority_queue<edge,vector<edge>,compareEdge>& pq, int pres, vector<edge>&tree){

    // if reaches  a non-visited vertex, adds edges connected to it to the queue
    if(graph[pres].notvisited){
        graph[pres].notvisited=false;
        for(int i=0;i<graph[pres].list.size();i++){
            edge newedge(pres,graph[pres].list[i].first,graph[pres].list[i].second);
            pq.emplace(newedge);
        }
    }

    // returns if queue is empty
    if(pq.empty())
        return;
    
    // prim's algorithm
    edge nextedge=pq.top();
    pq.pop();
    if(graph[nextedge.child].notvisited){
        tree.push_back(nextedge);
        recPrim(graph,pq,nextedge.child,tree);
    }
    else if(!pq.empty())
        recPrim(graph,pq,pres,tree);
}


void print(vector<edge> tree, int source){

    // count total weight
    int total=0;
    for(edge x:tree)
        total+=x.weight;
    
    // print
    cout<<"Total weight = "<<total<<endl;
    cout<<"Root node = "<<source<<endl;
    for(edge x:tree)
        cout<<x.parent<<' '<<x.child<<endl;
}


int main(){

    // Variable Declaration
    freopen("input.txt","r",stdin);
    int n, m;
    vector<edge> edges;
    vector<vertex> graph;
    vector<edge> mst;
    priority_queue<edge,vector<edge>,compareEdge> pq;

    // Make graph
    input_weighted_edges(n,m,edges);
    make_unordered_graph(edges,n,graph);

    // Prim's algorithm
    int source=0;
    recPrim(graph,pq,source,mst);

    // Print the output MST
    // fixtree(mst);
    // sort(mst.begin(),mst.end(),comparator_Edge);
    print(mst,source);
    return 0;
}
