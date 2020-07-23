#ifndef RENDERER_H
#define RENDERER_H

enum color
{
    BG_RED = 1,
    BG_ORANGE,
    BG_YELLOW,
    BG_GREEN,
    BG_BLUE,
    BG_PURPLE,
    BG_WHITE,
    BG_BLACK,

    FG_RED,
    FG_ORANGE,
    FG_YELLOW,
    FG_GREEN,
    FG_BLUE,
    FG_PURPLE,
    FG_WHITE,
    FG_BLACK
};

class Renderer
{
public:
    int screen_width;
    int screen_height;

    Renderer(int givenScreen_width, int givenScreen_height);
    ~Renderer();

    void init_terminal();
    void clean_terminal();
    void clear_window();

    void draw_rect(int givenX, int givenY, int givenW, int givenH);
    void draw_rect_char(int givenX, int givenY, int givenW, int givenH, char givenChar);
    void draw_rect_color(int givenX, int givenY, int givenW, int givenH, int givenColor);
    void draw_rect_char_color(int givenX, int givenY, int givenW, int givenH, char givenChar, int givenColor);
    void draw_char_array(int givenX, int givenY, int givenW, int givenH, const char &givenArray);
    void draw_circle(int givenX, int givenY, int givenRadius);
};

#endif
