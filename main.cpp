
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
			fprintf(stderr,"x=%d,y=%d\n",gMousex,gMousey);
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
			gameLocation moralTrack[6];
			gameLocation WasteLanderStars("wasteStars.txt");
			gameLocation IcariteStars("icariteStars.txt");
			gameLocation EuphoriaStars("euphoriaStars.txt");
			gameLocation SubterranStars("subStars.txt");
			sprites playerStars("playerStars.txt");
			//sprites playerDice("dice.txt");

			for(int i = 0; i<6; i++)
			{
				sprintf(filename, "moralTrack%d.txt",i);
				std::string filenameX = filename;
				moralTrack[i].LoadFromFile(filenameX);
				moralTrack[i].setTargetPane(&aPane);
				moralTrack[i].setSourceArray(&playerStars);
				moralTrack[i].setDefaultSource(i);
				moralTrack[i].setValue(0);
			}
			WasteLanderStars.setTargetPane(&aPane);
			IcariteStars.setTargetPane(&aPane);
			EuphoriaStars.setTargetPane(&aPane);
			SubterranStars.setTargetPane(&aPane);

			WasteLanderStars.setSourceArray(&playerStars);
			IcariteStars.setSourceArray(&playerStars);
			EuphoriaStars.setSourceArray(&playerStars);
			SubterranStars.setSourceArray(&playerStars);

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

							case SDLK_q:
								moralTrack[selectedPlayer].setValue(0);
								WasteLanderStars.setValue(0,selectedPlayer);
								IcariteStars.setValue(5,selectedPlayer);
								EuphoriaStars.setValue(0,selectedPlayer);
								SubterranStars.setValue(4,selectedPlayer);
								break;		

							case SDLK_w:
								moralTrack[selectedPlayer].setValue(1);
								WasteLanderStars.setValue(1,selectedPlayer);
								IcariteStars.setValue(4,selectedPlayer);
								EuphoriaStars.setValue(1,selectedPlayer);
								SubterranStars.setValue(0,selectedPlayer);
								break;

							case SDLK_e:
								moralTrack[selectedPlayer].setValue(2);
								WasteLanderStars.setValue(2,selectedPlayer);
								IcariteStars.setValue(3,selectedPlayer);
								EuphoriaStars.setValue(2,selectedPlayer);
								SubterranStars.setValue(2,selectedPlayer);
								break;

							case SDLK_r:
								moralTrack[selectedPlayer].setValue(3);
								WasteLanderStars.setValue(3,selectedPlayer);
								IcariteStars.setValue(2,selectedPlayer);
								EuphoriaStars.setValue(3,selectedPlayer);
								SubterranStars.setValue(5,selectedPlayer);
								break;
						
							case SDLK_t:
								moralTrack[selectedPlayer].setValue(4);
								WasteLanderStars.setValue(4,selectedPlayer);
								IcariteStars.setValue(1,selectedPlayer);
								EuphoriaStars.setValue(4,selectedPlayer);
								SubterranStars.setValue(1,selectedPlayer);
								break;
												
							case SDLK_y:
								moralTrack[selectedPlayer].setValue(5);
								WasteLanderStars.setValue(5,selectedPlayer);
								IcariteStars.setValue(0,selectedPlayer);
								EuphoriaStars.setValue(5,selectedPlayer);
								SubterranStars.setValue(3,selectedPlayer);
								break;
							case SDLK_u:
								WasteLanderStars.setValue(selectedPlayer,-1);
								IcariteStars.setValue(selectedPlayer,-1);
								EuphoriaStars.setValue(selectedPlayer,-1);
								SubterranStars.setValue(selectedPlayer,-1);

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

				for(int i = 0; i < 6; i++)
				{
					moralTrack[i].draw(gTM);
				}
				WasteLanderStars.draw(gTM);
				IcariteStars.draw(gTM);
				EuphoriaStars.draw(gTM);
				SubterranStars.draw(gTM);

				if(gMouseState == 1)
				{
					SDL_Rect destination;
					SDL_Rect source;
					SDL_Rect viewport = *aPane.getViewport();
	
					source.x = 0;
					source.y = 0;
					source.w = 14;
					source.h = 14;

					destination.x = gMousex;
					destination.y = gMousey;
					destination.w = source.w;
					destination.h = source.h;

					gTM->RenderTextureToViewport( 11, viewport, &destination, &source   );
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