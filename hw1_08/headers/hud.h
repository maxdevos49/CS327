#ifndef HUD_H
#define HUD_H

#include <iostream>

class Hud
{
private:
    std::string status;
    std::string message;
    std::string seed;
    int key;

public:
    Hud();
    ~Hud();

    void status(std::string givenString);
    void message(std::string givenString);
    void seed(std::string givenString);
    void key(std::string givenString);
};

#endif