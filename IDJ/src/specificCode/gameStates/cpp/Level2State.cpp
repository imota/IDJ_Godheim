/*
 * Level2State.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: Gabriel Naves
 */

#include "../include/Level2State.h"

/*
 * Level2State.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: Gabriel Naves
 */

#include "../include/Level2State.h"

Level2State::Level2State() : State(), tileSet(55,55,"img/level2/level2Tiles.png"),
                             tileMap("map/level2.txt", &tileSet),
                             movementMap("map/level2MovementMap.txt", tileSet)
{
    bg1.Open("img/level2/background.png");
    bg2.Open("img/level2/rocks.png");
    lavaCircle = new LavaCircle(680, 900);
    tmpBlackBg.Open("img/level2/blackBackground.png");
    EmplaceInitialObjects();
    Follow("Barrier");
    SelectMusic();
    music.Play(-1);
    ItemFlags::belt = true;
    ItemFlags::eagle = true;
    ItemFlags::fireBall = true;
    ItemFlags::hammer = true;
    changedMusic = false;
    end = false;

    StateData::CreateCheckPoint();
    StateData::startingLevel = 2;
    StateData::SaveCheckpoint("SavedData.txt");
}

Level2State::~Level2State()
{
    delete lavaCircle;
}

/**
 * Selects the musics of the level according to the soundtrack mode
 */
void Level2State::SelectMusic()
{
	if (!ItemFlags::redPotion)
	{
		if (StateData::soundMode == "Normal")
			music.Open("audio/SOUNDTRACK MODE/Fase 2/Fire Stage 1parte -Eber Filipe.mp3");
    	if (StateData::soundMode == "8bits")
    		music.Open("audio/8 bit MODE/LAVA STAGE parte 2 (HOTHOTPIMENTA) .mp3");
    	if (StateData::soundMode == "SNES")
    		music.Open("audio/Forever SNES MODE/Stage two (DONKEY KONG style) Parte 1.mp3");
	}
	else
	{
		if (StateData::soundMode == "Normal")
			music.Open("audio/SOUNDTRACK MODE/Fase 2/Fire Stage 2parte -Eber Filipe.mp3");
    	if (StateData::soundMode == "8bits")
    		music.Open("audio/8 bit MODE/LAVA2.mp3");
    	if (StateData::soundMode == "SNES")
    		music.Open("audio/Forever SNES MODE/Stage two (DK RUMBA) Parte 2.mp3");
	}
    music.Play(-1);
}

void Level2State::Update(float dt)
{
    Input();
    lavaCircle->Update(dt);
    CorrectStaticPointers();

    if (end)
    {
        SelectMusic();
        end = false;
    }

    if (!changedMusic and ItemFlags::redPotion)
    {
    	music.Stop();
    	changedMusic = true;
    	SelectMusic();
    }
    if (Thor::characterThor != NULL && Loki::characterLoki != NULL && Barrier::barrier != NULL)
    {
        UpdateArray(dt);
        ChecksForCollisions();
        ErasesDeadObjects();
    }
    if (StateData::haveDied)
    	EndGame(dt);
    if (Barrier::barrier == NULL) Camera::Unfollow();

    Camera::Update(dt);

    if (StageClear()) NextLevel();
}

void Level2State::EndGame(float dt)
{
//	Game::GetInstance().ResetWindowSize(360,275);
	Game::GetInstance().Push(new EndState());
	end = true;
}

bool Level2State::StageClear()
{
    if (Portal::endLevel2) return true;
	return (((Thor::characterThor) && (Thor::characterThor->box.Center().GetX() < 0) && (Thor::characterThor->box.Center().GetY() < 0)
			   && (Loki::characterLoki) && (Loki::characterLoki->box.Center().GetX() < 0) && (Loki::characterLoki->box.Center().GetY() < 0)) ? true : false);
}

void Level2State::NextLevel()
{
	requestDelete = true;
	StateData::checkpoint = false;
	StateData::UnlockSound(3);
	StateData::SaveCheckpoint("SavedData.txt");
    if (Portal::endLevel2)
    {
        Game::GetInstance().Push(new Credits());
        return;
    }
//	Game::GetInstance().ResetWindowSize(360,275);
	Game::GetInstance().Push(new EndState());
	end = true;
}

void Level2State::Render()
{
	if (!StateData::haveDied)
	{
		tmpBlackBg.Render(0,0);
		bg1.Render(-Camera::pos.GetX(), -Camera::pos.GetY());
		lavaCircle->Render();
		bg2.Render(-Camera::pos.GetX(), -Camera::pos.GetY());
		tileMap.RenderLayer(0, Camera::pos.GetX(), Camera::pos.GetY());
		RenderArray();
		if (Thor::characterThor != NULL) Thor::characterThor->RenderHP();
		if (Loki::characterLoki != NULL) Loki::characterLoki->RenderHP();
	}
}

