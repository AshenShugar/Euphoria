
//Using SDL, SDL_image, standard IO, math, and strings

#include <string>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <cmath>

#include "myEnum.hpp"
//#include "gameLocation.hpp"
#include "mto1gLocation.hpp"
#include "numberLocation.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 700;

SDL_Renderer* gRenderer = NULL;
TextureManager* gTM;	// manages loading and freeing all textures
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
int gMousex,gMousey, gMouseState;

gameLocation* boardLocation[NUMLOCALS];
gameLocation* playerStuff[ITEMCOUNT];

gameLocation* targetedLocal;
Mto1gLocation* targetedLocal2;
int targetDestinationID;

//The window renderer

bool init()
{
	//Initialization flag
	bool success = true;
	gMouseState = 0;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Enable VSync
		if( !SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" ) )
		{
			printf( "Warning: VSync not enabled!" );
		}

		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Euphoria:BaBD (single player)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				gTM = new TextureManager(gRenderer);
			}
		}
	}

	return success;
}

void mouseEvent(SDL_Event* e)
{
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		SDL_GetMouseState( &gMousex, &gMousey );

		switch( e->type )
		{
			case SDL_MOUSEBUTTONDOWN:
			gMouseState = 1;
			break;
				
			case SDL_MOUSEBUTTONUP:
			gMouseState = 0;
			break;
		}

	}
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	char filename[15];

	success = gTM->LoadTextureInfoFromFile("TextureInfo.txt");



	boardLocation[WASTESTARS] = new gameLocation("wasteStars.txt");
	boardLocation[ICARITESTARS] = new gameLocation("icariteStars.txt");
	boardLocation[EUPHORIASTARS] = new gameLocation("euphoriaStars.txt");
	boardLocation[SUBSTARS] = new gameLocation("subStars.txt");
	boardLocation[EF1] = new gameLocation("ef1.txt");
	boardLocation[EF2] = new gameLocation("ef2.txt");

	boardLocation[WF1] = new gameLocation("wf1.txt");
	boardLocation[WF2] = new gameLocation("wf2.txt");
	boardLocation[SF1] = new gameLocation("sf1.txt");
	boardLocation[SF2] = new gameLocation("sf2.txt");
	boardLocation[IF1U] = new gameLocation("if1u.txt");
	boardLocation[IF2U] = new gameLocation("if2u.txt");
	boardLocation[IF3U] = new gameLocation("if3u.txt");
	boardLocation[WIND_SALON] = new gameLocation("iwindsalon.txt");
	boardLocation[CLOUD_MINE] = new Mto1gLocation("imine.txt");
	boardLocation[IoHA] = new gameLocation("eioha.txt");
	boardLocation[ETUNNEL] = new gameLocation("edig.txt");
	boardLocation[EF1U] = new gameLocation("ef1u.txt");
	boardLocation[EF2U] = new gameLocation("ef2u.txt");
	boardLocation[GENERATOR] = new Mto1gLocation("egenerator.txt");
	boardLocation[ETUNNELU] = new gameLocation("etu.txt");
	boardLocation[AoFM] = new gameLocation("waofm.txt");
	boardLocation[WTUNNEL] = new gameLocation("wdig.txt");
	boardLocation[WF1U] = new gameLocation("wf1u.txt");
	boardLocation[WF2U] = new gameLocation("wf2u.txt");
	boardLocation[FARM] = new Mto1gLocation("wfarm.txt");
	boardLocation[WTUNNELU] = new gameLocation("wtu.txt");
	boardLocation[FPoHR] = new gameLocation("sfpohr.txt");
	boardLocation[SUBTUNNEL] = new gameLocation("sdig.txt");
	boardLocation[SF1U] = new gameLocation("sf1u.txt");
	boardLocation[SF2U] = new gameLocation("sf2u.txt");
	boardLocation[AQUIFER] = new Mto1gLocation("saquifer.txt");
	boardLocation[SUBTUNNELU] = new gameLocation("stu.txt");
	boardLocation[WORKER_ACTIVATE] = new gameLocation("workeractivate.txt");
	boardLocation[E_TRACK] = new gameLocation("etrack.txt");
	boardLocation[S_TRACK] = new gameLocation("strack.txt");
	boardLocation[W_TRACK] = new gameLocation("wtrack.txt");
	boardLocation[I_TRACK] = new gameLocation("itrack.txt");

	boardLocation[ST_TRACK] = new gameLocation("WTTrack.txt");
	boardLocation[WT_TRACK] = new gameLocation("ETTrack.txt");
	boardLocation[ET_TRACK] = new gameLocation("STTrack.txt");

	boardLocation[PLAYER_ARTIFACTS] = new gameLocation("partifacts.txt");
	boardLocation[PLAYER_DILEMMA] = new gameLocation("pdilemma.txt");
	boardLocation[PLAYER_WORKERS] = new gameLocation("pworkers.txt");
	boardLocation[PLAYER_RESOURCES] = new numberLocation("presources.txt");
	boardLocation[PLAYER_RECRUITS] = new gameLocation("precruits.txt");
	boardLocation[MARKET_TILES] = new gameLocation("factoryTiles.txt");
	boardLocation[MARKET_STARS] = new gameLocation("FactoryStars.txt");
	boardLocation[BLOCKED_LOCATIONS] = new gameLocation("BlockedLocations.txt");


	
	targetedLocal = NULL;
	targetDestinationID = -1;

	for(int i = 0; i<6; i++)
	{
		sprintf(filename, "moralTrack%d.txt",i);
		std::string filenameX = filename;
		boardLocation[i] = new gameLocation(filenameX);
		boardLocation[i]->setDefaultSource(i);
		boardLocation[i]->setValue(0);

		sprintf(filename, "brainTrack%d.txt",i);
		filenameX = filename;
		boardLocation[i+6] = new gameLocation(filenameX);
		boardLocation[i+6]->setDefaultSource(i);
		boardLocation[i+6]->setValue(0);
	}



	return success;
}

