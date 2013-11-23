#include "myEnum.hpp"
#include <SDL.h>
#include <fstream>

class sprites
{
	public:
		sprites();
		sprites(std::string filename);
		~sprites();
		void LoadFromFile(std::string filename);
		SDL_Rect* getSource(int sourceID);
		int getTextureID(void);
		int getSourceCount(void);
	private:
		SDL_Rect* mSources;
		int mSourceCount;
		int mTID;
};