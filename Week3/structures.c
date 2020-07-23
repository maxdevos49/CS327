#include <stdio.h>

struct room //can be used as "struct room r"
{
    int x_pos;
    int y_pos;
    int x_size;
    int y_size;
};

typedef struct //can be used as "room_2 r"
{
    int x_pos;
    int y_pos;
    int x_size;
    int y_size;
} room_2;

int main(int argc, char const *argv[])
{

    return 0;
}

void create_room(struct room *r)
{
    (*r).x_pos = 10; //works
    r->y_pos = 10;   //used for structs and is cleaner
    //   -> also dereference operator
}
