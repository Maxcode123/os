#include <stdio.h>

#include "headers/stack.h"

int main()
{   
    Stack s;
    stack_init(&s);
    push(1, &s);
    push(2, &s);
    push(5, &s);
    pop(&s);
    pop(&s);
    pop(&s);
    pop(&s);

    show(&s);
}