#include<stdio.h>
int main(){
    unsigned int n, i, max,ans=0,j=0,l;
    printf("Enter the number: ");
    scanf("%u",&n);

    for(max=0;n>>max;max++);

    for(i=0;i<max;i++){
        if(i%3==0) continue;

        l=((n&(1<<i))?1:0);
        l=l<<j;
        ans=ans|l;
        j++;
    }
    printf("%u",ans);
}
