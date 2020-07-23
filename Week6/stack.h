#ifndef STACK_H
#define STACK_H

typedef struct stack_node stack_node_t;

struct stack_node
{
    int value;
    stack_node_t *next;
};

typedef struct stack
{
    stack_node_t *top;
    int size;
} stack_t;

int stack_init(stack_t *s);
int stack_delete(stack_t *s);
int stack_peek(stack_t *s, int *value);
int stack_push(stack_t *s, int value);
int stack_pop(stack_t *s, int *value);
int stack_size(stack_t *s);
int stack_is_empty(stack_t *s);

#endif