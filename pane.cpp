#include "pane.hpp"

myEDGE Pane::oppositeEdge(myEDGE edge)
{
	if(edge == TOP)
		return BOTTOM;
	if(edge == BOTTOM)
		return TOP;
	if(edge == LEFT)
		return RIGHT;
	if(edge == RIGHT)
		return LEFT;
	if(edge == NOEDGE)
		return NOEDGE;
}


void Pane::clearOut(void)
{
	mNeighbour[TOP] = NULL;
	mNeighbour[BOTTOM] = NULL;
	mNeighbour[LEFT] = NULL;
	mNeighbour[RIGHT] = NULL;

	mNeighbourCount[TOP] = 0;
	mNeighbourCount[BOTTOM] = 0;
	mNeighbourCount[LEFT] = 0;
	mNeighbourCount[RIGHT] = 0;

	mx = 0;
	my = 0;
	mw = 0;
	mh = 0;

	mResizing = false;
	mResizingEdge = NOEDGE;
	mResizeAmount = 0;

	mViewport.x = 0;
	mViewport.y = 0;
	mViewport.h = 0;
	mViewport.w = 0;
}

Pane::Pane(void)
{
	clearOut();
}

Pane::Pane(int x, int y, int w, int h)
{
	clearOut();
	mx = x;
	my = y;
	mw = w;
	mh = h;
	mViewport.x = x;
	mViewport.y = y;
	mViewport.h = h;
	mViewport.w = w;
}

Pane::Pane(int x, int y, int w, int h, myEDGE neighbourIs, Pane* neighbour)
{
	clearOut();
	mx = x;
	my = y;
	mw = w;
	mh = h;
	mViewport.x = x;
	mViewport.y = y;
	mViewport.h = h;
	mViewport.w = w;
	mResizing = false;
	mResizingEdge = NOEDGE;
	mResizeAmount = 0;
	for( int i=0; i<4; i++)
	{
		if(i == neighbourIs)
		{
			mNeighbour[i] = new Pane*[1];
			mNeighbour[i][0] = neighbour;
			mNeighbourCount[i]++;
		}
		else
			mNeighbour[i] = NULL;
	}
}


void Pane::attachNeighbour(myEDGE neighbourIs, Pane* neighbour)
{
	Pane** newNeighbourArray;

	mNeighbourCount[neighbourIs]++;

	newNeighbourArray = new Pane*[mNeighbourCount[neighbourIs]];

	for( int i = 0; i < mNeighbourCount[neighbourIs] - 1; i++)
	{
		newNeighbourArray[i] = mNeighbour[neighbourIs][i];
	}
	newNeighbourArray[mNeighbourCount[neighbourIs] - 1] = neighbour;

	delete[] mNeighbour[neighbourIs];

	mNeighbour[neighbourIs] = newNeighbourArray;
}

Pane::~Pane()
{
	for(int i = 0; i < 4; i++)
	{
		delete[] mNeighbour[i];
	}
}

void Pane::setViewport(SDL_Rect* viewport)
{
	mViewport = *viewport;
}

SDL_Rect* Pane::getViewport(void)
{
	return &mViewport;
}

bool Pane::resizeTo(myEDGE edge, int location)
{
	bool bResult = true;
	int iAmount = 0;

	switch(edge)
	{
		case TOP:
			iAmount = location - (my + mh);
			break;
		case BOTTOM:
			iAmount = location - my;
			break;
		case LEFT:
			iAmount = location - mx;
			break;
		case RIGHT:
			iAmount = location - (mx + mw);
			break;
	}
	bResult = resize(edge,iAmount);
	return bResult;

}

bool Pane::resize(myEDGE edge, int amount)
{
	bool bResult = true;

	if( edge == NOEDGE || amount == 0)
	{
		return bResult;
	}

	bResult = resizeB(edge,amount);

	if(bResult == true)
	{
		updateViewport(edge);
	}
	else
	{
		resetFromViewport(edge);
	}
}


bool Pane::resizeB(myEDGE edge, int amount)
{
	bool bResult = true;
	if( edge == NOEDGE || amount == 0)
	{
		return bResult;
	}

	// If pane is in process of being resized, don't try and resize it again
	if(mResizing == true)
	{
		return bResult;
	}

	// if pane doesn't have a neighbour on the edge that's being resized,
	//  assume it's a screen edge and don't allow it to be resized
	if(mNeighbourCount[edge] == 0)
	{
		fprintf(stderr, "Trying to resize screen edge\n");
		return false;
	}

	mResizing = true;
	mResizingEdge = edge;

	for( int i=0; i < mNeighbourCount[edge];i++)
	{
		if(mNeighbour[edge] != NULL)
		{
			if(mNeighbour[edge][i] != NULL)
			{
				if(!(mNeighbour[edge])[i]->resizeB( oppositeEdge(edge), amount))
				{
					bResult = false;
				}
			}
		}
	}

	if(bResult == true)
	{
		switch(edge)
		{
			case TOP:
				if(mh + amount <= 0)
				{
					bResult = false;
				}
				else
					mh += amount;
				break;
			case BOTTOM:
				if (mh - amount <= 0 || my + amount <= 0)
				{
					bResult = false;
				}
				else
				{
					mh -= amount;
					my += amount;
				}
				break;
			case LEFT:
				if (mw - amount <= 0 || mx + amount <= 0)
					bResult = false;
				else
				{
					mw -= amount;
					mx += amount;
				}
				break;
			case RIGHT:
				if(mw + amount <= 0)
					bResult = false;
				else
					mw += amount;
				break;
		}
	}
	
	mResizing = false;
	mResizingEdge = NOEDGE;
	return bResult;
}

void Pane::resetFromViewport(myEDGE edge)
{
	if(mResizing == true)
		return;

	mResizing = true;

	for( int i=0; i < mNeighbourCount[edge];i++)
	{
		if(mNeighbour[edge] != NULL)
		{
			if(mNeighbour[edge][i] != NULL)
			{
				(mNeighbour[edge])[i]->resetFromViewport(oppositeEdge(edge));
			}
		}
	}

	mx = mViewport.x;
	my = mViewport.y;
	mh = mViewport.h;
	mw = mViewport.w;

	mResizing = false;
}

void Pane::updateViewport(myEDGE edge)
{
	if(mResizing == true)
		return;

	mResizing = true;

	for( int i=0; i < mNeighbourCount[edge];i++)
	{
		if(mNeighbour[edge] != NULL)
		{
			if(mNeighbour[edge][i] != NULL)
			{
				(mNeighbour[edge])[i]->updateViewport(oppositeEdge(edge));
			}
		}
	}

	mViewport.x = mx;
	mViewport.y = my;
	mViewport.h = mh;
	mViewport.w = mw;

	mResizing = false;
}

bool Pane::minimise()
{
	bool bResult = true;

	return bResult;
}

bool Pane::maximise()
{
	bool bResult = true;

	return bResult;
}

bool Pane::unminimise()
{
	bool bResult = true;

	return bResult;
}