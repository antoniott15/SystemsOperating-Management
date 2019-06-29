#include <stdio.h>
#include <pthread.h>
#include "functions.h"
#include <math.h>
#include "hrtimer_x86.h"
pthread_mutex_t lock;

volatile unsigned long mutexTas;

tatas_lock_t mutexTaTas;
ticket_lock_t mutex_ticket;
int NUM_HILOS = 2;
void *Without_synchronism(void *args);
void *PointerTrhread(void *args);
void *PointerTrhreadTa(void *args);
void *PointerTrhreadTaTas_wB(void *args);
void *PointerTrhreadTaTas(void *args);
void *PointerTicket(void *args);
long counting = 0;
int main()
{

    pthread_t yarn[NUM_HILOS];
    double end, start, totalTime;
    pthread_mutex_init(&lock, NULL);
    char options;

    printf("1)Without synchronism\n2)Pointer Thread\n3)Test and set\n4)Test and test and set without backoff\n5)Test and test and set\n6)Ticket\n0)Exit\n");
    while (options != '0')
    {
        scanf("%c", &options);
        switch (options)
        {
        case '1':
            start = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_create(&yarn[i], NULL, Without_synchronism, NULL);
            }
            end = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_join(yarn[i], NULL);
            }
            totalTime = fabs(end) - fabs(start);
            printf("Time Without synchronism: %f\n", totalTime);
            break;
        case '2':
            start = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_create(&yarn[i], NULL, PointerTrhread, NULL);
            }
            end = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_join(yarn[i], NULL);
            }
            totalTime = fabs(end) - fabs(start);
            printf("Time Pointer Thread: %f\n", totalTime);
            break;
        case '3':
            start = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_create(&yarn[i], NULL, PointerTrhreadTa, NULL);
            }
            end = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_join(yarn[i], NULL);
            }
            totalTime = fabs(end) - fabs(start);
            printf("Time with test and set: %f\n", totalTime);
            break;
        case '4':
            start = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_create(&yarn[i], NULL, PointerTrhreadTaTas_wB, NULL);
            }
            end = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_join(yarn[i], NULL);
            }
            totalTime = fabs(end) - fabs(start);
            printf("Time with test and test and set without backoff: %f\n", totalTime);
            break;
        case '5':
            start = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_create(&yarn[i], NULL, PointerTrhreadTaTas, NULL);
            }
            end = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_join(yarn[i], NULL);
            }
            totalTime = fabs(end) - fabs(start);
            printf("Time with test and test and set: %f\n", totalTime);
            break;
        case '6':
            start = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_create(&yarn[i], NULL, PointerTicket, NULL);
            }
            end = gethrtime_x86();
            for (int i = 0; i < NUM_HILOS; ++i)
            {
                pthread_join(yarn[i], NULL);
            }
            totalTime = fabs(end) - fabs(start);
            printf("Time with tickets: %f\n", totalTime);
            break;
        default:
            break;
        }
    }

    pthread_mutex_destroy(&lock);

    return 0;
}

void *Without_synchronism(void *args)
{
    for (int i = 0; i < 50000; i++)
    {
        counting++;
    }
}
void *PointerTrhread(void *args)
{

    for (int i = 0; i < 50000; i++)
    {
        pthread_mutex_lock(&lock);
        counting++;
        pthread_mutex_unlock(&lock);
    }

    pthread_exit(0);
}

void *PointerTrhreadTa(void *args)
{

    for (int i = 0; i < 50000; i++)
    {
        tas_lock(&mutexTas);
        counting++;

        tas_unlock(&mutexTas);
    }
    pthread_exit(0);
}

void *PointerTrhreadTaTas_wB(void *args)
{

    for (int i = 0; i < 50000; i++)
    {
        TaTas_lock_wB(&mutexTaTas);
        counting++;
        TaTas_unlock_wB(&mutexTaTas);
    }
    pthread_exit(0);
}

void *PointerTrhreadTaTas(void *args)
{

    for (int i = 0; i < 50000; i++)
    {
        TaTas_lock(&mutexTaTas);
        counting++;
        TaTas_unlock(&mutexTaTas);
    }
    pthread_exit(0);
}

void *PointerTicket(void *args)
{
    for (int i = 0; i < 50000; i++)
    {
        Ticket_Lock(&mutex_ticket);
        counting++;
        Ticket_Unlock(&mutex_ticket);
    }
    pthread_exit(0);
};
