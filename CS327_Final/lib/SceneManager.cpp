#include <iostream>
#include <vector>
#include <cstring>
#include <ncurses.h>
#include <unistd.h>

#include "headers/Scene.h"
#include "headers/SceneManager.h"

/**
 * Constructs a Scene manager with a given list of scenes
 * */
SceneManager::SceneManager(std::vector<Scene*> givenScenes)
{
    scenes = givenScenes;
    active_scene.push_back(givenScenes[0]);
 }

/**
 * Adds a scene to the scene manager
 * */
void SceneManager::add_scene(Scene *givenScene)
{
    scenes.push_back(givenScene);
}

/**
 * Removes a scene from the active scenes
 * */
void SceneManager::shutdown_scene(char* givenKey)
{
    int i;
    for(i = 0; i < active_scene.size(); i++)
    {
        if(strcmp(active_scene[i]->key, givenKey))
        {
            active_scene[i]->state = ready;
            active_scene.erase(active_scene.begin()+i + 1);
        }
    }
}

/**
 * Pauses the execution of a given scene if it is active
 * */
void SceneManager::pause_scene(char* givenKey)
{
    int i;
    for(i = 0; i < active_scene.size(); i++)
        if(strcmp(active_scene[i]->key, givenKey) && active_scene[i]->state == active)
            active_scene[i]->state = paused;
}

/**
 * Starts a scene if it was not already active and shuts down all other active scenes
 * */
void SceneManager::start_scene(char* givenKey)
{
    int i, j;
    for(i = 0; i < scenes.size(); i++)
    {   
        if(strcmp(scenes[i]->key, givenKey) == 0){

            //shutdown other scenes
            for(j = 0; j < active_scene.size(); j++)
            {
                shutdown_scene(active_scene[j]->key);
            }

            //empty the array
            active_scene.clear();

            //add new scene
            active_scene.push_back(scenes[i]);
            active_scene[0]->state = active;

            //clear the screen
            active_scene[0]->renderer->clear_window();

            //call create
            active_scene[0]->create();

            //start draw and update loop
            step_scene();
        }
    }
}

/**
 * Starts a scene in parallel with other scenes
 * */
void SceneManager::launch_scene(char* givenKey)
{

    int i;
    for(i = 0; i < active_scene.size(); i++)//prevent duping a scene
        if(strcmp(active_scene[i]->key, givenKey) == 0)
            return;

    for(i = 0; i < scenes.size(); i++)
        if(strcmp(scenes[i]->key, givenKey) == 0)
            active_scene.push_back(scenes[i]);

    for(i = 0; i < active_scene.size(); i++)
    {   
        if(strcmp(active_scene[i]->key, givenKey) == 0){

            //add new scene
            active_scene[i]->state = active;

            //call create
            active_scene[i]->create();
        }
    }
}

long current_milliseconds()
{
    std::__1::chrono::time_point<std::__1::chrono::system_clock> time = std::chrono::system_clock::now();
    std::__1::chrono::microseconds since_epoch = time.time_since_epoch();
    std::__1::chrono::milliseconds millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
    return millis.count();
}

// const int FRAMES_PER_SECOND = 100;

void SceneManager::step_scene()
{
    //allow to get milliseconds
    

    int i, key; 
    int sleep_time = 0;
    long next_game_tick = current_milliseconds();

    while(1)
    {
        key = active_scene[0]->global_input();

        if(active_scene[0]->state == quiting)
            break;

        for(i = 0; i < active_scene.size(); i++)
        {

            if(active_scene[i]->state != paused)
            {
                active_scene[i]->input(key);
                active_scene[i]->update();
                active_scene[i]->render();
            }
            else
            {
                attron(COLOR_PAIR(BG_BLUE));
                mvprintw(i,1, "Scene %s Paused", active_scene[i]->key);
                attroff(COLOR_PAIR(BG_BLUE));
            }
   
        }

        refresh();

        //limit framerate here
        next_game_tick += 1000 / 60;
        sleep_time = next_game_tick - current_milliseconds();
        if(sleep_time >= 0)
            usleep(sleep_time * 1000);

    }
    
}