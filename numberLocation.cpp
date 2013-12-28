#include "numberLocation.hpp"

numberLocation::numberLocation()
{
	mDestination = NULL;
	mSource = NULL;
	mTargetPane = NULL;
	mDefaultSource = 0;
	digitDestinations = NULL;
	numDigits = NULL;
	digitSource = NULL;
	mJustification = NULL;
}

numberLocation::numberLocation(std::string filename)
{
	mDestination = NULL;
	mSource = NULL;
	mDestinationSource = NULL;
	mTargetPane = NULL;
	mDefaultSource = 0;
	digitDestinations = NULL;
	numDigits = NULL;
	digitSource = NULL;
	mJustification = NULL;
	LoadFromFile(filename);
	mJustification = new TEXT_JUSTIFICATION[mMaxDestination];
}

numberLocation::~numberLocation()
{
	if(mDestination != NULL)
		delete[] mDestination;

	if(mDestinationSource != NULL)
		delete[] mDestinationSource;

	if(digitDestinations != NULL)
	{
		for(int i = 0; i < mMaxDestination; i++)
		{
			if (digitDestinations[i] != NULL)
				delete[] digitDestinations[i];
		}
		delete[] digitDestinations;
	}

	if(digitSource != NULL)
	{
		for (int i = 0; i < mMaxDestination; i++)
		{
			if (digitSource[i] != NULL)
			 delete[] digitSource[i];
		}
		delete[] digitSource;
	}

	if(numDigits != NULL)
		delete[] numDigits;

	if(mJustification != NULL)
		delete[] mJustification;

}

void numberLocation::setJustification( TEXT_JUSTIFICATION alignment)
{
	if(mJustification == NULL)
		mJustification = new TEXT_JUSTIFICATION[mMaxDestination];

	if(mJustification != NULL)
		mJustification[0] = alignment;
}

void numberLocation::setJustification(int locationID, TEXT_JUSTIFICATION alignment)
{
	if(mJustification == NULL)
		mJustification = new TEXT_JUSTIFICATION[mMaxDestination];

	if(mJustification != NULL && locationID < mMaxDestination)
		mJustification[locationID] = alignment;
}

bool numberLocation::draw(TextureManager* lpTM)
{
	SDL_Rect destination;
	SDL_Rect viewport;
	SDL_Rect* source;
	int iID;
	int numberToDisplay;

	if(!good() )
		return false;

	if(numDigits == NULL)
		return false;

	if(digitDestinations == NULL)
		return false;

	if(digitSource == NULL)
		return false;

	viewport = *(getTargetPane())->getViewport();

	if(mBackgroundTextureID != -1)
	{
		lpTM->RenderTextureToViewport( mBackgroundTextureID, viewport, &mBackgroundDestination, NULL);
	}


	for(iID = 0; iID < mMaxDestination; iID++)
	{
		for(int digitIndex = 0; digitIndex < numDigits[iID]; digitIndex++)
		{
			source = getSource(digitSource[iID][digitIndex]);
			destination = digitDestinations[iID][digitIndex];
			lpTM->RenderTextureToViewport( mSource->getTextureID(), viewport, &destination, source   );
		}
	}

	return true;

}

void numberLocation::recalculateDigitDestinations(int destinationToReDo)
{
	int theNumber;
	int tmpI;
	int tmpCount;
	int tmpDigit;
	int h,w;
	float scale;
	SDL_Rect tmpR;

	if(mDestinationSource == NULL || mSource == NULL)
		return;

	theNumber = mDestinationSource[destinationToReDo];
	// clear out the old stuff.  Probably a bit more efficient to only free and re-allocate the memory if the number of digits changes
	// something for the "todo" list.
	if(digitDestinations != NULL)
	{
		if(digitDestinations[destinationToReDo] != NULL)
			delete[] digitDestinations[destinationToReDo];
	}
	else
		digitDestinations = new SDL_Rect*[mMaxDestination];

	if(digitSource != NULL)
	{
		if(digitSource[destinationToReDo] != NULL)
			delete[] digitSource[destinationToReDo];
	}
	else
		digitSource = new int*[mMaxDestination];

	if(numDigits != NULL)
		numDigits[destinationToReDo] = -1;
	else
		numDigits = new int[mMaxDestination];

	tmpI = 10;
	tmpCount = 1;
	while(tmpI <= theNumber)
	{
		tmpI *= 10;
		tmpCount++;
	}

	numDigits[destinationToReDo] = tmpCount;
	digitDestinations[destinationToReDo] = new SDL_Rect[tmpCount];
	digitSource[destinationToReDo] = new int[tmpCount];

	h = 0;
	w = 0;

	for(tmpI = numDigits[destinationToReDo] - 1;tmpI >=0 ; tmpI--)
	{
		tmpDigit = theNumber % 10;
		digitSource[destinationToReDo][tmpI] = tmpDigit;
		theNumber -= tmpDigit;
		theNumber /= 10;
		w += mSource->getSource(tmpDigit)->w;
		if (mSource->getSource(tmpDigit)->h > h)
			h = mSource->getSource(tmpDigit)->h;
	}

	// scale to just fit it in vertically
	scale = (float)mDestination[destinationToReDo].h / (float)h;
	// check if need to scale it to fit it in horizontally instead.
	if(scale > (float)mDestination[destinationToReDo].w / (float)w)
	{
		scale = (float)mDestination[destinationToReDo].w / (float)w;
	}

	if(mJustification[destinationToReDo] == LEFT_JUSTIFY)
		w =  mDestination[destinationToReDo].x;
	else if(mJustification[destinationToReDo] == RIGHT_JUSTIFY)
		w =  mDestination[destinationToReDo].x + mDestination[destinationToReDo].w - (w * scale);
	else if(mJustification[destinationToReDo] == CENTRE_JUSTIFY)
		w = mDestination[destinationToReDo].x + (mDestination[destinationToReDo].w /2) - ((w * scale) /2);


	for(tmpI = 0; tmpI < numDigits[destinationToReDo]; tmpI++)
	{
		tmpR = *(mSource->getSource(digitSource[destinationToReDo][tmpI]));
		digitDestinations[destinationToReDo][tmpI].w = tmpR.w * scale;
		digitDestinations[destinationToReDo][tmpI].h = tmpR.h * scale;
		digitDestinations[destinationToReDo][tmpI].x = w;
		w += digitDestinations[destinationToReDo][tmpI].w;
		digitDestinations[destinationToReDo][tmpI].y = mDestination[destinationToReDo].y;
	}
}

bool numberLocation::setValue(int destinationID, int someNumber)
{
	if(mDestinationSource == NULL || destinationID >= mMaxDestination)
	{
		return false;
	}
	mDestinationSource[destinationID] = someNumber;
	recalculateDigitDestinations(destinationID);

	return true;
}

bool numberLocation::setValue(int someNumber)
{
	if(mDestinationSource == NULL)
		return false;

	mDestinationSource[0] = someNumber;
	recalculateDigitDestinations(0);

	return true;
}