#pragma once

#include <stdio.h>

#define STACK_LENGTH 5
#define INIT_VALUE -1

typedef struct {
    int s_array[STACK_LENGTH]; // array holding stack items
    int ind; // current index
} Stack;

void push(int value, Stack* s);
int pop(Stack* s);
int empty(Stack* s);
int full(Stack* s);
void clear(Stack* s);
void stack_init(Stack* s);
void show(Stack* s);
