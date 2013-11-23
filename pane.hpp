#include "myEnum.hpp"
#include <SDL.h>

class Pane
{
	public:

		Pane();
		Pane(int x, int y, int w, int h);
		Pane(int x, int y, int w, int h, myEDGE neighbourIs, Pane* neighbour);
		void attachNeighbour(myEDGE neighbourIs, Pane* neighbour); 
		~Pane();
		void setViewport(SDL_Rect* viewport);
		SDL_Rect* getViewport(void);
		bool resize(myEDGE edge, int amount);
		bool resizeTo(myEDGE, int location);
		bool minimise();
		bool maximise();
		bool unminimise();
		bool mMinimised;

	private:
		bool resizeB(myEDGE edge, int amount);
		void clearOut(void);
		myEDGE oppositeEdge(myEDGE edge);
		int mx,my,mw,mh;
		bool mResizing;
		myEDGE mResizingEdge;
		int mResizeAmount;
		SDL_Rect mViewport;
		int mNeighbourCount[4];
		Pane **mNeighbour[4];
		void updateViewport(myEDGE edge);
		void resetFromViewport(myEDGE edge);

};