void close()
{
	//Free loaded images
	delete(gTM);
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


// AI will probably do its thing here
// Not sure what else might happen.
void update(Uint32 tDelta)
{

}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			int selectedPlayer = 0;
			bool quit = false;
			SDL_Rect destination,source;

	SDL_Rect tmpR;
	bool setLoc = false;
	tmpR.w = 10;
	tmpR.h = 10;

			Pane aPane(0,0,804,700);
			Pane bPane(804,0,SCREEN_WIDTH - 804,700);

			sprites playerStars("playerStars.txt");
			sprites playerDice("dice.txt");
			sprites playerArtifacts("artifacts.txt");
			sprites playerDilemma("dilemmas.txt");
			sprites playerResources("resources.txt");
			sprites playerRecruits("recruits.txt");
			sprites marketTiles("FactorySprites.txt");
			sprites digits("numberSprites.txt");
			sprites markers("gameMarkers.txt");
			sprites blockingTiles("BlockingTiles.txt");


			// Default everything to one pane then fix the few that belong in another pane.
			for(int i = 0; i < NUMLOCALS; i++)
			{
				boardLocation[i]->setTargetPane(&aPane);
			}

			boardLocation[PLAYER_ARTIFACTS]->setTargetPane(&bPane);
			boardLocation[PLAYER_DILEMMA]->setTargetPane(&bPane);
			boardLocation[PLAYER_RESOURCES]->setTargetPane(&bPane);
			boardLocation[PLAYER_WORKERS]->setTargetPane(&bPane);
			boardLocation[PLAYER_RECRUITS]->setTargetPane(&bPane);

			// The majority of locations use either dice or stars, so roughly set them all to that and fix the few different ones next.
			for(int i = 0; i < NUMLOCALS; i++)
			{
				if(i < 15)
					boardLocation[i]->setSourceArray(&playerStars);
				else
					boardLocation[i]->setSourceArray(&playerDice);

			}


			boardLocation[PLAYER_ARTIFACTS]->setSourceArray(&playerArtifacts);
			boardLocation[PLAYER_DILEMMA]->setSourceArray(&playerDilemma);
			boardLocation[PLAYER_RESOURCES]->setSourceArray(&digits);
			boardLocation[PLAYER_WORKERS]->setSourceArray(&playerDice);
			boardLocation[PLAYER_RECRUITS]->setSourceArray(&playerRecruits);
			boardLocation[MARKET_TILES]->setSourceArray(&marketTiles);
			boardLocation[MARKET_STARS]->setSourceArray(&playerStars);
			boardLocation[E_TRACK]->setSourceArray(&markers);
			boardLocation[I_TRACK]->setSourceArray(&markers);
			boardLocation[S_TRACK]->setSourceArray(&markers);
			boardLocation[W_TRACK]->setSourceArray(&markers);
			boardLocation[BLOCKED_LOCATIONS]->setSourceArray(&blockingTiles);
	

			// set the justification for number locations once now at the start as they shouldn't need to change during the game.
			((numberLocation*)(boardLocation[PLAYER_RESOURCES]))->setJustification(0,LEFT_JUSTIFY);
			((numberLocation*)(boardLocation[PLAYER_RESOURCES]))->setJustification(1, LEFT_JUSTIFY);
			((numberLocation*)(boardLocation[PLAYER_RESOURCES]))->setJustification(2, LEFT_JUSTIFY);
			((numberLocation*)(boardLocation[PLAYER_RESOURCES]))->setJustification(3, LEFT_JUSTIFY);
			((numberLocation*)(boardLocation[PLAYER_RESOURCES]))->setJustification(4,RIGHT_JUSTIFY);
			((numberLocation*)(boardLocation[PLAYER_RESOURCES]))->setJustification(5,RIGHT_JUSTIFY);
			((numberLocation*)(boardLocation[PLAYER_RESOURCES]))->setJustification(6,RIGHT_JUSTIFY);


			// variables to track timing incase I want to animate anything.
			Uint32 tBeginning = SDL_GetTicks();
			Uint32 tDelta = 0;
			Uint32 tPrev = SDL_GetTicks();

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{       /*********/
				// Input //
				/*********/
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					// Throw all events at my mouseEvent function and it'll decide if it's actually a mouse event or not.
					mouseEvent(&e);
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
					{
						// Handles basic keyboard input.  May want to move it to a keyboardEvent function if it needs to get fancier.
						// Just used in testing at the moment.
						switch( e.key.keysym.sym )
						{
							case SDLK_1:
								selectedPlayer = 0;
								break;		
							case SDLK_2:
								selectedPlayer = 1;
								break;

							case SDLK_3:
								selectedPlayer = 2;
								break;
							case SDLK_4:
								selectedPlayer = 3;
								break;
						
							case SDLK_5:
								selectedPlayer = 4;
								break;
												
							case SDLK_6:
								selectedPlayer = 5;
								break;
							case SDLK_x:
								tmpR.w++;
								break;
							case SDLK_z:
								if(tmpR.w - 1 > 0)
									tmpR.w--;
								break;
							case SDLK_y:
								tmpR.h++;
								break;
							case SDLK_t:
								if(tmpR.h - 1 > 0)
									tmpR.h--;
								break;

						}
					}
				}

				// Update all the various data structures with the information from the "input".
				tDelta = SDL_GetTicks() - tPrev;
				tPrev = SDL_GetTicks();
				update(tDelta);

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xBB, 0xBB, 0xBB, 0xFF );
				SDL_RenderClear( gRenderer );

				// Probably should change 0 and 8 to something more human readable, but they just refer to texture id's, so might be more trouble than worth it.
				gTM->RenderTextureToViewport(0, *aPane.getViewport());
				gTM->RenderTextureToViewport(8, *bPane.getViewport());

				// Draw all the locations.
				// Need to make sure in the enum that the locations like the player stars that appear on the markets
				//  come after the markets so aren't drawn underneath where they can't be seen.
				for(int i = 0; i < NUMLOCALS; i++)
				{
					boardLocation[i]->draw(gTM);
				}

