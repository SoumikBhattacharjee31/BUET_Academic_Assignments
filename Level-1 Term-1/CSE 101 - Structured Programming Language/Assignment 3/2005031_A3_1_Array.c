#include <stdio.h>
#include <windows.h>
#include <conio.h>


void gotoxy(int x, int y);
int menu();
void function(int option, int array[]);
void input(int array[]);
void display(int array[]);
void sort(int array[]);
void insert(int array[]);
void append(int array[]);
void deleted(int array[]);
void search(int array[]);
int ifsorted(int array[]);
void endfunc();


int main(){
    int option, i, array[1000];

    for(i=0;i<1000;i++)
        array[i]=-1;

    do{
        option=menu();
        function(option, array);
    } while (option!=8);

    return 0;
}


void gotoxy(int x, int y){
    COORD coordinate = {x,y};
    HANDLE cursor = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(cursor, coordinate);
}


int menu(){
    int option;
    gotoxy(10,10); printf("Choice:-");
    gotoxy(10,11); printf("1 for taking input");
    gotoxy(10,12); printf("2 for display");
    gotoxy(10,13); printf("3 for sorting");
    gotoxy(10,14); printf("4 for inserting");
    gotoxy(10,15); printf("5 for appending");
    gotoxy(10,16); printf("6 for deleting");
    gotoxy(10,17); printf("7 for searching");
    gotoxy(10,18); printf("8 for Exit");
    gotoxy(10,20); printf("Enter your choice: ");

    scanf("%d",&option);
    system("CLS");
    return option;
}


void function(int option, int array[]){
    switch (option){
        case 1: input(array);    break;
        case 2: display(array);  break;
        case 3: sort(array);     break;
        case 4: insert(array);   break;
        case 5: append(array);   break;
        case 6: deleted(array);  break;
        case 7: search(array);   break;
    }
}


void input(int array[]){
    int i=0;

    do{
        gotoxy(10,10); printf("Enter number for position %d (Enter -1 to end taking input): ",i);
        scanf("%d",&array[i]);

        i++;

        system("CLS");
    } while(array[i-1]!=-1);

    gotoxy(10,10); printf("You have successfully input the value(s)");

    endfunc();
}


void display(int array[]){
    int i;

    gotoxy(10,10);

    if(array[0]==-1)
        printf("The array is empty");

    else{
        for(i=0;array[i]!=-1;i++)
            printf("%d ",array[i]);
    }

    endfunc();
}


void sort(int array[]){
    int i,j,temp;

    gotoxy(10,10);

    if(array[0]==-1)
        printf("The array is empty");

    else if(ifsorted(array))
        printf("The array is already sorted");

    else{
        for(i=0;array[i]!=-1;i++){
            for(j=i+1;array[j]!=-1;j++){
                if(array[i]>array[j]){

                    temp=array[i];
                    array[i]=array[j];
                    array[j]=temp;

                }
            }
        }
        printf("You have successfully sorted the array");
    }

    endfunc();
}


void insert(int array[]){
    int value, i;

    gotoxy(10,10);

    if(!ifsorted(array))
        append(array);

    else{
        printf("Enter the value that you would like to insert: ");
        scanf("%d",&value);
        system("CLS");

        for(i=0;array[i]!=-1;i++);

        for(;array[i-1]>value;i--)
            array[i]=array[i-1];

        array[i]=value;

        system("CLS");
        gotoxy(10,10); printf("You have successfully inserted the value");
        endfunc();
    }
}


void append(int array[]){
    int value, i;
    gotoxy(10,10);
    printf("Enter the value that you would like to insert: ");
    scanf("%d",&value);

    for(i=0;array[i]!=-1;i++);

    array[i]=value;

    system("CLS");
    gotoxy(10,10); printf("You have successfully inserted the value");
    endfunc();
}


void deleted(int array[]){
    int i;

    for(i=0;array[i]!=-1;i++)
        array[i]=-1;

    gotoxy(10,10); printf("You have successfully deleted the array");
    endfunc();
}


void search(int array[]){
    int value, i, found=0;

    gotoxy(10,10); printf("Enter the value that you would like to search: ");
    scanf("%d",&value);

    system("CLS");
    gotoxy(10,10);

    if(ifsorted(array)){
        for(i=0;value<array[i];i++);
        printf("%d is in the position %d", value, i);

        for(;array[i]==value;i++)
            printf(", %d",i);
        endfunc();
        return;
    }

    for(i=0;array[i]!=-1;i++){
        if(array[i]==value){
            found=1;
            printf("%d is in the position %d", value, i);
            i++;
            break;
        }
    }

    for(;array[i]!=-1;i++){
        if(array[i]==value)
            printf(", %d",i);
    }

    if(!found) printf("%d is not in the array",value);
    endfunc();
}


int ifsorted(int array[]){
    int i, j;
    for(i=1;array[i]!=-1 && array[i-1]<=array[i];i++);
    for(j=0;array[j]!=-1;j++);
    if(i==j)
        return 1;
    else
        return 0;
}


void endfunc(){
    gotoxy(10,11); printf("Press any key to go back to menu");
    getch();
    system("CLS");
}
