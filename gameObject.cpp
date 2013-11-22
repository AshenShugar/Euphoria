#include "gameObject.hpp"

sprite::sprite()
{
	textureID = 0;
	mx = 0;
	my = 0;
	mw = 0;
	mh = 0;
}

sprite::sprite(int tID, int x,int y,int w,int h)
{
	textureID = tID;
	mx = x;
	my = y;
	mw = w;
	mh = h;
}

gameObject::gameObject()
{
	mloaded = false;
}

gameObject::gameObject(std::string filename)
{
	LoadObjectFromFile(filename);
}

gameObject::~gameObject()
{
	if(valueSprites != NULL)
		delete[] valueSprites;
}

bool gameObject::good(void)
{
	return mloaded;
}

bool gameObject::setValue(int value)
{
	if(value > mMaxValue)	
		return false;
	else
		mValue = value;
}

Pane* gameObject::getTargetPane(void)
{
	return targetPane;
}

bool gameObject::setTargetPane(Pane* target)
{
	if(target == NULL)
	{
		targetPane = NULL;
		mloaded = false;
	}
	else
		targetPane = target;

	return mloaded;
}

int gameObject::getSpriteTID(void)
{
	return valueSprites[mValue].textureID;
}

SDL_Rect gameObject::getSpriteLocation(void)
{
	SDL_Rect tmpR;

	tmpR.x = valueSprites[mValue].mx;
	tmpR.y = valueSprites[mValue].my;
	tmpR.w = valueSprites[mValue].mw;
	tmpR.h = valueSprites[mValue].mh;

	return tmpR;
}

SDL_Rect gameObject::getSpriteSource(void)
{
	SDL_Rect tmpR;
	tmpR.x = valueSprites[mValue].sx;
	tmpR.y = valueSprites[mValue].sy;
	tmpR.w = valueSprites[mValue].sw;
	tmpR.h = valueSprites[mValue].sh;

	return tmpR;
}



void gameObject::LoadObjectFromFile(std::string filename)
{
	std::ifstream objectInfo( filename.c_str() );
	int iNumSprites, iLine;
	std::string spriteInfo;
	int textureId;
	bool bResult = true;

	if(objectInfo == NULL)
	{
		mloaded = false;
		return;
	}

	objectInfo >> iNumSprites;

	if (objectInfo.fail() )
	{
		bResult = false;
	}
	else
	{
		mMaxValue = iNumSprites;
		valueSprites = new sprite[iNumSprites];

		if(valueSprites == NULL)
		{
			bResult = false;
		}
		else
		{
			objectInfo >> backgroundTextureID;
			if(objectInfo.fail())
			{
				bResult = false;
			}
			else
			{
				for(iLine = 0; iLine < iNumSprites; iLine++)
				{
					objectInfo >> valueSprites[iLine].textureID;
					if( objectInfo.fail() )
					{
						bResult = false;
						break;
					}

					objectInfo >> valueSprites[iLine].mx;
					if( objectInfo.fail() )
					{
						bResult = false;
						break;
					}
					objectInfo >> valueSprites[iLine].my;
					if( objectInfo.fail() )
					{
						bResult = false;
						break;
					}
					objectInfo >> valueSprites[iLine].mw;
					if( objectInfo.fail() )
					{
						bResult = false;
						break;
					}
					objectInfo >> valueSprites[iLine].mh;
					if( objectInfo.fail() )
					{
						bResult = false;
						break;
					}

					objectInfo >> valueSprites[iLine].sx;
					if( objectInfo.fail() )
					{
						bResult = false;
						break;
					}
					objectInfo >> valueSprites[iLine].sy;
					if( objectInfo.fail() )
					{
						bResult = false;
						break;
					}
					objectInfo >> valueSprites[iLine].sw;
					if( objectInfo.fail() )
					{
						bResult = false;
						break;
					}
					objectInfo >> valueSprites[iLine].sh;
					if( objectInfo.fail() )
					{
						bResult = false;
						break;
					}


				}
			}
		}
	}

	objectInfo.close();
	mloaded = bResult;
}

bool gameObject::draw(TextureManager* lpTM)
{
	SDL_Rect destination,source;

	destination = getSpriteLocation();
	source = getSpriteSource();
	lpTM->RenderTextureToViewport( getSpriteTID(), *getTargetPane()->getViewport(), &destination, &source   );
}
