#include <stdio.h>
long int perm(int n);
int main(){
    int n;
    long int ans;
    printf("Type the number of tiles: ");
    scanf("%d",&n);
    ans = perm(n);
    printf("There are %ld ways",ans);
}
long int perm(int n){
    if (n==1) return 2;
    else if (n==2) return 7;
    else if (n%2) return perm(n-1)*2+perm(n-2)*3;
    else return perm(n-1)*2+perm(n-2)*3;
}
