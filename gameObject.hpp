#include "myEnum.hpp"
#include "pane.hpp"
#include "textureManager.h"
#include <string>
#include <fstream>


class gameObject
{
	public:
		gameObject();
		gameObject(std::string filename);
		~gameObject();
		void LoadObjectFromFile(std::string filename);
		bool setTargetPane(Pane*);
		Pane* getTargetPane(void);
		bool setValue(int value);
		bool good(void);

		int getTID(void);
		SDL_Rect getDestination(void);
		SDL_Rect getSource(void);
		bool draw(TextureManager* TM);

	protected:
		int mBackgroundTextureID;
		int mTextureID;
		int mMaxValue;
		SDL_Rect* mDestination;
		SDL_Rect mSource;
		bool mReady;
	private:

		Pane* mTargetPane;
		int mValue;

};