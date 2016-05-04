#include "stdafx.h"
#include "SpaceGame.h"
#include "InitialisationError.h"
#include "Cell.h"
#include "Grid.h"
#include "MainCharacter.h"
#include "IdleState.h"


SpaceGame::SpaceGame()
	: notRoomCell("Resources\\cell_test.png"), 
	roomCell("Resources\\cell_test2.png"),
	characterTex("Resources\\char.png")
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw InitialisationError("SDL_Init failed");
	}

	window = SDL_CreateWindow("COMP150 Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		throw InitialisationError("SDL_CreateWindow failed");
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		throw InitialisationError("SDL_CreateRenderer failed");
	}
}


SpaceGame::~SpaceGame()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void SpaceGame::run()
{
	Grid room;
	room.makeGrid(WINDOW_WIDTH, WINDOW_HEIGHT);
	Map mapLoader;
	mapLoader.LoadMap("Resources\\Map\\test_map.txt", room);
	MainCharacter characterOne;
	characterOne.currentRoom = std::make_shared<Grid>(room);  //to get room state
	characterOne.state = std::make_shared<IdleState>();

	running = true;
	while (running)
	{
		// Handle events
		SDL_Event ev;
		if (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			default:
				break;
			}
		}//End pollevent if


		const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
		
		characterOne.state->update(characterOne, room, keyboardState);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		int cellSize = room.getCellSize();
		
		for (int x = 0; x < room.grid.size(); x++)
		{
			for (int y = 0; y < room.grid[x].size(); y++)
			{
				int xPos = x * cellSize + cellSize / 2;
				int yPos = y * cellSize + cellSize / 2;
				//Renders cell based on state
				if (room.grid[x][y]->isRoom)//Detects if the cell is a room
				{
					roomCell.render(renderer, xPos, yPos, cellSize, cellSize);
				}
				//If a cell isn't part of a room don't render
			} //End for Y loop
			
			
		}//End for X loop

		//Need to render character based on state 
		characterTex.render(renderer, characterOne.getX(), characterOne.getY(), characterOne.getSize(), characterOne.getSize());
		SDL_RenderPresent(renderer);
	}//End while running

}
