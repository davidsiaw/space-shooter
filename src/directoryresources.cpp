#include "directoryresources.h"
#include "SDL_image.h"


DirectoryResources::DirectoryResources(std::string path)
{
	this->path = path;
}

DirectoryResources::~DirectoryResources()
{
}

SDL_Surface* DirectoryResources::GetImage(std::string filename)
{
	SurfaceCache::iterator elem = images.find(filename);
	if (elem == images.end())
	{
		images[filename] = IMG_Load((path + filename).c_str());
	}
	return images[filename];
}

std::string DirectoryResources::GetTextfile(std::string filename)
{
	FILE* fp = fopen((path + filename).c_str(), "rb");
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* text = new char[size + 1];
	fread(text, 1, size, fp);
	text[size] = 0;
	std::string res = text;
	delete[] text;
	fclose(fp);
	return res;
}

