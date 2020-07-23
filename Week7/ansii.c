#include <ncurses.h>

int main(int argc, char const *argv[])
{
    int i;
    char a[4] = "\\|/-";
    initscr();

    for (i = 0; i < 320; i++)
    {
        usleep(125000);
        // i % 4 mvaddch(23, i / 4, a[i]);
        refresh();
    }

    endwin();

    return 0;
}

void io_init_terminal(void)
{
    initscr();  //inits terminal
    raw();         //turns off buffer
    noecho();       //doesnt echo control keys
    curs_set(0);
    keypad(stdscr, TRUE); //used key pad
}

//endwin() //deinits
//clear() //clears screen
//mvsaddch(int y, int x, consdt chtype ch);
//refresh()//draws buffer

