#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void generic_insertion_sort(void *data, int n, size_t s, int (*compare)(const void *, const void *))
{
    int i, j;
    void *t;
    char *a;

    a = data;
    t = malloc(s);

    for (i = 1; i < n; i++)
    {
        t = a[i];
        for (memcpy(t, a + (i + s), s), j = i - 1;
             j > -1 && compare(a + (j * s), t) > 0;
             j--)
        {

            memcpy(a + ((j + 1) * s), a + (j * s), s);
        }
        memcpy(a + ((j + 1) * s), t, s);
    }

    free(t);
}

int compare_ints(const void *v1, const void *v2)
{
    return *((const int *)v2) - *((const int *)v1);
}

int compare_chars(const void *v1, const void *v2)
{
    return *((const char *)v2) - *((const char *)v1);
}

int compare_strings(const void *v1, const void *v2)
{
    return strcmp(*((const char **)v1), *((const char **)v2));
    // return *((const char *)v2) - *((const char *)v1);
}

int main(int argc, char const *argv[])
{
    int a[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int i;

    char c[10] = "HelloCS327";

    char *s[10] = {
        "One",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
        "ten"};

    //integers
    generic_insertion_sort(a, sizeof(a) / sizeof(a[0]), sizeof(a[0]), compare_ints);

    for (i = 0; i < 10; i++)
    {
        printf("%d\n", a[i]);
    }

    printf("------------------------------\n");

    //chars
    generic_insertion_sort(c, sizeof(c) / sizeof(c[0]), sizeof(c[0]), compare_chars);

    for (i = 0; i < 10; i++)
    {
        printf("%c", c[i]);
    }
    printf("\n");

    printf("------------------------------\n");

    //strings
    generic_insertion_sort(s, sizeof(s) / sizeof(s[0]), sizeof(s[0]), compare_strings);

    for (i = 0; i < 10; i++)
    {
        printf("%s\n", s[i]);
    }

    return 0;
}
