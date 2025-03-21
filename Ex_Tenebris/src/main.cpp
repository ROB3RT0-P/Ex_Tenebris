/*******************************************
	Written by Robert Parry [RJP] - 2024
*******************************************/

/*********************************************************************************
	Copyright (c) Robert Parry - 2024

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*********************************************************************************/

/*********************************************************************************
	RJP - TODO: 
		Priority:
				- 

		Non-Priority:
				-
*********************************************************************************/

#include "global.h"
#include "game.h"
#include "processmanager.h"
#include "entitymanager.h"
#include "resourcemanager.h"
#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <string>

namespace global
{

namespace // hidden namespace
{
	Game* GameInstance;
	ProcessManager* ProcessManagerInstance;
	EntityManager* EntityManagerInstance;
	ResourceManager* ResourceManagerInstance;

	ResourceManager::ResourceDBEntry resourceDatabase[] =
	{
		{ raw_enum(Res::Default),			"textures/texture.png",			ResourceManager::ResourceType::Texture },
		{ raw_enum(Res::PlayerSprite),		"textures/texture.png",			ResourceManager::ResourceType::Texture },
		{ raw_enum(Res::EnemySprite),		"textures/texture.png",			ResourceManager::ResourceType::Texture },
		{ raw_enum(Res::MainMenuSprite),	"textures/main_menu.png",		ResourceManager::ResourceType::Texture },
		{ raw_enum(Res::BackgroundSprite),	"textures/pathway.png",			ResourceManager::ResourceType::Texture },
		{ raw_enum(Res::GameOverSprite),	"textures/game_over.png",		ResourceManager::ResourceType::Texture },
		{ raw_enum(Res::ContinueSprite),	"textures/continue.png",		ResourceManager::ResourceType::Texture },
		{ raw_enum(Res::DeadEndSprite),		"textures/dead_end.png",		ResourceManager::ResourceType::Texture },
		{ raw_enum(Res::ContinueSprite),	"textures/continue.png",		ResourceManager::ResourceType::Texture },
		{ raw_enum(Res::EscapeSprite),		"textures/escape.png",			ResourceManager::ResourceType::Texture },
	};
}
	Game* game() { return GameInstance; }
	ProcessManager* processManager()	{ return ProcessManagerInstance; }
	EntityManager* entityManager()		{ return EntityManagerInstance; }
	ResourceManager* resourceManager()	{ return ResourceManagerInstance; }
}

int main( int argc, char* args[] )
{
	global::GameInstance = new Game();
	global::EntityManagerInstance = new EntityManager();
	global::ProcessManagerInstance = new ProcessManager();
	global::ResourceManagerInstance = new ResourceManager();
	
	global::resourceManager()->initializeResourceDatabase(global::resourceDatabase, sizeof(global::resourceDatabase) / 
		sizeof(global::resourceDatabase[0]), raw_enum(global::Res::Max), "./data/");

	Uint32 prevTicks = 0;
	Uint32 currentTicks = 0;
	float deltaTime = 0.0f;

	if( !global::game()->initialize(1280, 720))
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		{	
			bool quit = false;
			prevTicks = SDL_GetTicks();

			while( !quit )
			{
				currentTicks = SDL_GetTicks();
				deltaTime = (currentTicks - prevTicks) / 1000.f;
				prevTicks = currentTicks;

				global::game()->tickLogic(deltaTime);

				global::game()->renderAndPresent();

				global::game()->postFrameUpdate();

				quit = global::game()->handleEvents(deltaTime);
			}
		}
	}

	global::game()->close();

	return 0;
}
