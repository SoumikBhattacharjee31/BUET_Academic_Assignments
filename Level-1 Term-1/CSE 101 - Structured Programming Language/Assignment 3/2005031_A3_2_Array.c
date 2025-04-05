#include <stdio.h>
#include <windows.h>
#include <conio.h>


void gotoxy(int x, int y);
int menu();
void function(int option, int array[]);
void insert(int array[]);
void height(int array[]);
void display(int array[]);
void pre(int i, int array[]);
void in(int i, int array[]);
void post(int i, int array[]);
void deleted(int array[]);
void del(int i, int value, int array[]);
void getneardel(int i, int value, int array[]);
void delshiftright(int array[], int i);
void delshiftleft(int array[], int i);
void maxheight(int j, int array[]);
void printmaxh(int array[]);
void endfunc();
int maxh=0, neardel=0;


int main(){
    int option, i, array[1000];

    for(i=0;i<1000;i++)
        array[i]=-1;

    do{
        option=menu();
        function(option, array);
    } while (option!=6);

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
    gotoxy(10,11); printf("1 for inserting a new value");
    gotoxy(10,12); printf("2 for getting the height of a value");
    gotoxy(10,13); printf("3 for displaying the list");
    gotoxy(10,14); printf("4 for deleting an element");
    gotoxy(10,15); printf("5 for getting the maximum height of the tree");
    gotoxy(10,16); printf("6 for Exit");
    gotoxy(10,18); printf("Enter your choice: ");

    scanf("%d",&option);
    system("CLS");
    return option;
}


void function(int option, int array[]){
    switch (option){
        case 1: insert   (array);  break;
        case 2: height   (array);  break;
        case 3: display  (array);  break;
        case 4: deleted  (array);  break;
        case 5: printmaxh(array);  break;
    }
}


void insert(int array[]){
    int i;
    for(i=1;;i++){
        gotoxy(10,10); printf("Enter the value (Enter -2 when finished): ");
        scanf("%d",&array[i]);
        system("CLS");
        if(array[i]==-2){
            array[i]=-1;
            break;
        }
    }
    gotoxy(10,10); printf("The values are successfully inserted");
    endfunc();
}


void height(int array[]){
    int value, i=1, h=0;
    gotoxy(10,10);
    printf("Enter the value: ");
    scanf("%d",&value);
    system("CLS");
    gotoxy(10,10);
    while(array[i]!=-1){
        if(value<array[i]){
            i*=2;
            h++;
        }
        else if(value==array[i]){
            printf("The height of %d is %d",value, h);
            endfunc();
            return;
        }
        else{
            i=2*i+1;
            h++;
        }
    }
    printf("The value was not found");
    endfunc();
}


void display(int array[]){
    int option;
    gotoxy(10,10); printf("Choice:-");
    gotoxy(10,11); printf("1 for pre-order traversal");
    gotoxy(10,12); printf("2 for in-order traversal");
    gotoxy(10,13); printf("3 for post-order traversal");
    gotoxy(10,15); printf("Enter your choice: ");
    scanf("%d",&option);

    system("CLS");
    gotoxy(10,10);

    switch (option){
        case 1: pre (1, array);   break;
        case 2: in  (1, array);   break;
        case 3: post(1, array);   break;
    }

    endfunc();
}


void pre(int i, int array[]){
    printf("%d ",array[i]);
    if(array[2*i]!=-1) pre(2*i,array);
    if(array[2*i+1]!=-1) pre(2*i+1,array);
}


void in(int i, int array[]){
    if(array[2*i]!=-1) in(2*i,array);
    printf("%d ",array[i]);
    if(array[2*i+1]!=-1) in(2*i+1,array);
}


void post(int i, int array[]){
    if(array[2*i]!=-1) post(2*i,array);
    if(array[2*i+1]!=-1) post(2*i+1,array);
    printf("%d ",array[i]);
}


void deleted(int array[]){
    int value, i=1;
    gotoxy(10,10); printf("Enter the value: ");
    scanf("%d",&value);
    while(array[i]!=-1){
        if(value<array[i]){
            i*=2;
        }
        else if (value==array[i]) break;
        else{
            i=2*i+1;
        }
    }
    del(i, value, array);
    gotoxy(10,10); printf("The value was successfully deleted");
    endfunc();
}


void del(int i, int value, int array[]){
    if(array[2*i]==-1 && array[2*i+1]==-1) array[i]=-1;
    else if (array[2*i+1]==-1){
        delshiftright(array,i);
    }
    else if (array[2*i]==-1){
        delshiftleft(array,i);
    }
    else{
        neardel=array[2*i+1];
        getneardel(i,array[i],array);
        array[i]=neardel;
        i=2*i+1;
        while(array[i]!=-1){
        if(neardel<array[i]){
            i*=2;
        }
        else if (neardel==array[i]) break;
        else{
            i=2*i+1;
            }
        }
        del(i,neardel,array);
    }
}


void getneardel(int i, int value, int array[]){
    if(array[2*i]!=-1) getneardel(2*i, value, array);
    if(array[2*i+1]!=-1) getneardel(2*i+1, value, array);
    if(array[i]>value && array[i]<neardel) neardel=array[i];
}


void delshiftright(int array[], int i){
    if(array[2*i]==-1 && array[2*i+1]==-1){
        array[i]=-1;
        return;
    }
    if(array[2*i]!=-1){
            array[i]=array[2*i];
            array[2*i]=-1;
            delshiftright(array,2*i);
    }
    if(array[2*i+1]!=-1){
            array[i+1]=array[2*i+1];
            array[2*i+1]=-1;
            delshiftright(array,2*i+1);
    }
}


void delshiftleft(int array[], int i){
    if(array[2*i]==-1 && array[2*i+1]==-1){
        array[i]=-1;
        return;
    }
    if(array[2*i]!=-1){
            array[i-1]=array[2*i];
            array[2*i]=-1;
            delshiftleft(array,2*i);
    }
    if(array[2*i+1]!=-1){
            array[i]=array[2*i+1];
            array[2*i+1]=-1;
            delshiftleft(array,2*i+1);
    }
}


void maxheight(int j, int array[]){
    int i=1, h=0;
    int value=array[j];
    if(array[2*j]!=-1) maxheight(2*j,array);
    if(array[2*j+1]!=-1) maxheight(2*j+1,array);
    while(array[i]!=-1){
        if(value<array[i]){
            i*=2;
            h++;
        }
        else if(value==array[i]){
            if(h>maxh) maxh=h;
            return;
        }
        else{
            i=2*i+1;
            h++;
        }
    }
}


void printmaxh(int array[]){
    maxheight(1, array);
    gotoxy(10,10); printf("The height of the tree is %d",maxh);
    endfunc();
}


void endfunc(){
    gotoxy(10,11);
    printf("Press any key to go back to menu");
    getch();
    system("CLS");
}
