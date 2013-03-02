#include "app.h"

void App::OnLoop(boost::shared_ptr<Scene> scene)
{
    scene->OnLoop();
}
