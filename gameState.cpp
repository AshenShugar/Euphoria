#include "gameState.hpp"

locationStateValue::locationStateValue()
{
	destinationID = -1;
	sourceID = -1;
	value = -1;
	valueSet = false;
}

locationStateValue::~locationStateValue()
{
	// nothing to do I don't believe.
}

locationStateValue::locationStateValue(int dID, int sID, int* pvalue)
{
	destinationID = dID;
	sourceID = sID;
	if(pvalue != NULL)
	{
		value = *pvalue;
		valueSet = true;
	}
	else
	{
		value = -1;
		valueSet = false;
	}
}

locationState::locationState()
{
	gloc = NULL;
	valueCount = 0;
	locationValues = NULL;
}

locationState::~locationState()
{
	if(locationValues != NULL)
	{
		for(int x = 0; x < valueCount; x++)
		{
			if(locationValues[x] != NULL)
			{
				delete locationValues[x];
			}
		}
		delete[] locationValues;
	}
}

locationState::locationState(LOCATION_TYPE lType, gameLocation* lpGLoc)
{
	gloc = lpGLoc;
	valueCount = 0;
	locationValues = NULL;
	locType = lType;
}

locationState::locationState(LOCATION_TYPE lType, gameLocation* lpGLoc, int dID, int sID)
{
	locType = lType;
	gloc = lpGLoc;
	locationValues = NULL;

	if( addStateValue(dID,sID,NULL) )
		valueCount = 1;
	else
		valueCount = 0;

}

locationState::locationState(LOCATION_TYPE lType, gameLocation* lpGLoc, int dID, int sID, int value)
{
	locType = lType;
	gloc = lpGLoc;
	if( addStateValue(dID,sID,&value) )
		valueCount = 1;
	else
		valueCount = 0;
}


gameLocation* locationState::GetGameLocation(void)
{
	return gloc;
}

void locationState::SetGameLocation(gameLocation* lpGLoc)
{
	gloc = lpGLoc;
}

int locationStateValue::GetDestinationID(void)
{
	return destinationID;
}

void locationStateValue::SetDestinationID(int dID)
{
	destinationID = dID;
}


int locationStateValue::GetSourceID(void)
{
	return sourceID;
}

void locationStateValue::SetSourceID(int sID)
{
	sourceID = sID;
}

// pass a point to an int in.  If value has been set, it will be set to value and true returned
// if value hasn't been set yet, false will get returned
bool locationStateValue::GetValue(int* pvalue)
{
	if(valueSet)
		*pvalue = value;
	return valueSet;

}

// Not allowed to set the value of the location to "not set"
// Though depending on the location, it could be set to an invalid value like -1 to indicate it's
// not to be used, but that will be entirely up to the gameLocation to figure out.
void locationStateValue::SetValue(int ivalue)
{
	value = ivalue;
	valueSet = true;
}

LOCATION_TYPE locationState::GetLocationType(void)
{
	return locType;
}

void locationState::SetLocationType(LOCATION_TYPE locationType)
{
	locType = locationType;
}

// should have smarter memory management, but it should be fine as long as things don't get too big
// and change often.
bool locationState::addStateValue(int d, int s, int* v)	// add's a completely new locationStateValue
{
	locationStateValue* tmpLSValue = new locationStateValue(d,s,v);

	if(tmpLSValue == NULL)
	{
fprintf(stderr, "Unsuccessful in allocation memory in addStateValue()\n");
		return false;
	}
	// check if there are no locationStateValue's at all for this location yet
	if(locationValues == NULL)
	{
fprintf(stderr,"Adding first state Value\n");
		// just add a single locationStateValue
		locationValues = new locationStateValue*[1];
		locationValues[0] = tmpLSValue;
		valueCount = 1;
	}
	else
	{
fprintf(stderr,"Adding additinoal state Value\n");

		// need to allocate more memory to hold the new locationStateValue
		// Can't just add more memory onto the end, so create a whole new array, copy the values across and release the old memory
		// these are just pointers to the locationStateValues, not the actual locationStateValues, so it's not as bad as it looks.
		locationStateValue** newList = new locationStateValue*[valueCount + 1];
		if(newList == NULL)
		{
			return false;
fprintf(stderr,"Error allocation memory for new locationStateValue array\n");
		}
		for (int x = 0; x < valueCount; x++)
		{
			newList[x] = locationValues[x];
		}
		valueCount++;
		newList[valueCount-1] = tmpLSValue;
		delete[] locationValues;
		locationValues = newList;
	}
	return true;
}

// Set locationStateValue with destinationID of d to have sourceID of s and value pointed at by v
bool locationState::setStateValue(int d, int s, int* v)
{
	bool returnValue = false;
	locationStateValue* tmpptr;

	if(locationValues == NULL)
		return returnValue;

	for(int x = 0; x < valueCount; x++)
	{
		tmpptr = locationValues[x];

		if(tmpptr->GetDestinationID() == d)
		{
			tmpptr->SetSourceID(s);
			tmpptr->SetValue(*v);
			returnValue = true;
			break;
		}
	}
	return returnValue;
}


