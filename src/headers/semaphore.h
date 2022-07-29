#pragma once

#include <stdatomic.h>

#define SET 1
#define CLEAR 0

typedef volatile struct {
    atomic_int value;
    atomic_flag mutex;
} Semaphore;


void acquire(volatile atomic_flag* m)
{
    // infinite loop while mutex is SET
    while (atomic_flag_test_and_set(m) == SET);
}

void release(volatile atomic_flag* m)
{
    // set mutex to CLEAR
    atomic_flag_clear(m);
}

void sem_mutex_init(Semaphore* s)
{
    s->value = 1;
    atomic_flag_clear(&s->mutex);
}

void sem_empty_init(Semaphore* s, int N)
{
    s->value = N;
    atomic_flag_clear(&s->mutex);
}

void sem_full_init(Semaphore* s)
{
    s->value = 0;
    atomic_flag_clear(&s->mutex);
}

void down(Semaphore* s)
{
    acquire(&s->mutex);
    while (atomic_load(&s->value) == 0) ; // if value is 0, sleep
    atomic_fetch_sub(&s->value, 1); // else decrement by 1
    release(&s->mutex);
}

void up(Semaphore* s)
{
    atomic_fetch_add(&s->value, 1);
}
