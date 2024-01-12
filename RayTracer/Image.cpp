#include "Image.h"

Image::Image()
{
	m_xSize = 0;
	m_ySize = 0;
	m_pTexture = NULL;
}

Image::~Image()
{
	if (m_pTexture != NULL)
		SDL_DestroyTexture(m_pTexture);
}

void Image::Init(const int xSize, const int ySize, SDL_Renderer* pRenderer)
{
	//Resize Image Arrays
	m_rChanel.resize(xSize, std::vector<double>(ySize, 0.0));
	m_gChanel.resize(xSize, std::vector<double>(ySize, 0.0));
	m_bChanel.resize(xSize, std::vector<double>(ySize, 0.0));

	//Set Dimensions
	m_xSize = xSize;
	m_ySize = ySize;

	//Set Renderer
	m_pRenderer = pRenderer;

	//Initialize Texture
	InitTexture();
}

void Image::SetPixel(const int x, const int y, const double r, const double g, const double b)
{
	m_rChanel.at(x).at(y) = r;
	m_gChanel.at(x).at(y) = g;
	m_bChanel.at(x).at(y) = b;
}

void Image::Display()
{
	//Calculate Max Color Values
	ComputeMaxValues();

	//Allocate Pixel Buffer in Mem (using new keyword b/c SDL is in C so it needs to be compatable)
	Uint32* tempPixels = new Uint32[m_xSize * m_ySize];

	//Clear Pixeel Buffer
	memset(tempPixels, 0, m_xSize * m_ySize * sizeof(Uint32));


	for (int x = 0; x < m_xSize; x++)
	{
		for (int y = 0; y < m_ySize; y++)
		{
			//Convert 2D index to 1D index and set color data
			tempPixels[(y * m_xSize) + x] = ConvertColor(m_rChanel.at(x).at(y), m_gChanel.at(x).at(y), m_bChanel.at(x).at(y));
		}
	}

	//Update Texture With Pixel Buffer
	SDL_UpdateTexture(m_pTexture, NULL, tempPixels, m_xSize * sizeof(Uint32));

	delete[] tempPixels;

	//Copy Texture to Renderer
	SDL_Rect srcRect, bounds;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = m_xSize;
	srcRect.h = m_ySize;
	bounds = srcRect;
	SDL_RenderCopy(m_pRenderer, m_pTexture, &srcRect, &bounds);
}

int Image::GetXSize()
{
	return m_xSize;
}

int Image::GetYSize()
{
	return m_ySize;
}

void Image::SetWindowSize(int xSize, int ySize)
{
	//Resize Image Arrays
	m_rChanel.resize(xSize, std::vector<double>(ySize, 0.0));
	m_gChanel.resize(xSize, std::vector<double>(ySize, 0.0));
	m_bChanel.resize(xSize, std::vector<double>(ySize, 0.0));

	//Set Dimensions
	m_xSize = xSize;
	m_ySize = ySize;

	InitTexture();
}


Uint32 Image::ConvertColor(const double red, const double green, const double blue)
{
	// Convert the colours to unsigned integers.
	unsigned char r = static_cast<unsigned char>((red / m_overallMax) * 255.0);
	unsigned char g = static_cast<unsigned char>((green / m_overallMax) * 255.0);
	unsigned char b = static_cast<unsigned char>((blue / m_overallMax) * 255.0);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Uint32 pixelColor = (b << 24) + (g << 16) + (r << 8) + 255;
#else
	Uint32 pixelColor = (255 << 24) + (r << 16) + (g << 8) + b;
#endif

	return pixelColor;
}

void Image::InitTexture()
{
	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	bmask = 0x000000ff;
	gmask = 0x0000ff00;
	rmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	//Delete Previous Texture
	if (m_pTexture != NULL)
		SDL_DestroyTexture(m_pTexture);

	//Create New Texture to Store Image
	SDL_Surface* tempSurface = SDL_CreateRGBSurface(0, m_xSize, m_ySize, 32, rmask, gmask, bmask, amask);
	m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}

void Image::ComputeMaxValues()
{
	m_maxRed = 0.0;
	m_maxGreen = 0.0;
	m_maxBlue = 0.0;
	m_overallMax = 0.0;
	for (int x = 0; x < m_xSize; ++x)
	{
		for (int y = 0; y < m_ySize; ++y)
		{
			double redValue = m_rChanel.at(x).at(y);
			double greenValue = m_gChanel.at(x).at(y);
			double blueValue = m_bChanel.at(x).at(y);

			if (redValue > m_maxRed)
				m_maxRed = redValue;

			if (greenValue > m_maxGreen)
				m_maxGreen = greenValue;

			if (blueValue > m_maxBlue)
				m_maxBlue = blueValue;

			if (m_maxRed > m_overallMax)
				m_overallMax = m_maxRed;

			if (m_maxGreen > m_overallMax)
				m_overallMax = m_maxGreen;

			if (m_maxBlue > m_overallMax)
				m_overallMax = m_maxBlue;
		}
	}
}
