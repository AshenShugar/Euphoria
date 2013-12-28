#include "gameLocation.hpp"

class numberLocation: public gameLocation
{
	public:
		numberLocation();
		numberLocation(std::string filename);
		~numberLocation();

		void setJustification(TEXT_JUSTIFICATION allignment);
		void setJustification(int locationID, TEXT_JUSTIFICATION allignment);

		virtual bool draw(TextureManager* TM);
		virtual bool setValue(int someNumber);
		virtual bool setValue(int destinationID, int someNumber);

		void recalculateDigitDestinations(int DestinationToReDo);
	protected:
		SDL_Rect** digitDestinations;
		int** digitSource;
		int* numDigits;
		TEXT_JUSTIFICATION* mJustification;
};