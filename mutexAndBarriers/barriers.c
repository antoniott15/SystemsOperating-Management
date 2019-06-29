#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "functions.h"
#include "hrtimer_x86.h"
#define TESTINGBARRIER 1000
#define NUM_HILOS 2
pthread_barrier_t barrera;
volatile unsigned long lock;
void *barrierFuncLocal(void *);
void *barrierFuncReverse(void *);
void *barrierFuncPthread(void *);
void callFunctionPthread(int hilos);
void callFunctionReverse(int hilos);
void callFunctionLocal(int hilos);
int main()
{
    pthread_t yarn[NUM_HILOS];
    lock = 0;
    double end, start, totalTime;
    pthread_barrier_init(&barrera, NULL, NUM_HILOS);
    int threadId[NUM_HILOS];
    char options;

    printf("1)Pthread_Barrie\n2)Reverse Sense Barrier\n3)Local sense Barrier \n0)Exit\n");
    while (options != '0')
    {
        scanf("%c", &options);
        switch (options)
        {
        case '1':
            start = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                threadId[i] = i;
                pthread_create(&yarn[i], NULL, barrierFuncPthread, &threadId[i]);
            }
            end = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_join(yarn[i], NULL);
            }
            totalTime = fabs(end) - fabs(start);
            printf("Time with Pthread: %f\n", totalTime);
            break;
        case '2':
            start = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                threadId[i] = i;
                pthread_create(&yarn[i], NULL, barrierFuncReverse, &threadId[i]);
            }
            end = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_join(yarn[i], NULL);
            }
            totalTime = fabs(end) - fabs(start);
            printf("Time with Reverse sense: %f\n", totalTime);
            break;
        case '3':
            start = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                threadId[i] = i;
                pthread_create(&yarn[i], NULL, barrierFuncLocal, &threadId[i]);
            }
            end = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_join(yarn[i], NULL);
            }
            totalTime = fabs(end) - fabs(start);
            printf("Time with Local sense: %f\n", totalTime);
            break;
        default:
            break;
        }
    }

    return 0;
}

void *barrierFuncReverse(void *data)
{

    for (int i = 0; i < TESTINGBARRIER; ++i)
    {
        callFunctionReverse(NUM_HILOS);
    }
};

void *barrierFuncLocal(void *data)
{

    for (int i = 0; i < TESTINGBARRIER; ++i)
    {
        callFunctionLocal(NUM_HILOS);
    }
};

void *barrierFuncPthread(void *data)
{

    for (int i = 0; i < TESTINGBARRIER; ++i)
    {
        callFunctionPthread(NUM_HILOS);
    }
};

void callFunctionPthread(int hilos)
{
    pthread_barrier_wait(&barrera);
}

void callFunctionReverse(int hilos)
{
    static int counter = 0;
    static int flag = 0;
    int local_sense = flag;

    local_sense = !local_sense;

    TaTas_lock(&lock);

    counter++;

    if (counter == hilos)
    {
        TaTas_unlock(&lock);
        counter = 0;
        flag = local_sense;
    }
    else
    {
        TaTas_unlock(&lock);
        while (flag != local_sense)
        {
        };
    }
}

void callFunctionLocal(int hilos)
{
    static int counter = 0;
    static int flag = 0;
    int mycount = 0;

    TaTas_lock(&lock);
    if (counter == 0)
    {
        flag = 0;
    }
    mycount = counter++;
    TaTas_unlock(&lock);

    if (mycount == hilos)
    {
        counter = 0;
        flag = 1;
    }
    while (flag == 0)
    {
    }
}
