#include "gameObjectB.hpp"

gameObjectB::gameObjectB()
{
	mReady = false;
	mSources = NULL;
	mDestination = NULL;
	mDestinationValues = NULL;
}

gameObjectB::gameObjectB(std::string filename)
{
	mReady = false;
	mSources = NULL;
	mDestination = NULL;
	mDestinationValues = NULL;

	LoadObjectFromFile(filename);
}

gameObjectB::~gameObjectB()
{
	if(mSources != NULL)
	{
		delete[] mSources;
	}

	if(mDestination != NULL)
	{
		//delete[] mDestination;
	}

	if(mDestinationValues != NULL)
	{
		delete[] mDestinationValues;
	}
}

void gameObjectB::LoadObjectFromFile(std::string filename)
{
	std::ifstream objectInfo( filename.c_str() );
	int iNumDestinations, iLine, iNumSources;
	std::string spriteInfo;
	int textureId;
	bool bResult = true;

	if(objectInfo == NULL)
	{
		mReady = false;
		return;
	}

	objectInfo >> iNumSources;

	if (objectInfo.fail() )
	{
		mReady = false;
		return;
	}
	
	objectInfo >> iNumDestinations;
	if (objectInfo.fail() )
	{
		mReady = false;
		return;
	}


	mMaxValue = iNumSources;
	mMaxDestination = iNumDestinations;

	mDestination = new SDL_Rect[iNumDestinations];
	mSources = new SDL_Rect[iNumSources];
	mDestinationValues = new int[iNumDestinations];

	if(mDestination == NULL || mSources == NULL || mDestinationValues == NULL)
	{
		fprintf(stderr, "Error allocating memory for gameObjectB(%s)\n",filename.c_str() );
		mReady = false;
		return;
	}

	objectInfo >> mBackgroundTextureID;
	if(objectInfo.fail())
	{
		mReady = false;
		return;
	}

	objectInfo >> mTextureID;
	if(objectInfo.fail())
	{
		mReady = false;
		return;
	}

	for(iLine = 0; iLine < iNumSources; iLine++)
	{
		objectInfo >> mSources[iLine].x;
		if( objectInfo.fail() )
		{
			bResult = false;
			break;
		}
		objectInfo >> mSources[iLine].y;
		if( objectInfo.fail() )
		{
			bResult = false;
			break;
		}
		objectInfo >> mSources[iLine].w;
		if( objectInfo.fail() )
		{
			bResult = false;
			break;
		}
		objectInfo >> mSources[iLine].h;
		if( objectInfo.fail() )
		{
			bResult = false;
			break;
		}
	}
	if(bResult == false)
	{
		objectInfo.close();
		mReady = false;
		return;
	}
	
	for(iLine = 0; iLine < iNumDestinations; iLine++)
	{
		mDestinationValues[iLine] = -1;	// initialise array to be "empty"

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


	objectInfo.close();
	mReady = true;
}

bool gameObjectB::setValue(int location, int value)
{
	if(!good() )
		return false;

	if(value >= mMaxValue)
		return false;

	if(location >= mMaxDestination)
		return false;
	
	if(mDestinationValues == NULL)
		return false;
		

	mDestinationValues[location] = value;

	return true;
}


bool gameObjectB::draw(TextureManager* lpTM)
{
	SDL_Rect destination,source, viewport;
	int i;

	if(mBackgroundTextureID != -1)
		lpTM->RenderTextureToViewport( mBackgroundTextureID, viewport);
		
	for(i = 0; i < mMaxDestination; i++)
	{
		if(mDestinationValues[i] != -1)
		{
			destination = mDestination[i];
			source = mSources[mDestinationValues[i]];
			viewport = *(getTargetPane())->getViewport();
	
			lpTM->RenderTextureToViewport( getTID(), viewport, &destination, &source   );
		}
	}
}

