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
		bool good(void);
		LOCATION_TYPE getType(void);
		bool isClickable(void);
		void setClickable(bool isClickable);
		virtual bool draw(TextureManager* TM);
		std::string mFilename;

	protected:
		Pane* getTargetPane(void);
		SDL_Rect getDestination(int destinationID);
		SDL_Rect* getSource(int sourceID);

		int mBackgroundTextureID;
		int mMaxDestination;
		int mDefaultSource;
		SDL_Rect mBackgroundDestination;
		SDL_Rect* mDestination;
		sprites* mSource;
		int* mDestinationSource;
		Pane* mTargetPane;
		LOCATION_TYPE mType;
		bool mClickable;
};

#endif