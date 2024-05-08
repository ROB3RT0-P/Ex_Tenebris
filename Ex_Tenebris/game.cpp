/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "game.h"

bool Game::initialize(int ScreenWidth, int ScreenHeight)
{
	bool success = true;
	SCREEN_WIDTH_ = ScreenWidth;
	SCREEN_HEIGHT_ = ScreenHeight;

	// RJP - Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// RJP - Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		// RJP - Create window
		gWindow = SDL_CreateWindow("Ex Tenebris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH_, SCREEN_HEIGHT_, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// RJP - Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// RJP - Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// RJP - Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				global::resourceManager()->initializeLoaderData(gRenderer);
			}
		}
	}
	return success && loadInitialResources();
}

bool Game::loadInitialResources()
{
#ifdef _DEBUG
	debug = new Debug();
	debug->startFPSCounter();
#endif

	fScreenWidth_ = static_cast<float>(SCREEN_WIDTH_);
	fScreenHeight_ = static_cast<float>(SCREEN_HEIGHT_);
	fConsoleRenderPosX_ = fScreenWidth_ * 0.1f;
	fConsoleRenderPosY_ = fScreenHeight_ * 0.9f;
	bMusicPlaying_ = false;
	bStateSwitch_ = false;
	uCurrentTime_ = 0;
	uInitialTime_ = 0;
	uStateTimer_ = 3000;
	debugTextSize_ = 20;
	textSize_ = 60;
	textColor_ = { 255, 255, 255, 255 };
	debugTextColor_ = { 0, 255, 0, 255 };

	loadingProcess.callback = this;
	LoadingProcess::LoadRequest toLoad;
	
	toLoad.resource = global::Res::Default;
	loadingProcess.dataToLoad.push_back(toLoad);
	toLoad.resource = global::Res::PlayerSprite;
	loadingProcess.dataToLoad.push_back(toLoad);
	toLoad.resource = global::Res::MainMenuSprite;
	loadingProcess.dataToLoad.push_back(toLoad);
	toLoad.resource = global::Res::BackgroundSprite;
	loadingProcess.dataToLoad.push_back(toLoad);
	toLoad.resource = global::Res::GameOverSprite;
	loadingProcess.dataToLoad.push_back(toLoad);
	toLoad.resource = global::Res::DeadEndSprite;
	loadingProcess.dataToLoad.push_back(toLoad);
	toLoad.resource = global::Res::ContinueSprite;
	loadingProcess.dataToLoad.push_back(toLoad);

	global::processManager()->registerProcess(&loadingProcess, raw_enum(global::TaskID::Loading), raw_enum(global::TickOrder::DontCare), raw_enum(global::RenderOrder::DontCare));

	// RJP - Entity and Process Manager
	playerTexture_ = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::PlayerSprite));
	mainMenuTexture_ = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::MainMenuSprite));
	backgroundTexture_ = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::BackgroundSprite));
	gameOverTexture_ = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::GameOverSprite));
	deadEndTexture_ = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::DeadEndSprite));
	continueTexture_ = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::ContinueSprite));
	escapeTexture_ = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::EscapeSprite));

	playerEntity->registerWithEntityManager();	
	playerEntity = static_cast<Player*>(global::entityManager()->createEntity(raw_enum(global::EntityType::Player)));

	// RJP - Temporary texture loading - This is to be moved over to the Resource Manager.
	SDL_Surface* surface = IMG_Load("Data/textures/skulls_BG.png");
	mainMenuTexture_ = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);
	
	playerEntity = new Player();
	playerEntity->init();

	debugText = new Text(gRenderer, "Data/kenney/Fonts/kenneyPixel.ttf", debugTextSize_, debugTextColor_);
	gameText = new Text(gRenderer, "Data/kenney/Fonts/kenneyPixel.ttf", textSize_, textColor_);
	consoleText = new Text(gRenderer, "Data/kenney/Fonts/kenneyPixel.ttf", textSize_, textColor_);
	stateMachine = new StateMachine();
	audio = new AudioPlayer();
	textureRenderer = new Renderer(gRenderer);
	console = new Console(*stateMachine, *playerEntity);

	return true;
}

void Game::start(const Info& info) 
{
	stateMachine->setState( GameState::MENU );

#ifdef _DEBUG
	//stateMachine->setState(GameState::ESCAPE);
#endif
}

