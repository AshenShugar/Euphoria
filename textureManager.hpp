#include "myEnum.hpp"
#include <SDL.H>
#include <SDL_image.h>
#include <string>
#include <fstream>


class TextureItem
{
	public:
		TextureItem();
		~TextureItem();
		SDL_Texture* mTexture;
		int Id,mWidth,mHeight;
		std::string filename;
};

class TextureManager
{
	public:
		TextureManager(void);
		TextureManager(SDL_Renderer*);
		~TextureManager(void);
		bool LoadTextureInfoFromFile(std::string filename);
		void FreeTexture(int id);
		void FreeAllTexture(void);
		bool RenderTextureToViewport(int id, SDL_Rect viewport); 
		bool RenderTextureToViewport(int id, SDL_Rect viewport, SDL_Rect* dest); 

		bool RenderTextureToViewport(int id, SDL_Rect viewport, SDL_Rect* dest, SDL_Rect* source); 
		void ModulateTextureColour(int id, myRGB colours);
		SDL_Renderer *mRenderer;
	private:
		bool LoadTextureFromFile(int id);
		TextureItem *textureList;
		int iTextureCount;
};
