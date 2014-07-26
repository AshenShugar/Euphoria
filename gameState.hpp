#include "myEnum.hpp"

#include "mto1gLocation.hpp"
#include "numberLocation.hpp"

class locationStateValue
{
	public:
		locationStateValue();
		locationStateValue(int dID, int sID, int* value);
		~locationStateValue();

		int GetDestinationID(void);
		void SetDestinationID(int dID);
		int destinationID;

		int GetSourceID(void);
		void SetSourceID(int sID);
		int sourceID;

		bool GetValue(int* value);
		void SetValue(int value);
		int value;
		bool valueSet;		
};

class locationState
{
	public:
		locationState();
		locationState(LOCATION_TYPE lType,gameLocation* gloc);
		locationState(LOCATION_TYPE lType,gameLocation* gloc, int dID,int sID);
		locationState(LOCATION_TYPE lType,gameLocation* gloc, int dID,int sID,int value);
		~locationState();

		gameLocation* GetGameLocation(void);
		void SetGameLocation(gameLocation* gloc);
		gameLocation* gloc;

		LOCATION_TYPE GetLocationType(void);
		void SetLocationType(LOCATION_TYPE locType);
		LOCATION_TYPE locType;

		// set the game location to have the given destinationID,SourceID and Value.
		// it will use the locType to determine which of the three are relevant.
		void SyncToLocation(void);
		bool addStateValue(int d, int s, int* v);	// add's a completely new locationStateValue
		bool setStateValue(int d, int s, int* v);	// finds a locationStateValue with matching
								// d and updates it's s or v value
	protected:
		locationStateValue** locationValues;	// an array of locationStateValues's, even if there's only 1
		int valueCount;
};

class gameState
{
	public:
		gameState();
		gameState(int NumberOfStates);
		~gameState();
		// sync a particular locationState to the gamelocation
		bool SyncLocation(locationState*);
		// sync all the location States
		bool SyncLocations(void);
		locationState* AddLocationState(LOCATION_TYPE lType, gameLocation* gloc);
		locationState* SetLocationState(int Index, LOCATION_TYPE lType, gameLocation* gloc);
		locationState* FindLocationState(gameLocation* gloc);
		locationState* GetLocationX(int x);

	private:
		locationState** Head;
		int locationCount;
		int gameRound;
		int gamePhase;
		int gameSubPhase;
		int* gamePhaseList;
		int gamePhaseCount;
		int** gameSubPhaseList;	// need a subphase list for each phase, so an array of array of ints.
};