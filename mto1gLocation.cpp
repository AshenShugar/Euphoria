#include "mto1gLocation.hpp"

void Mto1gLocation::calculateDestinations(void)
{
		// Need to update the list of destination rects.
		if (mNumDestinations <= mMaxIndex)
		{
			int iNewRects,ix,iy, iIndex, xsize,ysize;
			// work out how wide the location is compared to how high
			float ratio = (float)mDestination[0].w / (float)mDestination[0].h;

			ix = 1;
			iy = 1;
			// while there aren't enough locations to contain everything...
			while( ix * iy <= mMaxIndex)
			{
				// check the ratio of width to height and see whether we should add more rows or more columns to try and fit everything in,
				if((float)iy * ratio < (float)ix)
					iy++;
				else
					ix++;
				// and keep adjusting either the width or height by 1 until there are enough sub-locations to fit in everything
			}
			// work out how many sub-locations we have room for
			iNewRects = ix * iy;
			// can just wipe out all the old sub-locations, as they're all different now.
			if(mTmpDestinations != NULL)
				delete[] mTmpDestinations;
			// allocate memory for the co-ordinates of all the new sub-locations
			mTmpDestinations = new SDL_Rect[iNewRects];
			// keep track of how many sub-locations we've got available now.
			mNumDestinations = iNewRects;
			// The sub-locations are only allowed to be squares at the moment, so see which way fits them in better,
			// horizontally or vertically.
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
			// now go through all the sub-locations and assign their co-ordinates
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
	int i;

	if(!good() )
		return false;

	viewport = *(getTargetPane())->getViewport();

	if(mBackgroundTextureID != -1)
		lpTM->RenderTextureToViewport( mBackgroundTextureID, viewport);

	for(i=0; i <= mMaxIndex; i++)
	{
		if(mDestinationSource[i] != -1)
		{
			if(mSource-> getSourceCount() <= mDestinationSource[i])
			{
				fprintf(stderr,"Something dodgy - mDestination[%d] = %d\n", i, mDestinationSource[i]);
			}
			source = getSource(mDestinationSource[i]);
			destination = mTmpDestinations[i];

			lpTM->RenderTextureToViewport( mSource->getTextureID(), viewport, &destination, source   );
		}

	}


	return true;

}

bool Mto1gLocation::setValue(int d,int s)
{
	// wipe out everything and start again with just this one item to display in the location
	// Currently not using the passed d interger since we're assuming there's only one location that we need to calculate sub-locations for.
	// probably wouldn't be that hard to add that functionality, 
	// though it would mean having an array keyed by this d value to an array of rects for the tmpDestinations,
	// and we don't have any current plans to need this functionality.
	if(mDestinationSource != NULL)
		delete[] mDestinationSource;

	mNextIndex = 0;
	mDestinationSource = new int[1];
	mDestinationSource[mNextIndex++] = s;	// set the item to be displayed and increment mNextIndex since it has to be after the only item in the array.
	mMaxIndex = 0;		// highest valid index into the mDestinationSource array.
	mNumDestinations = 0;	// Set this to 0 to to indicate that we can't be sure we have any valid sub-locations. 
				// This is to make sure CalculateDestinations does it's thing and doesn't think we've already got enough sub-locations defined.
	calculateDestinations();

	return true;
}

bool Mto1gLocation::clearValues(void)
{
	if(mDestinationSource != NULL)
		delete[] mDestinationSource;
	mNextIndex = 0;
	mDestinationSource = NULL;
	mMaxIndex = -1;
	mNumDestinations = 0;

	calculateDestinations();
	return true;
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

	// default to the next index being at the end of the list
	mNextIndex = mMaxIndex + 1;
	// but then loop through to see if there's a spot that's had a value remove, and make that the next spot to add a value.
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

// For cosmetic purposes, after removing a value, could do a recalculation to see if can get by with fewer sub-locations,
// which would allow the existing items to be drawn larger.  Would need to go through gDestinationSource and move all
// the -1 values to the end of the array so the draw function doesn't try to draw them in sub-locations that don't exist.

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

// returns the source ID of the item clicked on.
// takes the screen co-ordinates clicked on.
int Mto1gLocation::SourceClickedOn(int xScreen, int yScreen)
{
	int sourceID = -1;

	SDL_Rect* tmp = (getTargetPane())->getViewport();

	for( int i = 0; i <= mMaxIndex; i++)
	{
		if(xScreen < mTmpDestinations[i].x + tmp->x)
			continue;
		if(xScreen > (mTmpDestinations[i].x + tmp->x + mTmpDestinations[i].w) )
			continue;
		if(yScreen < mTmpDestinations[i].y + tmp->y)
			continue;
		if(yScreen > (mTmpDestinations[i].y + tmp->y + mTmpDestinations[i].h) )
			continue;	

		sourceID = mDestinationSource[i];
		break;
	}
	return sourceID;

}
