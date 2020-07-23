#include <stdio.h>

void times_table(void);

int main(int argc, char *argv[])
{
    times_table();

    return 0;
}

void times_table(void)
{
    int table[10][10];
    int i, j;

    for(j = 0; j < 10; j++){
        for(i = 0; i < 10; i++){
            table[j][i] = (i + 1) * (j + 1);
        }
    }

    for(j = 0; j < 10; j++){
        for(i = 0; i < 10; i++){
            printf("%3d ", table[j][i]);
        }
        printf("\n");
    }
}