#include "gameLocation.hpp"


gameLocation::gameLocation()
{
	mDestination = NULL;
	mSource = NULL;
	mTargetPane = NULL;
	mDefaultSource = 0;
	mColourMod = NULL;
}

gameLocation::gameLocation(std::string filename)
{
	mDestination = NULL;
	mSource = NULL;
	mDestinationSource = NULL;
	mTargetPane = NULL;
	mDefaultSource = 0;
	mColourMod = NULL;
	LoadFromFile(filename);

	if(mColourMod != NULL)
	{
		for(int i = 0; i < mMaxDestination; i++)
		{
			mColourMod[i].Red = 255;
			mColourMod[i].Green = 255;
			mColourMod[i].Blue = 255;
			mColourMod[i].modify = false;
		}
	}
}

gameLocation::~gameLocation()
{
	if(mDestination != NULL)
		delete[] mDestination;

	if(mDestinationSource != NULL)
		delete[] mDestinationSource;

	if(mColourMod != NULL)
		delete[] mColourMod;
}

bool gameLocation::good(void)
{
	if(mDestination == NULL)
		return false;

	if(mSource == NULL)
		return false;

	if(mTargetPane == NULL)
		return false;

	return true;
}

LOCATION_TYPE gameLocation::getType(void)
{
	return mType;
}

bool gameLocation::isClickable(void)
{
	return mClickable;
}

void gameLocation::setClickable(bool isClickable)
{
	mClickable = isClickable;
}


// Many destinations, many Source, set one destiation to one of the sources.
bool gameLocation::setValue(int destinationID, int sourceID)
{
	if(destinationID >= mMaxDestination || sourceID >= mSource->getSourceCount())	
		return false;
	else if(mDestinationSource == NULL)
	{
		fprintf(stderr, "mDestinationSource was NULL for game location '%s': (setValue(%d,%d);)",mFilename.c_str(), destinationID, sourceID);
		return false;
	}
	else
		mDestinationSource[destinationID] = sourceID;

	return true;
}

bool gameLocation::setDefaultSource(int sourceID)
{
	// allow it to be set even if the sources haven't been set.  Just double check it when they are set.
	if(mSource != NULL)
	{
		if(sourceID >= mSource->getSourceCount() )
			return false;
	}
	mDefaultSource = sourceID;
	return true;
}

// Many destinations, One Source.
bool gameLocation::setValue(int destinationID)
{
	if(destinationID >= mMaxDestination)
		return false;

	for( int i = 0; i < mMaxDestination; i++)
	{
		mDestinationSource[i] = -1;
	}
	mDestinationSource[destinationID] = mDefaultSource;

	return true;
}

// one destination, many source
bool gameLocation::setValueS(int sourceID)
{
	if(sourceID >= mSource->getSourceCount())
		return false;

	mDestinationSource[0] = sourceID;

	return true;
}

void gameLocation::setSourceArray(sprites* source)
{
	mSource = source;
	if (source != NULL)
	{
		if(mSource->getSourceCount() <= mDefaultSource)
		{
			mDefaultSource = -1;
		}
	}
}

Pane* gameLocation::getTargetPane(void)
{
	return mTargetPane;
}

void gameLocation::setTargetPane(Pane* target)
{
	if(target == NULL)
		mTargetPane = NULL;
	else
		mTargetPane = target;

}


SDL_Rect gameLocation::getDestination(int destinationID)
{
	return mDestination[destinationID];
}

SDL_Rect* gameLocation::getSource(int sourceID)
{
	return mSource->getSource(sourceID);
}

int gameLocation::isTargeted(int x, int y)
{
	int i;
	int iDestinationID = -1;

	if(mDestination == NULL)
	{
		fprintf(stderr,"game location (%s) not loaded correctly! ", mFilename.c_str());
		iDestinationID = -2;
	}
	else
	{
		SDL_Rect* tmp = (getTargetPane())->getViewport();
		for( i = 0; i < mMaxDestination; i++)
		{
			if(x < mDestination[i].x + tmp->x)
				continue;
			if(x > (mDestination[i].x + tmp->x + mDestination[i].w) )
				continue;
			if(y < mDestination[i].y + tmp->y)
				continue;
			if(y > (mDestination[i].y + tmp->y + mDestination[i].h) )
				continue;	

			iDestinationID = i;
			break;
		}
	}
	return iDestinationID;
}

