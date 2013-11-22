#include "myEnum.hpp"
#include "pane.hpp"
#include "textureManager.h"
#include <string>
#include <fstream>

class sprite
{
	public:
		sprite();
		sprite(int textureID,int x, int y, int w, int h);
		int textureID;
		int mx,my,mw,mh;	// destination coordinates
		int sx,sy,sw,sh;	// source coordinates
};

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

		int backgroundTextureID;
		int getSpriteTID(void);
		SDL_Rect getSpriteLocation(void);
		SDL_Rect getSpriteSource(void);
		bool draw(TextureManager* TM);

	private:
		bool mloaded;
		int mValue;
		int mMaxValue;
		sprite* valueSprites;
		Pane* targetPane;
};