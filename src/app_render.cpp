#include "app.h"

void App::OnRender(boost::shared_ptr<Scene> scene)
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    scene->OnRender();
    SDL_GL_SwapBuffers();
}
