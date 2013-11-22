#include "myEnum.hpp"

class Feature
{
	public:
		Feature();
		Feature(int visibleTextureID, int hiddenTextureID, FEATURE featureType)
	private:
		int mVisibleTextureID;
		int mHiddenTextureID;
		FEATURE mFeature;
		int mFeatureID;
}