#include <stdio.h>

typedef void (*func)(void);

void zero()
{
    printf("%s\n", __FUNCTION__);
}

void one()
{
    printf("%s\n", __FUNCTION__);
}

void two()
{
    printf("%s\n", __FUNCTION__);
}

void three()
{
    printf("%s\n", __FUNCTION__);
}

void four()
{
    printf("%s\n", __FUNCTION__);
}

int main(int argc, char *argv[])
{
    func table[] = {
        zero,
        one,
        two,
        three,
        four};

    int i;

    for (i = 0; i < 5; i++)
    {
        table[i]();
    }

    return 0;
}
