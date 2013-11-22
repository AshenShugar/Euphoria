#include <SDL.h>
#include "myEnum.hpp"
#include "hexlocation.h"

class hexTile
{
	public:
		hexTile();
		bool setType(TERRAINTYPE lType);
		bool setFeature(Feature* lFeature, bool visible=false);
		bool setLocation(hexLocation lLocation);
		bool showFeature(void);
		int movementValue(bool Daytime);
	private:
		int id;
		TERRAINTYPE mType;
		Feature *mFeature
		hexLocation mLocation;
		int TypeTextureID;
		bool FeatureVisible;
		int mOrientation;	// Not needed for mage Knight
}