#include<stdio.h>

/*void mystrcpy(char t[], char s[]) {
    int i = 0;
    while((t[i]=s[i])) {
        i++;
    }
}*/

void mystrcpy(char *t, char *s)
{
    while( (*t++ = *s++) );
}

/* int mystrlen(char s[]) {
    int i = 0;
    while(s[i]) {
        i++;
    }
    return i;
}*/

int mystrlen(char *s)
{
    char *st = s;
    while(*s++);
    return s  - st - 1;
}

/*void mystrcat(char t[], char s[]) {
    int i = 0;
    int j = mystrlen(t);
    while((t[j]=s[i])) {
        i++;
        j++;
    }
}*/

void mystrcat(char *t, char *s)
{
    //t+=mystrlen(t);
    while(*t++);
    t--;
    while( (*t++ = *s++) );
}

/*int mystrcmp(char t[], char s[]) {
    int i = 0;
	while(s[i]  && t[i] && s[i]==t[i] ){
		i++;
	}
	return s[i]-t[i];
}*/

int mystrcmp(char *t, char *s)
{
    while(*s  && *t && *s++ == *t++);
    return *s - *t;
}

int main()
{
    char s[80], t[80];
    char u[80] = "C string ";
    int len, cmp;
    printf("Enter your string: ");
    gets(s);
    mystrcpy(t,s);
    printf("Your copied string: %s\n", t);
    mystrcat(u,t);
    printf("Your concatenated string: %s\n", u);
    len = mystrlen(u);
    printf("Length of your concatenated string: %d\n", len);
    cmp = mystrcmp(s,t);
    printf("Comparing %s and %s : %d\n", s, t, cmp);
    cmp = mystrcmp(u,t);
    printf("Comparing %s and %s : %d\n", u, t, cmp);
    return 0;
}
