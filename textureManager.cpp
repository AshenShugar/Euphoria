#include "textureManager.hpp"
  /****************/
 /* Texture Item */
/****************/

TextureItem::TextureItem()
{
	Id = -1;
	filename = "";
	mTexture = NULL;
}

TextureItem::~TextureItem()
{
	if(mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
	}
}


  /*******************/
 /* Texture Manager */
/*******************/

TextureManager::TextureManager(void)
{
	iTextureCount = 0;
	textureList = NULL;
}

TextureManager::TextureManager(SDL_Renderer* pRenderer)
{
	iTextureCount = 0;
	mRenderer = pRenderer;
	textureList = NULL;
}

TextureManager::~TextureManager(void)
{
	FreeAllTexture();
	if(textureList != NULL)
	{
		delete[] textureList;
	}
}

void TextureManager::ModulateTextureColour(int Tid, myRGB colours)
{
	if (Tid >= iTextureCount || textureList == NULL)
		return;
	SDL_SetTextureColorMod( textureList[Tid].mTexture, colours.Red, colours.Green, colours.Blue );
}

// Reads in a file that has a list of files that hold textures and what id they should be given.
bool TextureManager::LoadTextureInfoFromFile(std::string filename)
{
	std::ifstream textureInfo( filename.c_str() );
	int iNumTextures, iLine;
	std::string textureFile;
	int textureId;
	bool bResult = true;

	if(textureInfo == NULL)
	{
		return false;
	}

	textureInfo >> iNumTextures;

	if (textureInfo.fail() )
	{
		bResult = false;
	}
	else
	{
		textureList = new TextureItem[iNumTextures];

		if(textureList == NULL)
		{
			bResult = false;
		}
		else
		{
			for(iLine = 0; iLine < iNumTextures; iLine++)
			{
				textureInfo >> textureId;
				if( textureInfo.fail() )
				{
					bResult = false;
					break;
				}
				textureInfo >> textureFile;
				if( textureInfo.fail() )
				{
					bResult = false;
					break;
				}
				textureList[iLine].Id = textureId;
				textureList[iLine].filename = textureFile;
			}
		}
	}
	if(bResult == true)
		iTextureCount = iNumTextures;
	textureInfo.close();
	return bResult;
}

bool TextureManager::LoadTextureFromFile(int id)
{
	//Get rid of preexisting texture
	if(textureList[id].mTexture != NULL)
		SDL_DestroyTexture(textureList[id].mTexture);

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( textureList[id].filename.c_str() );
	if( loadedSurface == NULL )
	{
		fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n", textureList[id].filename.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0x00 ) );

		//Create texture from surface pixels		newTexture = SDL_CreateTextureFromSurface( mRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", textureList[id].filename.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			textureList[id].mWidth = loadedSurface->w;
			textureList[id].mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	textureList[id].mTexture = newTexture;

	return textureList[id].mTexture != NULL;
}


void TextureManager::FreeAllTexture(void)
{
	for(int i = 0; i < iTextureCount; i++)
	{
		if(textureList[i].mTexture != NULL)
		{
			SDL_DestroyTexture(textureList[i].mTexture);
			textureList[i].mTexture = NULL;
		}
	}

}

bool TextureManager::RenderTextureToViewport(int iTextureID, SDL_Rect viewport)
{
	return RenderTextureToViewport(iTextureID,viewport,NULL,NULL);
}

bool TextureManager::RenderTextureToViewport(int iTextureID, SDL_Rect viewport, SDL_Rect* dest)
{
	return RenderTextureToViewport(iTextureID,viewport,dest,NULL);
}

bool TextureManager::RenderTextureToViewport(int iTextureID, SDL_Rect viewport, SDL_Rect* dest, SDL_Rect* source)
{
	if (textureList[iTextureID].mTexture == NULL)
	{
		if(!LoadTextureFromFile( iTextureID ))
		{
			fprintf(stderr,"Load Texture From File Fail\n");
			return false;
		}
	}
	SDL_RenderSetViewport( mRenderer, &viewport );
	SDL_RenderCopy(mRenderer, textureList[iTextureID].mTexture,source,dest);

	return true;
}