void gameLocation::LoadFromFile(std::string filename)
{
	std::ifstream objectInfo( filename.c_str() );
	int iNumDestinations, iLine;

	bool bResult = true;
	mFilename = filename;

	if(objectInfo == NULL)
	{
		return;
	}

	objectInfo >> iLine;
	
	if (!objectInfo.fail() )
	{
		mType = (LOCATION_TYPE)iLine;
		objectInfo >> iNumDestinations;

		if (!objectInfo.fail() )
		{
			mMaxDestination = iNumDestinations;
			mDestination = new SDL_Rect[iNumDestinations];
			mDestinationSource = new int[iNumDestinations];
			mColourMod = new myRGB[iNumDestinations];
	
			if(mDestination != NULL && mDestinationSource != NULL && mColourMod != NULL)
			{
				objectInfo >> mBackgroundTextureID;
				if(objectInfo.fail())
				{
					bResult = false;
				}
				else
				{
					if(mBackgroundTextureID != -1)
					{
						objectInfo >> mBackgroundDestination.x;
						if(objectInfo.fail())
						{
							bResult = false;
						}
						objectInfo >> mBackgroundDestination.y;
						if(objectInfo.fail())
						{
							bResult = false;
						}
						objectInfo >> mBackgroundDestination.w;
						if(objectInfo.fail())
						{
							bResult = false;
						}
						objectInfo >> mBackgroundDestination.h;
						if(objectInfo.fail())
						{
							bResult = false;
						}
					}
					for(iLine = 0; iLine < iNumDestinations; iLine++)
					{
						mDestinationSource[iLine] = -1;	// initialise to "empty"
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
	else
	{
		bResult = false;
	}
	if(bResult == false)
	{
		fprintf(stderr,"Error in LoadFromFile(\"%s\")",filename.c_str());
		if(mDestination != NULL)
		{
			delete[] mDestination;
			mDestination = NULL;
		}
		if(mDestinationSource != NULL)
		{
			delete[] mDestinationSource;
			mDestinationSource = NULL;
		}
	}

	objectInfo.close();
}

void gameLocation::setColourModulation(int destinationID, int R,int G,int B)
{
	if( mColourMod == NULL)
	{
		mColourMod = new myRGB[mMaxDestination];
		if(mColourMod == NULL)
		{
			fprintf(stderr, "Error allocation memory for colour modulation array\n");
			return;
		}
		for(int i = 0; i < mMaxDestination; i++)
		{
			mColourMod[i].Red = 255;
			mColourMod[i].Green = 255;
			mColourMod[i].Blue = 255;
			mColourMod[i].modify = false;
		}
	}
	mColourMod[destinationID].Red = R;
	mColourMod[destinationID].Green = G;
	mColourMod[destinationID].Blue = B;

	if(R == 255 && G == 255 && B == 255)
		mColourMod[destinationID].modify = false;
	else
		mColourMod[destinationID].modify = true;

}

bool gameLocation::draw(TextureManager* lpTM)
{
	SDL_Rect destination, viewport;
	SDL_Rect* source;
	int iID;
	bool resetColourMod = false;

	if(!good() )
		return false;

	viewport = *(getTargetPane())->getViewport();

	if(mBackgroundTextureID != -1)
	{
		lpTM->RenderTextureToViewport( mBackgroundTextureID, viewport, &mBackgroundDestination, NULL);
	}
	
	for(iID = 0; iID < mMaxDestination; iID++)
	{
		if(mDestinationSource[iID] != -1)
		{
			source = getSource(mDestinationSource[iID]);
			destination = getDestination(iID);

			if(mColourMod[iID].modify)
			{
				lpTM->ModulateTextureColour(mSource->getTextureID(), mColourMod[iID]);
				resetColourMod = true;
			}
			else if (resetColourMod == true)
			{
				lpTM->ModulateTextureColour(mSource->getTextureID(), mColourMod[iID]);
				resetColourMod = false;
			}
			else
				resetColourMod = false;

			lpTM->RenderTextureToViewport( mSource->getTextureID(), viewport, &destination, source   );
		}
	}

	return true;
}
