#include "gameObject.hpp"

class gameObjectB: public gameObject
{
	public:
		gameObjectB();
		~gameObjectB();
		gameObjectB(std:string filename);

		void LoadObjectFromFile(std::string filename);
		bool draw(TextureManager* TM);
		bool setValue(int location, int value);

	private:
		SDL_Rect* destinations;	// array
		SDL_Rect* sources;	// array
		int* destinationValues;	// array

};