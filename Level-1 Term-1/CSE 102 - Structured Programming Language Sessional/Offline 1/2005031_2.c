#include <stdio.h>
void calender(int m, int y);
void printmonth(int m);
int initialweekday(int y, int m);
int daysinamonth(int m, int y);

int main(){
    int y, m;
    printf("Enter year: ");
    scanf("%d",&y);
    printf("Enter month (1-12): ");
    scanf("%d",&m);
    calender(m,y);
    return 0;
}

void calender(int m, int y){
    int h, lastday, days;
    h=initialweekday(y,m);
    lastday=daysinamonth(m,y);

    printf("\nCalender for:\n\n");
    printmonth(m);
    printf("%d\n\n",y);

    printf("SUN  MON  TUE  WED  THU  FRI  SAT\n");
    for(days=0;days<h;days++) printf("     ");
    for(days=1;days<=lastday;days++,h++){
        if(h>6) printf("\n"), h=0;
        printf("%-5d",days);
    }
    printf("\n");
}

void printmonth(int m){
    switch(m){
        case 1:  printf("JANUARY, ");   break;
        case 2:  printf("FEBRUARY, ");  break;
        case 3:  printf("MARCH, ");     break;
        case 4:  printf("APRIL, ");     break;
        case 5:  printf("MAY, ");       break;
        case 6:  printf("JUNE, ");      break;
        case 7:  printf("JULY, ");      break;
        case 8:  printf("AUGUST, ");    break;
        case 9:  printf("SEPTEMBER, "); break;
        case 0:  printf("OCTOBER, ");   break;
        case 11: printf("NOVEMBER, ");  break;
        case 12: printf("DECEMBER, ");  break;
    }
}

int initialweekday(int y, int m){
    int h;
    if(m==1||m==2) m+=12, y--;
    h=(1+ (26*(m+1)/10)+ y+ y/4+ 6*(y/100)+ (y/400))%7-1;
    if(h==-1) h=6;
    return h;
}

int daysinamonth(int m, int y){
    switch(m){
        case 4:
        case 6:
        case 9:
        case 11: return 31;                                         break;
        case 2:  return ((y%4==0 && y%100!=0)|| y%400==0)?29:28;    break;
        default: return 30;                                         break;
    }
}
