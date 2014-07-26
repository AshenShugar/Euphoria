#ifndef GAMELOCATIONLOADED

#define GAMELOCATIONLOADED

#include "myEnum.hpp"
#include <SDL.h>
#include <fstream>
#include <string>
#include "sprite.hpp"
#include "pane.hpp"
#include "textureManager.hpp"

class gameLocation
{
	public:
		gameLocation();
		gameLocation(std::string filename);
		~ gameLocation();
		void LoadFromFile(std::string filename);

		void setTargetPane(Pane*);
		void setSourceArray(sprites* source);
		bool setDefaultSource(int defaultSourceID);

		int isTargeted(int x, int y);

		virtual bool setValue(int destinationID, int sourceID); 
		virtual bool setValue(int destinationID); // Assume only 1 source
		virtual bool setValueS(int); // Assume only 1 destination
		virtual bool clearValues(void);	// remove all the values.
		bool good(void);
		LOCATION_TYPE getType(void);
		bool isClickable(void);
		void setClickable(bool isClickable);
		virtual bool draw(TextureManager* TM);
		std::string mFilename;
		void setColourModulation(int destinationID, int r,int g,int b);

	protected:
		Pane* getTargetPane(void);
		SDL_Rect getDestination(int destinationID);
		SDL_Rect* getSource(int sourceID);

		int mBackgroundTextureID;
		int mMaxDestination;		// the number of destinations in this location.
		int mDefaultSource;
		SDL_Rect mBackgroundDestination;	// to allow drawing a background for this location without covering the entire pane.
		SDL_Rect* mDestination;		// lists the possible destinations for sprites to be displayed in this location.
		sprites* mSource;		// a list of sprites that may be displayed in this location.
		int* mDestinationSource;	// lists the source id for what to display in a destination
		Pane* mTargetPane;		// the pane the location should be drawn too
		LOCATION_TYPE mType;		// what type of location this is.  Doesn't seem to be used yet.
		bool mClickable;		// whether this location should respond to mouse clicks.
		myRGB* mColourMod;		// a list of colour modulations to be applied to the corresponding destinations.
};

#endif