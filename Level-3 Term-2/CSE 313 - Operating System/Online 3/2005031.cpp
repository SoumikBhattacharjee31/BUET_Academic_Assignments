#include<bits/stdc++.h>
#include<chrono>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
using namespace std;

pthread_mutex_t mtx;
int a=1,b=1,c=1,n=0;

void *_thread(void * arg) {
    int id = *(int*)arg;
    while(1) {
        pthread_mutex_lock(&mtx);
        if(n<=0) break;
        if(a+b+c==0){
            a=b=c=1;
            cout<<endl;
            n--;
            pthread_mutex_unlock(&mtx);
            continue;
        }
        if(id==0)a=0;
        else if(id==1)b=0;
        else if(id==2)c=0;
        cout << (char)('A'+id);
        pthread_mutex_unlock(&mtx);
    }
    pthread_mutex_unlock(&mtx);
    return nullptr;
}

int main(int argc, char *argv[]) {
    if (argc == 2) n = atoi(argv[1]);
    else cin >> n;

    pthread_t t1, t2, t3;
    pthread_mutex_init(&mtx, 0);
    int a[] = {0, 1, 2};
    
    pthread_create(&t1,NULL,_thread,&a[0]);
    pthread_create(&t2,NULL,_thread,&a[1]);
    pthread_create(&t3,NULL,_thread,&a[2]);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&mtx);
    return 0;
}