#pragma once

#include "stack.h"


int produce_item()
{
    return 5;
}

void insert_item(int i, Stack* s)
{
    push(i, s);
}