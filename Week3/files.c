#include <stdio.h>

typedef enum action
{
    read_binary,
    write_binary,
    read_text,
    write_text
} action_t;

int main(int argc, char const *argv[])
{
    /**
     * r = read
     * w = write
     * a = append
     * b = binary for windows only
     * */
    // FILE fopen(const char *path, const char *mode);

    //fclose(FILE *fp)

    //binary

    //returns num bytes succeded
    // size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

    //
    // size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

    FILE *f;

    struct
    {
        int i;
        int j;
    } s = {1, 2};

    if (!(f = fopen("binary_file", "w")))
    {
        fprintf(stderr, "Failed to open file.");

        return -1;
    }

    if (!(fwrite(&s, sizeof(s), 1, f)))
    {
        fprintf(stderr, "Failed to write to file.");
        return -1;
    }

    s.i = 0;
    s.j = 0;
    fclose(f);
    if (!(f = fopen("binary_file", "r")))
    {
        fprintf(stderr, "Failed to open file.");

        return -1;
    }

    fread(&s, sizeof(s), 1, f);

    printf("I: %d, J: %d\n", s.i, s.j);

    return 0;
}
