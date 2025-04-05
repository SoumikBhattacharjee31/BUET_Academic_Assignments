#include<bits/stdc++.h>
using namespace std;
typedef vector<vector<double>> MAT;


// Edge structure
struct Edge{
    int parent, child;
    double distance;
    Edge(int p, int c, double d){
        parent=p;
        child=c;
        distance=d;
    }
};


// input
void input(int& n, int& m, vector<Edge>& edges){
    cin>>n>>m;
    while(m--){
        int p, c, d;
        cin>>p>>c>>d;
        p--,c--;
        Edge e(p,c,d);
        edges.push_back(e);
    }
}


// print matrix
void print(MAT&mat){
    cout<<"Shortest distance matrix"<<endl;

    for(int i=0;i<mat.size();i++){
        for(int j=0;j<mat.size();j++)
            if(mat[i][j]!=INT_MAX)
                cout<<mat[i][j]<<' ';
            else
                cout<<"inf"<<' ';
        cout<<endl;
    }

    cout<<endl;
}


// created adjanency matrix
MAT initMat(vector<Edge> edges, int n){
    MAT mat(n, vector<double>(n));

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if(i!=j)
                mat[i][j]=INT_MAX;
            else
                mat[i][j]=0;

    for(auto e:edges){
        mat[e.parent][e.child]=e.distance;
        mat[e.child][e.parent]=1.0/e.distance;
    }

    return mat;
}


// floyd-warshal implementation
MAT floyd_apsp(MAT mat, bool& hello){
    int n=mat.size();
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            for(int k=0;k<n;k++)
                if(mat[j][i]!=INT_MAX && mat[i][k]!=INT_MAX){
                    mat[j][k]=min(mat[j][k],mat[j][i]+mat[i][k]);
                    hello=true;
                }
    return mat;
}


// matrix multiplication implementation
MAT matmul_apsp(MAT mat){
    int n=mat.size();
    double maxitr=log2(n)+1;
    for(double h=0;h<maxitr;h++){
        MAT res(n,vector<double>(n,INT_MAX));
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                for(int k=0;k<n;k++)
                    if(mat[i][k]!=INT_MAX && mat[k][j]!=INT_MAX)
                        res[i][j]=min(res[i][j],mat[i][k]*mat[k][j]);
        mat=res;
    }
    return mat;
}

int main(){
    map<string,int> mp;
    int n;
    cin>>n;
    vector<string> revmp;
    for(int i=0;i<n;i++){
        string s;
        cin>>s;
        mp[s]=i;
        revmp.push_back(s);
    }
    int m;
    cin>>m;
    cout<<m;
    vector<Edge> edges;
    for(int i=0;i<m;i++){
        string ci,cj;
        double weight;
        cin>>ci>>weight>>cj;
        Edge e(mp[ci],mp[cj],weight);
        edges.push_back(e);
    }
    MAT mat=initMat(edges,n);
    bool ques=false;
    MAT floyd_mat=floyd_apsp(mat,ques);
    print(floyd_mat);
    if(ques)cout<<"yes";
    else cout<<"no";
    return 0;
}


/*int main(){
    // initialization and input
    freopen("in.txt","r",stdin);
    int n,m;
    vector<Edge> edges;
    input(n,m,edges);

    // create adjacency matrix
    MAT mat=initMat(edges,n);

    // floyd-warshal part
    MAT floyd_mat=floyd_apsp(mat);
    print(floyd_mat);

    // matrix multiplication part
    MAT matmul_mat=matmul_apsp(mat);
    print(matmul_mat);

    return 0;
}*/
