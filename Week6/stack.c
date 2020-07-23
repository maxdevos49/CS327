#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int stack_init(stack_t *s)
{
    s->size = 0;
    s->top = NULL;
}
int stack_delete(stack_t *s)
{
    stack_node_t *n;
    s->size = 0;

    for(n = s->top; n; n=s->top){
        s->top = n->next;
        free(n);
    }
}
int stack_peek(stack_t *s, int *value)
{
    if (s->top)
    {
        value = s->top->value;
        return 0;
    }
    return 1;
}
int stack_push(stack_t *s, int value)
{
    stack_node_t *n;

    if (!(n = malloc(sizeof(*n))))
    {
        return 1;
    }
    n->value = value;
    n->next = s->top;
    s->top = n;
    s->size++;

    return 0;
}
int stack_pop(stack_t *s, int *value)
{
    stack_node_t *n;
    if(!s->top){
        return 1;
    }

    n = s->top;
    s->top = n->next;
    *value = n->value;
    s->size--;
    free(n);

    return 0;
}
int stack_size(stack_t *s)
{
    return s->size;
}
int stack_is_empty(stack_t *s)
{
    return !s->top;
}
