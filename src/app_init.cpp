
#include "app.h"
#include "params.h"

#include "graphicfuncs.h"

void Init3D(int screenWidth, int screenHeight) {
	
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,2);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glClearColor(0, 0, 0, 0);
    glClearDepth(1.0f);

    glViewport(0, 0, screenWidth, screenHeight);


	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(60.0f,(GLfloat)screenWidth/(GLfloat)screenHeight,4.0f,10000.0f);
	
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_CULL_FACE);
	//glEnable(0x81AB);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations


	glEnable (GL_ALPHA_TEST); 
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool App::OnInit()
{
	if(TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
        return false;
	}
		
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }

    SDL_WM_SetCaption("Seikan Musou",NULL);

    if((surfDisplay = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL )) == NULL)
    {
        return false;
    }

    glewInit();

	Init3D(screenWidth, screenHeight);

    return true;
}
