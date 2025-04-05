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
bool compareEdge(edge edge1, edge edge2){
    return edge1.weight<edge2.weight;
}


// input
void input(int&no_of_vertices, int&no_of_edges, vector<edge>&edges){
    cin>>no_of_vertices>>no_of_edges;
    for(int i=0;i<no_of_edges;i++){
        int parent, child, weight;
        cin>>parent>>child>>weight;
        edge newedge(parent,child,weight);
        edges.push_back(newedge);
    }
}


// find set using union find algorithm
int findset(int parent[], int element){
    if(element==parent[element])
        return element;
    return findset(parent,parent[element]);
}


// print
void print(vector<edge> tree){
    int total=0;
    for(edge x:tree)
        total+=x.weight;
    cout<<"Total weight = "<<total<<endl;
    for(edge x:tree)
        cout<<x.parent<<' '<<x.child<<endl;
}


void kruskal(vector<edge>edges, int n, vector<edge>&tree){
    // initialize sets
    int parent[n];
    int rank[n];
    for(int i=0;i<n;i++)
        parent[i]=i;
    for(int i=0;i<n;i++)
        rank[i]=-1;

    // disjoint set union
    for(auto it=edges.begin();it!=edges.end();it++){
        int set1=findset(parent,it->parent);
        int set2=findset(parent,it->child);
        if(set1!=set2){
            if(rank[set1]<rank[set2])
                swap(set1,set2);
            else if(rank[set1]==rank[set2])
                rank[set1]++;
            parent[set2]=set1;
            tree.push_back(*it);
        }
    }
}


int main(){
    // Variable declaration
    freopen("input.txt","r",stdin);
    int n, m;
    vector<edge> edges;
    vector<edge> tree;

    // intput and sort edges
    input(n,m,edges);
    sort(edges.begin(),edges.end(),compareEdge);

    // kruskal algorithm
    kruskal(edges,n,tree);

    // print
    print(tree);
    return 0;
}