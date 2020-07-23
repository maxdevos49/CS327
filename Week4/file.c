#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum action
{
    read_binary,
    write_binary,
    read_text,
    write_text
} action_t;

/**
 * Value switches to main are:
 * -wt write_text
 * -rt read_text
 * -wb write_binary
 * -rb read_binary
 */
int main(int argc, char *argv[])
{
    FILE *f;

    struct
    { // Doesn't need a name but can only be used in main().
        int i;
        int j;
    } s = {1, 2};
    action_t action;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <-wt|-wb|-rt|-rb>\n", argv[0]); //<> required argument, [] optional argument.
        return -1;
    }
    else if (!strcmp(argv[1], "-rt")) // strcmp compares strings. Returns 0 if same which equals false.
        action = read_text;
    else if (!strcmp(argv[1], "-rb")) // strcmp compares strings. Returns 0 if same which equals false.
        action = read_binary;
    else if (!strcmp(argv[1], "-wt")) // strcmp compares strings. Returns 0 if same which equals false.
        action = write_text;
    else if (!strcmp(argv[1], "-wb")) // strcmp compares strings. Returns 0 if same which equals false.
        action = write_binary;
    else
    {
        fprintf(stderr, "Invalid command line parameter: %s\n", argv[1]);
        return -2;
    }
    switch (action)
    {
    case write_binary:
        // Open a FILE
        if (!(f = fopen("binary_file", "w")))
        {
            fprintf(stderr, "Failed to open file");
            return -3;
        }

        if (fwrite(&s, sizeof(s), 1, f) != 1)
        {
            fprintf(stderr, "Failed to write to file");
            return -3;
        }
        fclose(f);
        break;

    case read_binary:
        s.i = s.j = 0;
        f = fopen("binary_file", "r");

        fread(&s, sizeof(s), 1, f);

        fclose(f);

        printf("Read the values %d and %d\n", s.i, s.j);
        break;

    case write_text:
        if (!(f = fopen("text_file", "w")))
        {
            fprintf(stderr, "Failed to open file");
            return -3;
        }

        fprintf(f, "%d %d\n", s.i, s.j);
        fclose(f);
        break;

    case read_text:
        s.i = s.j = 0;
        if (!(f = fopen("text_file", "r")))
        {
            fprintf(stderr, "Failed to open file");
            return -3;
        }

        fscanf(f, "%d %d\n", &s.i, &s.j);

        fclose(f);

        printf("Read the values %d and %d\n", s.i, s.j);
        break;
    }
    return 0;
}