void Level2State::EmplaceInitialObjects()
{
    objectArray.emplace_back(new Portal(22*55, 21*55));
    FloatingBlock *block = new FloatingBlock("img/level2/FloatingBlock3.png", 580, 300, M_PI, 100, 300, 3);
    objectArray.emplace_back(block);
    objectArray.emplace_back(new ChainedDragon(block, true, movementMap));
    block = new FloatingBlock("img/level2/FloatingBlock3.png", 880, 300, -M_PI/2, 100, 160, 3);
    objectArray.emplace_back(block);
    objectArray.emplace_back(new ChainedDragon(block, false, movementMap));
    block = new FloatingBlock("img/level2/FloatingBlock3.png", 880, 490, 0, 0, 0, 3);
    objectArray.emplace_back(block);
    objectArray.emplace_back(new ChainedDragon(block, false, movementMap));
    objectArray.emplace_back(new FloatingBlock("img/level2/FloatingBlock3.png", 450, 770, M_PI, 100, 250, 3));
    objectArray.emplace_back(new FloatingBlock("img/level2/FloatingBlock1.png", 795, 1022, -M_PI/2, 100, 250, 3));
    objectArray.emplace_back(new FloatingBlock("img/level2/FloatingBlock3.png", 1*55, 20*55,3*M_PI/2, 100, 150, 2.5));
    objectArray.emplace_back(new FloatingBlock("img/level2/FloatingBlock2.png", 282, 1267, -M_PI/2, 100, 150, 3));
    objectArray.emplace_back(new FloatingBlock("img/level2/FloatingBlock3.png", 397, 590, 0, 0, 0, 4));
    objectArray.emplace_back(new Loki(1175,200, movementMap));
    objectArray.emplace_back(new Thor(1225,200, movementMap));
    objectArray.emplace_back(new Skeleton(22*55,21.1*55));
    objectArray.emplace_back(new Skeleton(20*55,21.1*55));
    objectArray.emplace_back(new Skeleton(18*55,21.1*55));
    objectArray.emplace_back(new Skeleton(15*55,21.1*55));
    objectArray.emplace_back(new Skeleton(14*55,21.1*55));
    objectArray.emplace_back(new Barrier());
    objectArray.emplace_back(new Meteor(0,0,11*55,29*55));
    objectArray.emplace_back(new Lava(0, 1430));
    objectArray.emplace_back(new Item(20,7.5*55,"img/items/redPotion.png","redPotion",true,false));
}

void Level2State::Input()
{
    InputManager &im = InputManager::GetInstance();
    if (im.ShouldQuit()) requestQuit = true;
    if (InputManager::GetInstance().KeyPress(SDLK_1))
    {
    	music.Stop();
    	StateData::soundMode = "Normal";
    	SelectMusic();
    	music.Play(-1);
    }
    if (InputManager::GetInstance().KeyPress(SDLK_2))
    {
    	music.Stop();
    	StateData::soundMode = "8bits";
    	SelectMusic();
    	music.Play(-1);
    }
    if (InputManager::GetInstance().KeyPress(SDLK_3))
    {
    	music.Stop();
    	StateData::soundMode = "SNES";
    	SelectMusic();
    	music.Play(-1);
    }
}

void Level2State::Follow(std::string object)
{
    for (int i = 0; i < (int)objectArray.size(); i++)
        if (objectArray[i]->Is(object))
            Camera::Follow(&(*objectArray[i]), true, 0, 0,
                    tileMap.GetWidth()*tileSet.GetTileWidth() - Game::GetInstance().GetWindowWidth(),
                    tileMap.GetHeight()*tileSet.GetTileHeight() - Game::GetInstance().GetWindowHeight());
}

void Level2State::ChecksForCollisions()
{
    for(unsigned int i = 0; i<objectArray.size()-1; i++)
    {
        for (unsigned int j=i+1;j<objectArray.size();j++)
        {
            if (Collision::IsColliding(objectArray[j]->box, objectArray[i]->box,
                                       objectArray[j]->rotation*2*M_PI/360, objectArray[i]->rotation*2*M_PI/360))
            {
               objectArray[j]->NotifyCollision(*objectArray[i]);
               objectArray[i]->NotifyCollision(*objectArray[j]);
            }
        }
    }
}

void Level2State::ErasesDeadObjects()
{
    for (unsigned int i = 0; i < objectArray.size(); i++)
        if (objectArray[i]->IsDead())
            objectArray.erase(objectArray.begin()+i), i--;
}

void Level2State::CorrectStaticPointers()
{
	for (auto it = objectArray.begin(); it != objectArray.end(); it++) //Prevents wrong null pointers
	{
		if(it->get()->Is("Thor"))
			Thor::characterThor = (Thor*) it->get();
		if(it->get()->Is("Loki"))
			Loki::characterLoki = (Loki*) it->get();
		if(it->get()->Is("Barrier"))
			Barrier::barrier = (Barrier*) it->get();
	}
}