void locationState::SyncToLocation(void)
{
	locationStateValue* lpTmpLSV;
	int tmpValue,tmpDID,tmpSID;
	bool hasValue;

	// clear out all the old values
	if(gloc == NULL)
	{
fprintf(stderr, "game Location == NULL in SyncToLocation()\n");
		return;
	}
	else
		gloc->clearValues();
	
	
	for(int x = 0; x < valueCount; x++)
	{
		lpTmpLSV = locationValues[x];
		if( lpTmpLSV == NULL)
		{
fprintf(stderr, "locationValues[%d] == NULL.  WTF?\n", x);
		}
		else
		{
			tmpDID = lpTmpLSV->GetDestinationID();
			tmpSID = lpTmpLSV->GetSourceID();
			hasValue = lpTmpLSV->GetValue(&tmpValue);	// returns true
		}


		if( locType == NOTSET)
		{
			// no idea what to do I guess
		}
		else if(locType == MVtoML)
		{
			gloc->setValue(tmpDID, tmpSID);
		}
		else  if(locType == OVtoOL)
		{
			gloc->setValue(tmpDID, tmpSID);
		}
		else  if(locType == MCVtoOL)
		{
			// just add the values to the location since the old values got cleared before this loop
			((Mto1gLocation*)gloc)->addValue(tmpSID);
		}
		else if(locType == NMVtoML)
		{
			gloc->setValue(tmpDID, tmpValue);
		}
		else
		{
			// Something's screwed up		
		}
	}
}

gameState::gameState()
{
	locationCount = 0;
	Head = NULL;
	gameRound = -1;
	gamePhase = -1;
	gameSubPhase = -1;
	gamePhaseList = NULL;
	gameSubPhaseList = NULL;
}

gameState::gameState(int NumberOfStates)
{
	locationCount = NumberOfStates;
	Head = new locationState*[NumberOfStates];
	gameRound = -1;
	gamePhase = -1;
	gameSubPhase = -1;
	gamePhaseList = NULL;
	gameSubPhaseList = NULL;
}

gameState::~gameState()
{
	if(gameSubPhaseList != NULL)
	{
		for(int x = 0; x < gamePhaseCount; x++)
		{
			if( gameSubPhaseList[x] != NULL )
			{
				delete[] gameSubPhaseList[x];
			}
		}
		delete[] gameSubPhaseList;
	}

	if( gamePhaseList != NULL)
		delete[] gamePhaseList;

	if(Head != NULL)
	{
		for(int x = 0; x < locationCount; x++)
		{
			delete Head[x];
		}
		delete[] Head;
	}
}


// Could be made smarter by adding another AddLocationState function that also takes the total number of expected
// locationStates, so just allocate the memory once rather than continually allocating and freeing memory as
// states get added.  Since this should only happen once when the game loads, it can be fixed later if at all.
locationState* gameState::AddLocationState(LOCATION_TYPE lType, gameLocation* gloc)
{
	locationState* tmpLS;
	locationState** newHead;

	if( Head == NULL)
	{
		Head = new locationState*[1];
		tmpLS = new locationState(lType, gloc);
		Head[0] = tmpLS;
		locationCount = 1;
	}
	else
	{
		newHead = new locationState*[locationCount + 1];
		tmpLS = new locationState(lType, gloc);
		for(int x = 0; x < locationCount; x++)
		{
			newHead[x] = Head[x];
		}
		Head[locationCount++] = tmpLS;
		delete[] Head;
	}
	return tmpLS;
}

// set the location and type of a locationState that's already been created
locationState* gameState:: SetLocationState(int index, LOCATION_TYPE lType, gameLocation* gloc)
{
	if(index >= locationCount)
		return NULL;
	if( Head == NULL)
		return NULL;
	if( Head[index] == NULL )
	{
		Head[index] = new locationState(lType, gloc);
	}
	else
	{
		Head[index]->SetLocationType(lType);
		Head[index]->SetGameLocation(gloc);
	}
}

// if we add the locationStates in the same order as the gamelocation enum, we should be fine
// to get a particular locationState just by knowing the enum value and using it as an index 
// into the array of locationStates
locationState* gameState::GetLocationX(int x)
{
	if(locationCount > x && x >= 0)
		return Head[x];
	else
		return NULL;
}

locationState* gameState::FindLocationState(gameLocation* gloc)
{
	for(int x = 0; x < locationCount; x++)
	{
		if(gloc == Head[x]->GetGameLocation() )
		{
			return Head[x];
		}
	}
	return NULL;
}

bool gameState::SyncLocation(locationState* locToSync)
{

	locToSync->SyncToLocation();
}

// sync all the location States
bool gameState::SyncLocations(void)
{
	for(int x = 0; x < locationCount; x++)
	{
		if(Head[x] != NULL)
		{
			Head[x]->SyncToLocation();
		}
	}
}

