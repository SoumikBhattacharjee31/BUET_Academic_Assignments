#include <bits/stdc++.h>
using namespace std;
typedef vector<vector<double>> Graph;

struct bitmask{
    int size;
    vector<unsigned int> arr;
    bitmask(int size){
        this->size = size;
        arr.resize(ceil(size/32.0),-1);
        if(size%32)
            arr[arr.size()-1]=(1<<(size%32))-1;
    }
    bool is_empty(){
        for (auto x:arr){
            if(x)
                return false;
        }
        return true;
    }
    void erase(int i){
        arr[i/32]&=~(1<<(i%32));
    }
    int operator [] (int i){
        return (arr[i/32]&(1<<(i%32)))?1:0;
    }
    void print(){
        for (auto x:arr){
            while(x){
                cout<<x%2;
                x/=2;
            }
        }
        cout<<endl;
    }
};

Graph Create_Random_Graph(int v){
    random_device rd;
    mt19937 gen(rd());
    Graph graph(v,vector<double>(v,0));
    for (int i=0; i<v; i++){
        for (int j=0; j<v; j++){
            if(i==j)
                continue;
            double minrange=50;
            double maxrange=200;
            for(int k=0;k<v;k++){
                if(graph[i][k]==0 || graph[k][j]==0 || i==k || j==k)
                    continue;
                double tempmaxrange = graph[i][k]+graph[k][j];
                double tempminrange = abs(graph[i][k]-graph[k][j]);
                maxrange = min(maxrange, tempmaxrange);
                minrange = max(minrange, tempminrange);
            }
            uniform_real_distribution<double> dist(minrange, maxrange);
            int weight = dist(gen);
            graph[i][j]=graph[j][i]=weight;
        }
    }
    return graph;
}

pair<double, vector<int>> mincost(Graph graph, bitmask bet, int endvertex){
    // bet.print();
    if(bet.is_empty()){
        vector<int> distvector;
        distvector.push_back(0);
        distvector.push_back(endvertex);
        return make_pair(graph[endvertex][0],distvector);
    }
    double mindistance=DBL_MAX;
    int minindex = -1;
    vector<int> minvector;
    for(int i=0;i<graph.size();i++){
        if(bet[i]){
            bitmask newbet(bet);
            newbet.erase(i);
            auto obj = mincost(graph, newbet, i);
            double newdistance = obj.first + graph[i][endvertex];
            if(mindistance>newdistance){
                mindistance = newdistance;
                minindex=i;
                minvector=obj.second;
            }
        }
    }
    minvector.push_back(endvertex);
    return make_pair(mindistance,minvector);
}

vector<int> Extract_TSP(Graph graph){
    bitmask bet(graph.size());
    bet.erase(0);
    auto path = mincost(graph, bet, 0).second;
    reverse(path.begin(),path.end());
    return path;
}

struct edge{
    int parent, child, weight;
    edge(int parent, int child, int weight){
        this->parent=parent;
        this->child=child;
        this->weight=weight;
    }
};

struct vertex{
    vector<pair<int,int>> list;
    bool notvisited=true;
};

struct compareEdge{
    bool operator () (edge edge1, edge edge2){
        return edge1.weight>edge2.weight;
    }
};

void make_adjacency_list(Graph edges, int no_of_vertices, vector<vertex>&graph){
    for(int i=0;i<no_of_vertices;i++){
        vertex newvertex;
        graph.push_back(newvertex);
    }
    for(int i=0;i<edges.size();i++)
        for(int j=0;j<edges.size();j++)
            if(i!=j)
                graph[i].list.push_back(make_pair(j,edges[i][j]));
}

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

vector<edge> prim(Graph matrix){
    vector<vertex> graph;
    vector<edge> mst;
    priority_queue<edge,vector<edge>,compareEdge> pq;
    make_adjacency_list(matrix, matrix.size(), graph);
    int source=0;
    recPrim(graph,pq,source,mst);
    return mst;
}

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

void dfs(int cur, vector<vertex> graph, vector<int>&ans){
    if(!graph[cur].notvisited)
        return;
    graph[cur].notvisited=false;
    ans.push_back(cur);
    for(auto x:graph[cur].list)
        dfs(x.first,graph,ans);
}

vector<int> Metric_Approximation_TSP(Graph graph){
    auto mst=prim(graph);
    vector<vertex> graph2;
    make_unordered_graph(mst, graph.size(), graph2);
    vector<int> ans;
    dfs(0,graph2,ans);
    ans.push_back(ans[0]);
    return ans;
}

double Calculate_Tour_Length(vector<int> tour, Graph graph){
    double ans=0;
    for(int i=1;i<tour.size();i++)
        ans+=graph[tour[i]][tour[i-1]];
    return ans;
}


int main(){
    int v=5;
    int x;
    cout<<"Enter the number of test cases: ";
    cin>>x;
    for(int i=1;i<=x;i++){
        auto graph = Create_Random_Graph(v);
        auto optimal_path =  Extract_TSP(graph);
        auto approx_path = Metric_Approximation_TSP(graph);
        auto optimal_distance = Calculate_Tour_Length(optimal_path, graph);
        auto approx_distance = Calculate_Tour_Length(approx_path, graph);
        double ratio = approx_distance/optimal_distance;
        cout<<"x: "<<i<<" ,ratio: "<<ratio<<endl;


        for(auto x:optimal_path)cout<<x<<' ';
        cout<<endl;
        for(auto x:approx_path)cout<<x<<' ';
    }
}