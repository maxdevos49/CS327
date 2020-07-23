#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

/* Do not modify write_pgm() or read_pgm() */
int write_pgm(char *file, void *image, uint32_t x, uint32_t y)
{
    FILE *o;

    if (!(o = fopen(file, "w")))
    {
        perror(file);

        return -1;
    }

    fprintf(o, "P5\n%u %u\n255\n", x, y);

    /* Assume input data is correctly formatted. *
   * There's no way to handle it, otherwise.   */

    if (fwrite(image, 1, x * y, o) != (x * y))
    {
        perror("fwrite");
        fclose(o);

        return -1;
    }

    fclose(o);

    return 0;
}

/* A better implementation of this function would read the image dimensions *
 * from the input and allocate the storage, setting x and y so that the     *
 * user can determine the size of the file at runtime.  In order to         *
 * minimize complication, I've written this version to require the user to  *
 * know the size of the image in advance.                                   */
int read_pgm(char *file, void *image, uint32_t x, uint32_t y)
{
    FILE *f;
    char s[80];
    unsigned i, j;

    if (!(f = fopen(file, "r")))
    {
        perror(file);

        return -1;
    }

    if (!fgets(s, 80, f) || strncmp(s, "P5", 2))
    {
        fprintf(stderr, "Expected P6\n");

        return -1;
    }

    /* Eat comments */
    do
    {
        fgets(s, 80, f);
    } while (s[0] == '#');

    if (sscanf(s, "%u %u", &i, &j) != 2 || i != x || j != y)
    {
        fprintf(stderr, "Expected x and y dimensions %u %u\n", x, y);
        fclose(f);

        return -1;
    }

    /* Eat comments */
    do
    {
        fgets(s, 80, f);
    } while (s[0] == '#');

    if (strncmp(s, "255", 3))
    {
        fprintf(stderr, "Expected 255\n");
        fclose(f);

        return -1;
    }

    if (fread(image, 1, x * y, f) != x * y)
    {
        perror("fread");
        fclose(f);

        return -1;
    }

    fclose(f);

    return 0;
}

int apply_square_matrix(int matrix_size, int matrix[matrix_size][matrix_size], int image_x, int image_y, uint8_t image[image_x][image_y], int x, int y)
{
    int sum = 0;
    int i, j;

    //loop through matrix
    for (i = 0; i < matrix_size; i++)
    {
        for (j = 0; j < matrix_size; j++)
        {
            //sum up the matrix math
            sum += matrix[i][j] * image[x + (i - 1)][y + (j - 1)];
        }
    }

    return sum;
}

int main(int argc, char *argv[])
{
    //check for am image
    if (argv[1])
    {
        int size_x = 1024;
        int size_y = 1024;

        uint8_t image[size_x][size_y];
        uint8_t out[size_x][size_y];

        //read image
        read_pgm(argv[1], image, size_x, size_y);

        int matrix_x[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}};

        int matrix_y[3][3] = {
            {-1, -2, -1},
            { 0,  0,  0},
            { 1,  2,  1}};

        for (int i = 1; i < size_x; i++)
        {
            for (int j = 1; j < size_y; j++)
            {
                //apply matrix's
                int result_x = pow(apply_square_matrix(3, matrix_x, size_x, size_y, image, i, j), 2);
                int result_y = pow(apply_square_matrix(3, matrix_y, size_x, size_y, image, i, j), 2);

                //combine x and y
                int result = sqrt(result_x + result_y);

                //clamp
                if (result > 255)
                {
                    result = 255;
                }
                else if (result < 0)
                {
                    result = 0;
                }

                //save result into output
                out[i][j] = result;
            }
        }
        //write output image
        write_pgm("sobel.pgm", out, size_x, size_y);
    }
    else
    {
        printf("This program needs an argument of the image type 'example.pgm'.\n");
    }

    return 0;
}