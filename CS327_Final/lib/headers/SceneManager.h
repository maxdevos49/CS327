#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <vector>

#include "Scene.h"

class Scene;

class SceneManager
{
private:
    void step_scene();

public:
    std::vector<Scene*> scenes;
    std::vector<Scene*> active_scene;
    SceneManager(std::vector<Scene*> givenScenes);

    void add_scene(Scene *givenScene);
    void pause_scene(char* givenKey);
    void start_scene(char* givenKey);
    void shutdown_scene(char* givenKey);
    void launch_scene(char* givenKey);
};

#endif
