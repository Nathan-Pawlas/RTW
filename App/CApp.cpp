#include "CApp.h"

//Constructor
CAPP::CAPP()
{
	isRunning = true;
	pWindow = NULL;
	pRenderer = NULL;

	m_xSize = 1280;
	m_yZise = 720;
}

CAPP::CAPP(int xSize, int ySize)
{
	isRunning = true;
	pWindow = NULL;
	pRenderer = NULL;

	m_xSize = xSize;
	m_yZise = ySize;
}

//Initialize Window and Renderer
bool CAPP::OnInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	pWindow = SDL_CreateWindow("RayTracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_xSize, m_yZise, SDL_WINDOW_SHOWN);

	if (pWindow == NULL)
		return false;

	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);

	m_Image.Init(1280, 720, pRenderer);

	//Set Background Color
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
	SDL_RenderClear(pRenderer);

	m_renderer.Render(m_Image);

	m_Image.Display();

	SDL_RenderPresent(pRenderer);

	return true;

}

int CAPP::OnExecute()
{
	SDL_Event event;

	if (!OnInit())
		return -1;

	//Main Render Loop
	while (isRunning)
	{
		while (SDL_PollEvent(&event) != 0)
			OnEvent(&event);
		OnLoop();
		OnRender();
	}

	return 0;
}

void CAPP::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_QUIT)
		isRunning = false;
}

void CAPP::OnLoop()
{
}

void CAPP::OnRender()
{

}

//Deallocate Everything
void CAPP::OnExit()
{
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	pWindow = NULL;
	SDL_Quit();
}



