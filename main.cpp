
//Using SDL, SDL_image, standard IO, math, and strings

#include <string>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <cmath>

#include "myEnum.hpp"
#include "gameLocation.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 700;

SDL_Renderer* gRenderer = NULL;
TextureManager* gTM;	// manages loading and freeing all textures
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
int gMousex,gMousey, gMouseState;

gameLocation boardLocation[NUMLOCALS];

gameLocation* targetedLocal;
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


		//Set mouse over sprite
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

	success = gTM->LoadTextureInfoFromFile("TextureInfo.txt");

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
			Pane aPane(0,0,804,700);
			char filename[15];

			boardLocation[WASTESTARS].LoadFromFile("wasteStars.txt");
			boardLocation[ICARITESTARS].LoadFromFile("icariteStars.txt");
			boardLocation[EUPHORIASTARS].LoadFromFile("euphoriaStars.txt");
			boardLocation[SUBSTARS].LoadFromFile("subStars.txt");
			boardLocation[EF1].LoadFromFile("ef1.txt");
			boardLocation[EF2].LoadFromFile("ef2.txt");

	boardLocation[WF1].LoadFromFile("wf1.txt");
	boardLocation[WF2].LoadFromFile("wf2.txt");
	boardLocation[SF1].LoadFromFile("sf1.txt");
	boardLocation[SF2].LoadFromFile("sf2.txt");
	boardLocation[IF1U].LoadFromFile("if1u.txt");
	boardLocation[IF2U].LoadFromFile("if2u.txt");
	boardLocation[IF3U].LoadFromFile("if3u.txt");
	boardLocation[WIND_SALON].LoadFromFile("iwindsalon.txt");
	boardLocation[CLOUD_MINE].LoadFromFile("imine.txt");
	boardLocation[IoHA].LoadFromFile("eioha.txt");
	boardLocation[ETUNNEL].LoadFromFile("edig.txt");
	boardLocation[EF1U].LoadFromFile("ef1u.txt");
	boardLocation[EF2U].LoadFromFile("ef2u.txt");
	boardLocation[GENERATOR].LoadFromFile("egenerator.txt");
	boardLocation[ETUNNELU].LoadFromFile("etu.txt");
	boardLocation[AoFM].LoadFromFile("waofm.txt");
	boardLocation[WTUNNEL].LoadFromFile("wdig.txt");
	boardLocation[WF1U].LoadFromFile("wf1u.txt");
	boardLocation[WF2U].LoadFromFile("wf2u.txt");
	boardLocation[FARM].LoadFromFile("wfarm.txt");
	boardLocation[WTUNNELU].LoadFromFile("wtu.txt");
	boardLocation[FPoHR].LoadFromFile("sfpohr.txt");
	boardLocation[SUBTUNNEL].LoadFromFile("sdig.txt");
	boardLocation[SF1U].LoadFromFile("sf1u.txt");
	boardLocation[SF2U].LoadFromFile("sf2u.txt");
	boardLocation[AQUIFER].LoadFromFile("saquifer.txt");
	boardLocation[SUBTUNNELU].LoadFromFile("stu.txt");
	boardLocation[WORKER_ACTIVATE].LoadFromFile("workeractivate.txt");
	boardLocation[E_TRACK].LoadFromFile("etrack.txt");
	boardLocation[S_TRACK].LoadFromFile("strack.txt");
	boardLocation[W_TRACK].LoadFromFile("wtrack.txt");
	boardLocation[I_TRACK].LoadFromFile("itrack.txt");

			sprites playerStars("playerStars.txt");
			sprites playerDice("dice.txt");
			
			targetedLocal = NULL;
			targetDestinationID = -1;

			for(int i = 0; i<6; i++)
			{
				sprintf(filename, "moralTrack%d.txt",i);
				std::string filenameX = filename;
				boardLocation[i].LoadFromFile(filenameX);
				boardLocation[i].setDefaultSource(i);
				boardLocation[i].setValue(0);

				sprintf(filename, "brainTrack%d.txt",i);
				filenameX = filename;
				boardLocation[i+6].LoadFromFile(filenameX);
				boardLocation[i+6].setDefaultSource(i);
				boardLocation[i+6].setValue(0);
			}

			for(int i = 0; i < NUMLOCALS; i++)
			{
				boardLocation[i].setTargetPane(&aPane);
				if(i < 15)
					boardLocation[i].setSourceArray(&playerStars);
				else
					boardLocation[i].setSourceArray(&playerDice);

			}


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
					mouseEvent(&e);
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
					{
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

						}
					}
				}

				// Update all the various data structures with the information from the "input".
				tDelta = SDL_GetTicks() - tPrev;
				tPrev = SDL_GetTicks();
				update(tDelta);

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
				SDL_RenderClear( gRenderer );

				gTM->RenderTextureToViewport(0, *aPane.getViewport());

				for(int i = 0; i < NUMLOCALS; i++)
				{
					boardLocation[i].draw(gTM);
				}


				if(gMouseState == 1 && targetDestinationID == -1)
				{
					fprintf(stderr,"x=%d,y=%d\n",gMousex,gMousey);

					for( int i=0; i < NUMLOCALS; i++)
					{
						targetDestinationID = boardLocation[i].isTargeted(gMousex,gMousey);
						if(targetDestinationID > -1)
						{
							targetedLocal = &boardLocation[i];
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
						targetedLocal->setValue(targetDestinationID, selectedPlayer);

					targetedLocal = NULL;
					targetDestinationID = -1;	// reset mouse

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