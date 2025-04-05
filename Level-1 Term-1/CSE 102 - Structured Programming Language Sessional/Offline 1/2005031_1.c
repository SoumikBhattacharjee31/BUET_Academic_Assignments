#include <stdio.h>
int weekday(int y, int m, int q);
void printmonth(int m);
void printweekday(int h);

int main(){
    int y, m, q, h;
    printf("Enter year: ");
    scanf("%d",&y);
    printf("Enter month (1-12): ");
    scanf("%d",&m);
    printf("Enter day: ");
    scanf("%d",&q);

    h=weekday(y,m,q);

    printmonth(m);
    printf("%d, %d is ", q, y);
    printweekday(h);
    return 0;
}

int weekday(int y, int m, int q){
    int h;
    if(m==1||m==2){
        m+=12;
        y--;
    }
    h=(q+ (26*(m+1)/10)+ y+ y/4+ 6*(y/100)+ (y/400))%7;
    return h;
}

void printmonth(int m){
    switch(m){
        case 1:  printf("JANUARY ");   break;
        case 2:  printf("FEBRUARY ");  break;
        case 3:  printf("MARCH ");     break;
        case 4:  printf("APRIL ");     break;
        case 5:  printf("MAY ");       break;
        case 6:  printf("JUNE ");      break;
        case 7:  printf("JULY ");      break;
        case 8:  printf("AUGUST ");    break;
        case 9:  printf("SEPTEMBER "); break;
        case 0:  printf("OCTOBER ");   break;
        case 11: printf("NOVEMBER ");  break;
        case 12: printf("DECEMBER ");  break;
    }
}

void printweekday(int h){
    switch(h){
        case 0:  printf("SATURDAY");   break;
        case 1:  printf("SUNDAY");     break;
        case 2:  printf("MONDAY");     break;
        case 3:  printf("TUESDAY");    break;
        case 4:  printf("WEDNESDAY");  break;
        case 5:  printf("THURSDAY");   break;
        case 6:  printf("FRIDAY");     break;
    }
}
