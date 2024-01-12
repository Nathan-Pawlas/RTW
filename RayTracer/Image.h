#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <SDL.h>

class Image
{
public:
	Image();
	~Image();

	//Initizalize Image
	void Init(const int xSize, const int ySize, SDL_Renderer* pRenderer);
	//Set Pixel Color
	void SetPixel(const int x, const int y, const double r, const double g, const double b);
	//Display Image
	void Display();

	//Getters
	int GetXSize();
	int GetYSize();

	//Setters
	void SetWindowSize(int xSize, int ySize);

private:
	//Double -> Uint32 for SDL (doubles used for lighting purposes)
	Uint32 ConvertColor(const double red, const double green, const double blue);
	void InitTexture();
	void ComputeMaxValues();

private:
	//2D Arrays for Image Data
	std::vector<std::vector<double>> m_rChanel;
	std::vector<std::vector<double>> m_gChanel;
	std::vector<std::vector<double>> m_bChanel;

	//Image Size
	int m_xSize, m_ySize;

	double m_maxRed, m_maxGreen, m_maxBlue, m_overallMax;

	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture;
};

#endif // !IMAGE_H



