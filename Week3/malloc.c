#include <stdio.h>

int main(int argc, char const *argv[])
{
    int array_length = 10000;

    //create an array of an undetermined size until runtime
    int *p = malloc(array_length * sizeof(int)); //malloc
    int *p2 = calloc(array_length, sizeof(int)); //like malloc but for arrays and inits to all 0's

    size_t array_size;
    int size;
    int *array;
    size = 0;
    array_size = 16;
    array = malloc(array_size * sizeof(*array));
    /*Fill up the array*/
    if (size == array_size)
    {
        /*remember to test!!*/
        array = realloc(array, array_size *= 2);
    }

    return 0;
}
