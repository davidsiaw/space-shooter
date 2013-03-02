#ifndef SCENE_H
#define SCENE_H

#include <sdl.h>
#include "definitions.h"

INTERFACE Scene
{
public:
        Scene() { }
        virtual ~Scene() { }
		virtual void OnInit() = 0;
        virtual void OnRender() = 0;
        virtual void OnEvent(SDL_Event* Event) = 0;
        virtual void OnLoop() = 0;
		virtual bool IsComplete() = 0;
};

#endif // SCENE_H
