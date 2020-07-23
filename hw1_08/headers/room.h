#ifndef ROOM_H
#define ROOM_H

#include <position.h>;
#include <dimension.h>;

/**
 * Room Class
 * Represents a room
 * */
class Room
{
public:
    Room();

    bool connected;
    bool occupied;
    Position pos;
    Dimensions dim;
};

#endif