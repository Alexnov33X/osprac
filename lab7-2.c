#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int a = 0;

void* mythread(void* dummy)
{
    pthread_t mythid;
    mythid = pthread_self();
    a = a + 1;
    printf("Thread %ld, Calculation result = %d\n", mythid, a);
    return NULL;
}
//Жалуется на некоторые warning-и, но работает. Хотя вроде ничего запретного тут не делаю

//исправил всё
int main()
{
    pthread_t thid, thid2, mythid;
    int result;

    result = pthread_create(&thid, (pthread_attr_t*)NULL, mythread, NULL);
    if (result != 0) {
        printf("Error on thread create, return value = %d\n", result);
        exit(-1);
    }
    printf("2nd thread created, thid = %ld\n", thid);

    result = pthread_create(&thid2, (pthread_attr_t*)NULL, mythread, NULL);
    if (result != 0) {
        printf("Error on thread create, return value = %d\n", result);
        exit(-1);
    }
    printf("3rd thread created, thid = %ld\n", thid2);

    mythid = pthread_self();
    a = a + 1;
    printf("Thread %ld, Calculation result = %d\n", mythid, a);

    pthread_join(thid, (void**)NULL);
    pthread_join(thid2, (void**)NULL);
    return 0;
}