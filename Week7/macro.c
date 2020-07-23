#include <stdio.h>
#include <random.h>

#define MAX_ROOMS 20
#define min(x, y) x < y ? x : y

#define max(x, y) { \
    typeof (x) _x = x; \
    typeof (y) _y = y; \
    _x > _y ? _x : _y; \
}

#define NPC_SMART   0x00000001
#define NPC_TELE    0x00000002
#define NPC_TUNNEL  0x00000004
#define NPC_ERRATIC 0x00000008

int characteristics;
characteristics = rand() % 16;
characteristics = rand() & 0xf;

/*
Could use..
sprintf(s, "%x", characteristics)

Better way..
symbol[characteristics];
*/

/*
0b0000000000000000bbb0
0b00000000000000000001
0b00000000000000000000
*/

#define has_characteristic(character, bit) ((character)->npc->characteristics & NPC_##bit)

if (has_characteristic(npc, SMART)) {
    // This is a smart monster
} else {
    // This is a dumb monster
}

#define to_string(s) #s

#define table_element(e) { e, #e }

struct { int, char * } table[] = {
    table_element(i),
    table_element(j),
    table_element(k),
    table_element(l),
    table_element(m),
    table_element(n);
};

int main(int argc, char *argv[]) {
    MAX_ROOMS;

    min(3, 4);

    if (10 > min(foo(x), tar(x))) {

    }

    max(foo(x), bar(x));

    to_string(foo);

    return 0;
}
