#ifndef SCENE_H
#define SCENE_H

#include <ncurses.h>

#include "SceneManager.h"
#include "Renderer.h"

#include "../../state.h"

class SceneManager;
class Renderer;

enum SceneState{
    not_ready,
    ready,
    active,
    paused,
    quiting
};

class Scene
{
public:
    SceneManager *manager;
    Renderer *renderer;
    State *data;

    char* key;
    SceneState state;

    Scene()
    {
        key = (char*)"";
        state = not_ready;
    };

    virtual ~Scene(){};

    virtual void input(int key) {};
    virtual void create() {};
    virtual void update() {};
    virtual void render() {};

    /**
     * Handles the global scene inputs
     * */
    int global_input()
    {
        int key;
        switch(key = getch())
        {
            case 'q':
                state = quiting;
                return ERR;
                break;
            case 'p':
                state = (state != paused) ? paused : active;
                return ERR;
                break;
            default:
                return key;
        }

        return 0;
    };

    /**
     * Adds the Scene manager via dependency injection
     * */
    void add_scene_manager(SceneManager *givenManager)
    {
        manager = givenManager;
    };

    /**
     * Adds the Renderer via dependency injection
     * */
    void add_renderer(Renderer *givenRenderer)
    {
        renderer = givenRenderer;
    };

    /**
     * Adds the state manager via dependency injection
     * */
    void add_state_manager(State *givenStateManager)
    {
        data = givenStateManager;
    };
};

#endif
