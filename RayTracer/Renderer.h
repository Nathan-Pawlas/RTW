#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>
#include <SDL.h>
#include "Image.h"



class Renderer
{
public:
	Renderer();

	bool Render(Image& outputImage);

private:

private:

};


#endif // !RENDERER_H