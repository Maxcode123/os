#pragma once

#include "stack.h"


int remove_item(Stack* s)
{
    return pop(s);
}

void consume_item(int i)
{
    return;
}