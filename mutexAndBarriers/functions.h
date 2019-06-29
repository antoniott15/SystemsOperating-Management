#ifndef FUNCTIONSATOMICS
#define FUNCTIONSATOMICS

#include "atomic_ops.h"

void tas_lock(volatile unsigned long *mutexTas)
{
    while (tas(mutexTas))
    {
    }
}

void tas_unlock(volatile unsigned long *mutexTas)
{
    *mutexTas = 0;
}

void TaTas_lock_wB(volatile unsigned long *mutexTaTas)
{
    int count = 0;

    tatas_acquire_withoutBackOff(mutexTaTas);
    count++;
}
void TaTas_unlock_wB(volatile unsigned long *mutexTaTas)
{
    tatas_release(mutexTaTas);
}

void TaTas_lock(volatile unsigned long *mutexTaTas)
{
    int count = 0;
    tatas_acquire(mutexTaTas);
    count++;
}
void TaTas_unlock(volatile unsigned long *mutexTaTas)
{
    tatas_release(mutexTaTas);
}

void Ticket_Lock(ticket_lock_t *mutex_ticket)
{
    ticket_acquire(mutex_ticket);
}

void Ticket_Unlock(ticket_lock_t *mutex_ticket)
{
    ticket_release(mutex_ticket);
}

#endif