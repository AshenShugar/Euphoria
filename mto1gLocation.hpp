#include "gameLocation.hpp"

class Mto1gLocation: public gameLocation
{
	public:
		Mto1gLocation();
		Mto1gLocation(std::string filename);
		~Mto1gLocation();

		virtual bool draw(TextureManager* TM);
		bool removeValue(int sourceID);
		bool addValue(int sourceID);
		void calculateDestinations(void);
		virtual bool setValue(int destinationID, int sourceID); 


	private:
		int mNextIndex;
		int mMaxIndex;
		int mNumDestinations;
		SDL_Rect* mTmpDestinations;
};