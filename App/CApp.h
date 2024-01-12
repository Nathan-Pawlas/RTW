#ifndef CAPP_H
#define CAPP_H

#include <SDL.h>
#include "../RayTracer/Image.h"
#include "../RayTracer/Renderer.h"

class CAPP
{
public:
	CAPP();
	CAPP(int xSize, int ySize);

	bool OnInit();
	int OnExecute();
	void OnEvent(SDL_Event* event);
	void OnLoop();
	void OnRender();
	void OnExit();

private:
	Image m_Image;
	Renderer m_renderer;

	bool isRunning;
	SDL_Window* pWindow;
	SDL_Renderer* pRenderer;

	int m_xSize;
	int m_yZise;
};
#endif // !CAPP_H