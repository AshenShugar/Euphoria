#include "gameObject.hpp"


gameObject::gameObject()
{
	mReady = false;
}

gameObject::gameObject(std::string filename)
{
	LoadObjectFromFile(filename);
}

gameObject::~gameObject()
{
	if(mDestination != NULL)
		delete[] mDestination;
}

bool gameObject::good(void)
{
	return mReady;
}

bool gameObject::setValue(int value)
{
	if(value > mMaxValue)	
		return false;
	else
		mValue = value;

	return true;
}

Pane* gameObject::getTargetPane(void)
{
	return mTargetPane;
}

bool gameObject::setTargetPane(Pane* target)
{
	if(target == NULL)
	{
		mTargetPane = NULL;
		mReady = false;
		return false;
	}
	else
		mTargetPane = target;

	return true;
}

int gameObject::getTID(void)
{
	return mTextureID;
}

SDL_Rect gameObject::getDestination(void)
{
	return mDestination[mValue];
}

SDL_Rect gameObject::getSource(void)
{
	return mSource;
}



void gameObject::LoadObjectFromFile(std::string filename)
{
	std::ifstream objectInfo( filename.c_str() );
	int iNumDestinations, iLine;
	std::string spriteInfo;
	int textureId;
	bool bResult = true;

	if(objectInfo == NULL)
	{
		mReady = false;
		return;
	}

	objectInfo >> iNumDestinations;

	if (objectInfo.fail() )
	{
		bResult = false;
	}
	else
	{
		mMaxValue = iNumDestinations;
		mDestination = new SDL_Rect[iNumDestinations];

		if(mDestination == NULL)
		{
			bResult = false;
		}
		else
		{
			objectInfo >> mBackgroundTextureID;
			if(objectInfo.fail())
			{
				bResult = false;
			}
			else
			{
				objectInfo >> mTextureID;
				if(objectInfo.fail())
				{
					bResult = false;
				}
				else
				{
					objectInfo >> mSource.x;
					if( objectInfo.fail() )
					{
						objectInfo.close();
						mReady = false;
						return;
					}
					objectInfo >> mSource.y;
					if( objectInfo.fail() )
					{
						objectInfo.close();
						mReady = false;
						return;
					}
					objectInfo >> mSource.w;
					if( objectInfo.fail() )
					{
						objectInfo.close();
						mReady = false;
						return;
					}
					objectInfo >> mSource.h;
					if( objectInfo.fail() )
					{
						objectInfo.close();
						mReady = false;
						return;
					}

					for(iLine = 0; iLine < iNumDestinations; iLine++)
					{

						objectInfo >> mDestination[iLine].x;
						if( objectInfo.fail() )
						{
							bResult = false;
							break;
						}
						objectInfo >> mDestination[iLine].y;
						if( objectInfo.fail() )
						{
							bResult = false;
							break;
						}
						objectInfo >> mDestination[iLine].w;
						if( objectInfo.fail() )
						{
							bResult = false;
							break;
						}
						objectInfo >> mDestination[iLine].h;
						if( objectInfo.fail() )
						{
							bResult = false;
							break;
						}
					}
				}
			}
		}
	}

	objectInfo.close();
	mReady = bResult;
}

bool gameObject::draw(TextureManager* lpTM)
{
	SDL_Rect destination,source, viewport;

	destination = getDestination();
	source = getSource();
	viewport = *(getTargetPane())->getViewport();

	if(mBackgroundTextureID != -1)
		lpTM->RenderTextureToViewport( mBackgroundTextureID, viewport);
	
	lpTM->RenderTextureToViewport( getTID(), viewport, &destination, &source   );
}
