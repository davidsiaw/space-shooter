#ifndef APP_H
#define APP_H

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_ttf.h>
#include "controller.h"

class App
{
    private:
        boost::shared_ptr<Controller> controller;
        bool running;
        SDL_Surface* surfDisplay;

    public:
        App(boost::shared_ptr<Controller> controller);
        int OnExecute();
        bool OnInit();
        void OnEvent(boost::shared_ptr<Scene> scene, SDL_Event* Event);
        void OnLoop(boost::shared_ptr<Scene> scene);
        void OnRender(boost::shared_ptr<Scene> scene);
        void OnCleanup();
};

#endif // APP_H
