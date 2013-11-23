#include "sprite.hpp"

sprites::sprites()
{
	mSources = NULL;
	mSourceCount = 0;
	mTID = -1;
}

sprites::sprites(std::string filename)
{
	mSources = NULL;
	mSourceCount = 0;
	mTID = -1;

	LoadFromFile(filename);
}

sprites::~sprites()
{
	mSources = NULL;
	mSourceCount = 0;
	mTID = -1;
}

void sprites::LoadFromFile(std::string filename)
{
	std::ifstream objectInfo( filename.c_str() );
	int iNumSources, iLine;

	bool bResult = true;

	if(objectInfo == NULL)
	{
		return;
	}

	objectInfo >> iNumSources;

	if (!objectInfo.fail() )
	{
		mSourceCount = iNumSources;
		mSources = new SDL_Rect[mSourceCount];

		if(mSources != NULL )
		{
			objectInfo >> mTID;
			if(objectInfo.fail())
			{
				bResult = false;
			}
			else
			{
				for(iLine = 0; iLine < mSourceCount; iLine++)
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
			}
		}
	}
	if(bResult == false)
	{
		if(mSources != NULL)
		{
			delete[] mSources;
			mSources = NULL;
		}
		mTID = -1;
		mSourceCount = 0;
	}
	objectInfo.close();

}

SDL_Rect* sprites::getSource(int sourceID)
{
	if(sourceID >= mSourceCount || mSources == NULL)
		return NULL;

	return &mSources[sourceID];
}

int sprites::getSourceCount(void)
{
	return mSourceCount;
}

int sprites::getTextureID(void)
{
	return mTID;
}