#include "headers/stack.h"

void push(int value, Stack* s)
{
    if (full(s))
    {
        printf("Stack overflow! Cannot push in a full stack.\n");
        return;
    }
    s->s_array[s->ind] = value;
    s->ind++;
}

int pop(Stack* s)
{
    if (empty(s))
    {
        printf("Stack is empty! Cannot pop in a empty stack.\n");
        return;
    }
    int value = s->s_array[s->ind-1];
    s->s_array[s->ind-1] = INIT_VALUE;
    s->ind--;
    return value;
}

int empty(Stack* s)
{
    if (s->ind == 0)
    {
        return 1;
    }
    return 0;
}

int full(Stack* s)
{
    if (s->ind == STACK_LENGTH)
    {
        return 1;
    }
    return 0;
}

void clear(Stack* s)
{
    if (empty(s))
    {
        printf("Stack is empty! Cannot clear an empty.\n");
        return;
    }
    for (int i = s->ind-1; i>=0; i--)
    {
        s->s_array[i] = INIT_VALUE;
    }
    s->ind = 0;
}

void stack_init(Stack* s)
{
    for (int i = 0; i < STACK_LENGTH; i++)
    {
        s->s_array[i] = INIT_VALUE;
    }
    s->ind = 0;
}

void show(Stack* s)
{
    for (int i = 0; i < s->ind; i++)
    {
        printf("%d\n", s->s_array[i]);
    }
}