void Game::tickLogic( float deltaTime ) 
{
	global::processManager()->tickProcesses();
	
	switch ( stateMachine->getState() )
	{
	case GameState::DEBUG:
#ifdef _DEBUG
		debug->updateFPSCounter();
#endif
		break;

		case GameState::MENU:
			if (!bMusicPlaying_)
			{
				audio->play("Data/music/Miserere_mei.mp3");
				bMusicPlaying_ = true;
			}
			break;

		case GameState::PLAY:
			bStateSwitch_ = false;
			if (!bMusicPlaying_)
			{
				audio->play("Data/music/SCP-x2x.mp3");
				bMusicPlaying_ = true;
			}

			break;

		case GameState::PAUSE:

			break;

		case GameState::GAMEOVER:
			if (!bMusicPlaying_)
			{
				audio->play("Data/music/Gathering_Darkness.mp3");
				bMusicPlaying_ = true;
			}
			break;

		case GameState::DEADEND:
			if (!bStateSwitch_)
			{
				uInitialTime_ = SDL_GetTicks();
				bStateSwitch_ = true;
			}
			uCurrentTime_ = SDL_GetTicks();

			if ((uCurrentTime_ - uInitialTime_) > uStateTimer_)
			{
				stateMachine->setState(GameState::PLAY);
			}

			break;

		case GameState::CONTINUE:
			if (!bStateSwitch_)
			{
				uInitialTime_ = SDL_GetTicks();
				bStateSwitch_ = true;
			}
			uCurrentTime_ = SDL_GetTicks();

			if ((uCurrentTime_ - uInitialTime_) > uStateTimer_)
			{
				stateMachine->setState(GameState::PLAY);
			}

			break;

		case GameState::ESCAPE:
			if (!bMusicPlaying_)
			{
				audio->play("Data/music/Night_of_Chaos.mp3");
				bMusicPlaying_ = true;
			}

			break;

		}
}

void Game::renderAndPresent()
{	
	SDL_RenderClear(gRenderer);
	global::processManager()->renderProcesses();
	SDL_RenderPresent(gRenderer);
}

void Game::render(const Info& info)
{
	switch (stateMachine->getState())
	{
	case GameState::MENU:
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		textureRenderer->textureRenderScaledToScreen(mainMenuTexture_, 0, 0, fScreenWidth_, fScreenHeight_);
		gameText->RenderConsoleText("EX TENEBRIS", static_cast<int>(fScreenWidth_ * 0.41f), static_cast<int>(fScreenHeight_ * 0.05f));
		gameText->RenderPhasingText("Retributio ab intus...", static_cast<int>(fScreenWidth_ * 0.35f), static_cast<int>(fScreenHeight_ * 0.9f), SDL_GetTicks(), 2000);

#ifdef _DEBUG
		debugText->RenderDebugText("MAIN_MENU", 10, 10);
#endif
		break;

#ifdef _DEBUG
	case GameState::DEBUG:
		debugText->RenderDebugText("DEBUG", 10, 10);
		debugText->RenderFlashingText("Flashing Text", 100, 100, SDL_GetTicks(), 1000);
		debugText->RenderPhasingText("Phasing Text", 100, 200, SDL_GetTicks(), 2000);
		debugText->RenderDebugText("FPS: " + std::to_string(debug->getMaxFPS()), SCREEN_WIDTH_ - 100, 100);
		debugText->RenderDebugText("Player: " + std::to_string(playerEntity->getEntityID()), 100, 400);
		break;
#endif

	case GameState::PLAY:
	
#ifdef _DEBUG
		debugText->RenderDebugText("PLAY", 10, 10);
#endif

		break;

	case GameState::PAUSE:
	
#ifdef _DEBUG
		debugText->RenderConsoleText("PAUSE_MENU", 10, 10);
#endif
		break;

	case GameState::GAMEOVER:
#ifdef _DEBUG
		debugText->RenderConsoleText("GAME_OVER", 10, 10);
#endif
		break;

	case GameState::DEADEND:
#ifdef _DEBUG
		debugText->RenderDebugText("DEAD_END", 10, 10);
#endif
		break;

	case GameState::CONTINUE:
#ifdef _DEBUG
		debugText->RenderDebugText("CONTINUE", 10, 10);
#endif
		break;

	case GameState::INTRO:
#ifdef _DEBUG
		debugText->RenderDebugText("INTRO", 10, 10);
#endif
		break;
	
	case GameState::ESCAPE:
#ifdef _DEBUG
		debugText->RenderDebugText("ESCAPE", 10, 10);
#endif
		break;
	}
}

