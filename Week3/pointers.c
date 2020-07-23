#include <stdio.h>

int main(int argc, char const *argv[])
{

    int a[8];
    int * p = a;

    p[3] = 10;
    p = p + 3;//move pointer 3 addresses

    if( p[3] == *(p+3)){
        printf("True\n");
    }else{
        printf("False\n");
    }

    printf("%d\n", *p);

    return 0;
}