// testing stuff
if(true)
{
				if(gMouseState == 1 && targetDestinationID == -1)
				{
					fprintf(stderr,"x=%d,y=%d\n",gMousex,gMousey);

					for( int i=0; i < NUMLOCALS; i++)
					{
						targetDestinationID = boardLocation[i]->isTargeted(gMousex,gMousey);
						if(targetDestinationID > -1)
						{
							targetedLocal = boardLocation[i];
							targetedLocal2 = (Mto1gLocation*)boardLocation[i];
							break;
						}
						else if( targetDestinationID == -2)
						{
							fprintf(stderr, "bad i = %d!\n", i);
						}
					}
				}

				if(gMouseState == 0 && targetDestinationID > -1)
				{
					if(targetedLocal->isTargeted(gMousex,gMousey) != - 1)
					{
						if (targetedLocal->getType() == MCVtoOL )
						{
							targetedLocal2->addValue(selectedPlayer);
						}
						else
						{
							targetedLocal->setValue(targetDestinationID, selectedPlayer);
						}
					}
					targetedLocal = NULL;
					targetDestinationID = -1;	// reset mouse

				}

}
else
{
/* Create accurate location info */
				if(gMouseState == 1)	// Mouse down, display sprite at location
				{
					int tmpSID = 0;
					sprites* tmpSprite;
					Pane* tmpP;
					tmpR.x = gMousex;
					tmpR.y = gMousey;
					setLoc = true;

					tmpSID = selectedPlayer;
					tmpSprite = &blockingTiles;
					if( gMousex >= (aPane.getViewport())->x &&  gMousex < ( (aPane.getViewport())->x + (aPane.getViewport())->w) )
					{
						tmpP = &aPane;
					}
					else
					{
						tmpP = &bPane;
						tmpR.x = gMousex - bPane.getViewport()->x;
						tmpR.y = gMousey - bPane.getViewport()->y;
					}
					gTM->RenderTextureToViewport(4, *tmpP->getViewport(), &tmpR, tmpSprite->getSource(tmpSID));
				}
}

				if(gMouseState == 0 && setLoc == true)
				{
					setLoc = false;
					fprintf(stderr,"%d %d %d %d\n", tmpR.x, tmpR.y, tmpR.w, tmpR.h);
				}

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}