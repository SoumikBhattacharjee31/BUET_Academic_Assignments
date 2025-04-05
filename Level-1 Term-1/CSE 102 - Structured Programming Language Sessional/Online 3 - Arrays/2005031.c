#include<stdio.h>
int main(){
    int n, i, j, min=32767;
    printf("Enter the height of the square matrix: ");
    scanf("%d",&n);
    int mat[n][n];
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("Enter the value of the (%d,%d) element", i+1, j+1);
            scanf("%d",&mat[i][j]);
            min=(min>mat[i][j])?mat[i][j]:min;
        }
    }
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(n%2){
                if(i==j && i==n/2) continue;
                mat[i][j]=(mat[i][j]%2)?min:mat[i][j];
            }
            else{
                if((i==n/2&&(j==i-1||j==i))||(j==n/2&&(i==j-1||j==i))) continue;
                mat[i][j]=(mat[i][j]%2)?min:mat[i][j];
            }
        }
    }
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%4d",mat[i][j]);
        }
        printf("\n");
    }
    return 0;
}
