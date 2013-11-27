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

		bool setValue(int destinationID, int sourceID); 
		bool setValue(int destinationID); // Assume only 1 source
		bool setValueS(int); // Assume only 1 destination
		bool good(void);
		bool draw(TextureManager* TM);
		std::string mFilename;

	protected:
		Pane* getTargetPane(void);
		SDL_Rect getDestination(int destinationID);
		SDL_Rect* getSource(int sourceID);

		int mBackgroundTextureID;
		int mMaxDestination;
		int mDefaultSource;
		SDL_Rect* mDestination;
		sprites* mSource;
		int* mDestinationSource;
		Pane* mTargetPane;
};