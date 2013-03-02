#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "scene.h"
#include "params.h"

#include "gamescene.h"

class Controller
{
	boost::shared_ptr<Scene> scene;

public:
    Controller(int , char**)
    :
	scene(new GameScene())
    {
    }

	void Initialize() 
	{
		scene->OnInit();
	}

    boost::shared_ptr<Scene> GetCurrentScene()
    {
		return scene;
    }
};

#endif // CONTROLLER_H

