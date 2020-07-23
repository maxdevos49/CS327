#include <iostream>
#include <ncurses.h>
#include <cmath>

#include "headers/Renderer.h"

#undef COLOR_BLACK
#define COLOR_BLACK 232

/**
 * Constructs a renderer based on the screen width and screen height
 * */
Renderer::Renderer(int givenScreen_width, int givenScreen_height)
{
    screen_width = givenScreen_width;
    screen_height = givenScreen_height;
    init_terminal();
}

/**
 * Destructs the renderer and cleans up ncurses
 * */
Renderer::~Renderer()
{
    endwin();
}

/**
 * Shuts off ncurses
 * */
void Renderer::clean_terminal()
{
    endwin();
}

/**
 * Enables ncurses
 * */
void Renderer::init_terminal()
{
    initscr();
    curs_set(0);
    start_color();
    raw();
    noecho();
    keypad(stdscr, 1);
    cbreak();
    timeout(0);

    //default background
    init_pair(BG_RED, COLOR_WHITE, COLOR_RED);
    init_pair(BG_YELLOW, COLOR_WHITE, COLOR_YELLOW);
    init_pair(BG_GREEN, COLOR_WHITE, COLOR_GREEN);
    init_pair(BG_BLUE, COLOR_WHITE, COLOR_BLUE);
    init_pair(BG_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(BG_WHITE, COLOR_BLACK, COLOR_WHITE);

    //default foregrounds
    init_pair(FG_RED, COLOR_RED, COLOR_BLACK);
    init_pair(FG_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(FG_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(FG_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(FG_BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(FG_WHITE, COLOR_WHITE, COLOR_BLACK);
}

/**
 * Clears the entire window with spaces
 * */
void Renderer::clear_window()
{
    int i, j;
    for(i = 0; i < screen_height; i++)
        for(j = 0; j < screen_width; j++)
            mvaddch(i,j,' ');
}



void Renderer::draw_rect_color(int givenX, int givenY, int givenW, int givenH, int givenColor)
{
    attron(COLOR_PAIR(givenColor));
    draw_rect(givenX, givenY, givenW, givenH);
    attroff(COLOR_PAIR(givenColor));
}

void Renderer::draw_rect_char_color(int givenX, int givenY, int givenW, int givenH, char givenChar, int givenColor)
{
    attron(COLOR_PAIR(givenColor));
    draw_rect_char(givenX, givenY, givenW, givenH, givenChar);
    attroff(COLOR_PAIR(givenColor));
}

/**
 * Draws a rectangle with spaces
 * */
void Renderer::draw_rect(int givenX, int givenY, int givenW, int givenH)
{
    int i, j;
    for(i = givenY; i < givenY + givenH; i++)
        for(j = givenX; j < givenX + givenW; j++)
            mvaddch(i,j,' ');
}

void Renderer::draw_rect_char(int givenX, int givenY, int givenW, int givenH, char givenChar)
{
    int i, j;
    for(i = givenY; i < givenY + givenH; i++)
        for(j = givenX; j < givenX + givenW; j++)
            mvaddch(i,j, givenChar);
}

/**
 * Draws a character array at a specific location
 * */
void Renderer::draw_char_array(int givenX, int givenY, int givenW, int givenH,const char &givenArray)
{
    // char *character_array = (char *)givenArray;

    // int i, j, index;
    // for(i = givenY; i < givenY + givenH; i++)
    // {
    //     for(j = givenX; j < givenX + givenW; j++)
    //     {
    //         index = i * givenW + j;
    //         mvaddch(i, j, givenArray[index]);
    //     }
    // }

}

/**
 * Draws a circle based on a radius and a specific location
 * */
void Renderer::draw_circle(int givenX, int givenY, int givenRadius)
{
    //TODO
    //! Probably only draws a quarter of a circle but unsure
    //! Fix when other things are working
    int i, j, distance;
    for(i = givenY; i < givenY + givenRadius; i++)
    {
        for(j = givenX; j < givenX + givenRadius; j++)
        {
            distance = sqrt(pow((givenX - j),2) + pow((givenY - i), 2));
            mvaddch(i,j,'x');
            if(distance < givenRadius)
                mvaddch(i,j,' ');
        }
    }
}