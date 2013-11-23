#include "gameLocation.hpp"


gameLocation::gameLocation()
{
	mDestination = NULL;
	mSource = NULL;
	mTargetPane = NULL;
	mDefaultSource = 0;
}

gameLocation::gameLocation(std::string filename)
{
	mDestination = NULL;
	mSource = NULL;
	mDestinationSource = NULL;
	mTargetPane = NULL;
	mDefaultSource = 0;
	LoadFromFile(filename);
}

gameLocation::~gameLocation()
{
	if(mDestination != NULL)
		delete[] mDestination;

	if(mDestinationSource != NULL)
		delete[] mDestinationSource;
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

// Many destinations, many Source, set one destiation to one of the sources.
bool gameLocation::setValue(int destinationID, int sourceID)
{
	if(destinationID >= mMaxDestination || sourceID >= mSource->getSourceCount())	
		return false;
	else
		mDestinationSource[destinationID] = sourceID;

	return true;
}

bool gameLocation::setDefaultSource(int sourceID)
{
	if(sourceID >= mSource->getSourceCount() )
		return false;

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


void gameLocation::LoadFromFile(std::string filename)
{
	std::ifstream objectInfo( filename.c_str() );
	int iNumDestinations, iLine;

	bool bResult = true;

	if(objectInfo == NULL)
	{
		return;
	}

	objectInfo >> iNumDestinations;

	if (!objectInfo.fail() )
	{
		mMaxDestination = iNumDestinations;
		mDestination = new SDL_Rect[iNumDestinations];
		mDestinationSource = new int[iNumDestinations];

		if(mDestination != NULL && mDestinationSource != NULL)
		{
			objectInfo >> mBackgroundTextureID;
			if(objectInfo.fail())
			{
				bResult = false;
			}
			else
			{
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
	if(bResult == false)
	{
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

bool gameLocation::draw(TextureManager* lpTM)
{
	SDL_Rect destination, viewport;
	SDL_Rect* source;
	int iID;

	if(!good() )
		return false;

	viewport = *(getTargetPane())->getViewport();

	if(mBackgroundTextureID != -1)
		lpTM->RenderTextureToViewport( mBackgroundTextureID, viewport);
	
	for(iID = 0; iID < mMaxDestination; iID++)
	{
		if(mDestinationSource[iID] != -1)
		{
			source = getSource(mDestinationSource[iID]);
			destination = getDestination(iID);
			lpTM->RenderTextureToViewport( mSource->getTextureID(), viewport, &destination, source   );
		}
	}

	return true;
}
