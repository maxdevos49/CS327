/**
 * Dungeon Header
 * Author: Maxwell DeVos
 * Language: C
 * */

//defines the maps width in characters
#define MAP_WIDTH 81
//defines the maps height in characters
#define MAP_HEIGHT 20
//defines the minimum number of rooms
#define MIN_ROOMS 6
//defines the minimum room width in characters
#define MIN_ROOM_WIDTH 4
//defines the minimum room height in characters
#define MIN_ROOM_HEIGHT 3
//defines the maximum room size in characters
#define MAX_ROOM_SIZE 10
///The amount of times a room will be atempted to be placed
#define MAX_ROOM_ATTEMPTS 1000
//defines the seed to use for generating the map
#define SEED 1548866650


/**
 * Room Structure
 * */
typedef struct room
{
    int x;
    int y;
    int width;
    int height;
} room;

/**
 * Cell Structure
 * */
typedef struct cell
{
    int h;
    int c;
} cell;

/**
 * Enum of character values for storing in map array
 * */
enum characters
{
    c_rock,
    c_h_wall,
    c_v_wall,
    c_room,
    c_corridor,
    c_player,
    c_u_stairs,
    c_d_stairs
};

/**
 * Fills in the map with its initial values and adds borders.
 * */
void fill_map(void *map_array);

/**
 * Adds a room to the map.
 * returns 0 if successful. 1 if it fails.
 * */
int add_room(room *r, int index, void *map_array);

/**
 * Creates a corridor between two rooms
 * */
void place_corridors(room *r, int index, void *map_array);

/**
 * Applys the position of the cursor drawing corridors to the map array
 * */
void add_corridor(void *map_array, int x, int y);

/**
 * Connects all rooms with corridors
 * */
int place_rooms(room *r, void *map_array);

/**
 * Adds the player to the map
 * */
void place_player(void *map_array);

/**
 * Adds stairs to the map
 * */
void place_stairs(void *map_array);

/**
 * --------------------------Temporary in this location for testing--------------------------
 * Draws the map based on the given map array
 * */
void draw_map(void *map_array);