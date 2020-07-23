#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

int main(int argc, char const *argv[])
{
    stack_t s;
    int i;

    stack_init(&s);

    for (i = 0; i< 5; i++)
    {
        stack_push(&s, i);
    }

    stack_peek(&s, &i);
    printf("%d\n", i);

    while(!stack_peek(&s, &i)){
        printf("%d\n", i);
        stack_pop(&s, &i);
    }

    printf("%d\n", stack_size(&s));

    return 0;
}