void Game::postFrameUpdate()
{
	global::processManager()->endOfFrameCleanup();
}

bool Game::handleEvents(float deltaTime)
{
	cUserInput_ = '\0';

	switch (stateMachine->getState())
	{
		case GameState::MENU:
			while (SDL_PollEvent(&event_) != 0)
			{
				cUserInput_ = controls::handleInput(event_);
				if (cUserInput_ == '\a' && stateMachine->getState() == GameState::MENU)
				{
					stateMachine->setState(GameState::DEBUG);
					audio->stop();
					bMusicPlaying_ = false;
				}
			}


			if (console->manageInput(cUserInput_)) return 1;

			break;

		case GameState::DEBUG:
			while (SDL_PollEvent(&event_) != 0)
			{
				cUserInput_ = controls::handleInput(event_);
			}
			if (console->manageInput(cUserInput_)) return 1;

			break;

		case GameState::PLAY:

			while (SDL_PollEvent(&event_) != 0)
			{
				cUserInput_ = controls::handleInput(event_);
				if (cUserInput_ == '\a' && stateMachine->getState() == GameState::PLAY) stateMachine->setState(GameState::PAUSE);
				if (cUserInput_ != '\0') break;
			}

			if (console->manageInput(cUserInput_)) return 1;

			if (console->checkPlayerState())
			{
				audio->stop();
				bMusicPlaying_ = false;
			}

#ifdef _DEBUG
			if (console->getPrevConsoleOutput() == "die")
			{
				audio->stop();
				bMusicPlaying_ = false;
			}

			if (console->getPrevConsoleOutput() == "r")
			{
				stateMachine->setState(GameState::CONTINUE);
			}
		
			if (console->getPrevConsoleOutput() == "l")
			{
				stateMachine->setState(GameState::DEADEND);
			}
#endif

			break;

		case GameState::PAUSE:

			while (SDL_PollEvent(&event_) != 0)
			{
				cUserInput_ = controls::handleInput(event_);
				if (cUserInput_ == '\a' && stateMachine->getState() == GameState::PAUSE) stateMachine->setState(GameState::PLAY);
			}

			if (console->manageInput(cUserInput_)) return 1;

			break;

		case GameState::GAMEOVER:

			while (SDL_PollEvent(&event_) != 0)
			{
				cUserInput_ = controls::handleInput(event_);
			}

			if (console->manageInput(cUserInput_)) return 1;

			break;

		case GameState::INTRO:

			break;

		case GameState::DEADEND:

			break;

		case GameState::CONTINUE:

			break;

		case GameState::ESCAPE:
	
			while (SDL_PollEvent(&event_) != 0)
			{
				cUserInput_ = controls::handleInput(event_);
			}
			if (console->manageInput(cUserInput_)) return 1;

			break;
		}
	return 0;
}

void Game::close()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	gWindow = nullptr;
	gRenderer = nullptr;

	font = nullptr;
	debugText = nullptr;
	playerTexture_ = nullptr;
	audio = nullptr;
	background = nullptr;
	titleBackground = nullptr;
	font = nullptr;
	debugText = nullptr;
	gameText = nullptr;
	consoleText = nullptr;
	playerTexture_ = nullptr;
	backgroundTexture_ = nullptr;
	mainMenuTexture_ = nullptr;
	gameOverTexture_ = nullptr;
	deadEndTexture_ = nullptr;
	continueTexture_ = nullptr;
	escapeTexture_ = nullptr;
	textureRenderer = nullptr;
	stateMachine = nullptr;
	console = nullptr;
	
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

void Game::onLoadComplete(LoadingProcess::LoadRequest* loadedResources, size_t count)
{
	gTexture = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::Default));
	global::processManager()->registerProcess(this, raw_enum(global::TaskID::DontCare), raw_enum(global::TickOrder::DontCare), raw_enum(global::RenderOrder::DontCare));
}
