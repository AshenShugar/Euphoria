#include "mto1gLocation.hpp"

void Mto1gLocation::calculateDestinations(void)
{
		// Need to update the list of destination rects.
		if (mNumDestinations <= mMaxIndex)
		{
			int iNewRects,ix,iy, iIndex, xsize,ysize;
			float ratio = (float)mDestination[0].w / (float)mDestination[0].h;

			ix = 1;
			iy = 1;
			while( ix * iy <= mMaxIndex)
			{
				if((float)iy * ratio < (float)ix)
					iy++;
				else
					ix++;
			}
			iNewRects = ix * iy;
			delete[] mTmpDestinations;
			mTmpDestinations = new SDL_Rect[iNewRects];
			mNumDestinations = iNewRects;
			if((float)mDestination[0].w / (float)ix < (float)mDestination[0].h / (float)iy )
			{
				xsize = (mDestination[0].w / ix) - 1;
				ysize = xsize;
			}
			else
			{
				ysize = (mDestination[0].h / iy) - 1;
				xsize = ysize;
			}

			iIndex = 0;
			for(int i = 0; i< ix; i++)
			{
				for(int j = 0; j < iy; j++)
				{
					mTmpDestinations[iIndex].x = mDestination[0].x + (i * (xsize + 1));
					mTmpDestinations[iIndex].y = mDestination[0].y + (j * (ysize + 1));
					mTmpDestinations[iIndex].w = xsize;
					mTmpDestinations[iIndex].h = ysize;
					iIndex++;
				}
			}
		}
}

Mto1gLocation::Mto1gLocation()
{
	mNextIndex = 0;
	mMaxIndex = -1;
	mNumDestinations = 0;
	mTmpDestinations = NULL;
	mDestination = NULL;
	mSource = NULL;
	mTargetPane = NULL;
	mDefaultSource = 0;
}

Mto1gLocation::Mto1gLocation(std::string filename)
{
	mDestination = NULL;
	mSource = NULL;
	mTargetPane = NULL;
	mDefaultSource = 0;
	mNumDestinations = 0;
	mTmpDestinations = NULL;
	LoadFromFile(filename);
	mNextIndex = mMaxDestination;
	mMaxIndex = mMaxDestination - 1;	
	calculateDestinations();	
}

Mto1gLocation::~Mto1gLocation()
{
	if(mTmpDestinations != NULL)
		delete[] mTmpDestinations;

	if(mDestination != NULL)
		delete[] mDestination;

	if(mDestinationSource != NULL)
		delete[] mDestinationSource;
}
bool Mto1gLocation::draw(TextureManager* lpTM)
{

	SDL_Rect destination, viewport;
	SDL_Rect* source;
	int i,j;

	if(!good() )
		return false;

	viewport = *(getTargetPane())->getViewport();

	if(mBackgroundTextureID != -1)
		lpTM->RenderTextureToViewport( mBackgroundTextureID, viewport);
	j = 0;
	for(i=0; i <= mMaxIndex; i++)
	{
		if(mDestinationSource[i] != -1)
		{
			if(mSource-> getSourceCount() <= mDestinationSource[i])
			{
				fprintf(stderr,"Something dodgy - mDestination[%d] = %d\n", i, mDestinationSource[i]);
			}
			source = getSource(mDestinationSource[i]);
			destination = mTmpDestinations[j];

			lpTM->RenderTextureToViewport( mSource->getTextureID(), viewport, &destination, source   );
			j++;
		}

	}


	return true;

}

bool Mto1gLocation::setValue(int d,int s)
{
	// do nothing for now.
}

bool Mto1gLocation::addValue(int sourceID)
{
	if(mNextIndex > mMaxIndex)
	{
		// create larger array and copy the values into this new one
		int* pNewArray = new int[mNextIndex+1];
		for(int i = 0; i <= mMaxIndex; i++)
		{
			pNewArray[i] = mDestinationSource[i];
		}
		if(mDestinationSource != NULL)
			delete[] mDestinationSource;
		mDestinationSource = pNewArray;
		mMaxIndex = mNextIndex;
		calculateDestinations();
	}
	mDestinationSource[mNextIndex] = sourceID;

	mNextIndex = mMaxIndex + 1;
	for(int i = 0; i <= mMaxIndex; i++)
	{
		if(mDestinationSource[i] == -1)
		{
			mNextIndex = i;
			break;
		}
	}

	return true;
}

bool Mto1gLocation::removeValue(int sourceID)
{
	for(int i = 0; i <= mMaxIndex; i++)
	{
		if(mDestinationSource[i] == sourceID)
		{
			mDestinationSource[i] = -1;
			mNextIndex = i;
			break;
		}
	}
}