#ifndef DIRECTORYRESOURCES_H
#define DIRECTORYRESOURCES_H

#include "resourcecontainer.h"
#include <map>

class DirectoryResources : public ResourceContainer
{
	typedef std::map<std::string, SDL_Surface*> SurfaceCache;

	std::string path;
	SurfaceCache images;

public:
	DirectoryResources(std::string path);
	~DirectoryResources();
    virtual SDL_Surface* GetImage(std::string filename);
	virtual std::string GetTextfile(std::string filename);
};


#endif // DIRECTORYRESOURCES_H
