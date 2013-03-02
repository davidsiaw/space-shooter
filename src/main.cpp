#include <stdio.h>
#include <stdlib.h>
#include <sdl.h>
#include "app.h"

#include "directoryresources.h"
#include "params.h"

int screenWidth = 800;
int screenHeight = 600;
ResourceContainer* resources = NULL;

int main(int argc, char* argv[])
{
	resources = new DirectoryResources("D:/gcc/kubus/res");

    boost::shared_ptr<Controller> c(new Controller(argc, argv));
    App theApp(c);

    return theApp.OnExecute();
}
