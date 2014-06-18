/*
 * Thor.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Gabriel Naves
 *              Icaro Mota
 */

#include "../include/Thor.h"

Thor* Thor::characterThor;

Thor::Thor(float x, float y, MovementMap& movMap) :
        Character(movMap,"img/characters/thor_walk.png",7,0.1,"img/characters/thor_jump.png",4,0.1,"img/characters/thorStairs.png",2,0.15)
{
    characterSp.Open("img/characters/thor.png");
    box.Set(x-characterSp.GetWidth()/2, y-characterSp.GetHeight()/2, characterSp.GetWidth(), characterSp.GetHeight());
    characterThor = this;
}

Thor::~Thor()
{
    characterThor = NULL;
}

void Thor::Input()
{
    horizontal = vertical = 0;
    InputManager &input = InputManager::GetInstance();
    //sets the directions in which the camera must move

    if (input.KeyPress(SDLK_l) || input.IsKeyDown(SDLK_l))
        horizontal += 1;
    if (input.KeyPress(SDLK_j) || input.IsKeyDown(SDLK_j))
        horizontal -= 1;
    if (input.KeyPress(SDLK_i))
        vertical += 1;
    if (input.IsKeyDown(SDLK_i))
        vertical = 2;
    if (input.KeyPress(SDLK_k) or input.IsKeyDown(SDLK_k))
        vertical -= 1;
    if (input.KeyPress(SDLK_u))
        actionButton = true;
}

void Thor::ReleasesStairs()
{
    SetActionState(NONE);
    SetVState(FALLING);
}

void Thor::HoldStairs()
{
    SetActionState(CLIMBING);
    box.SetPoint(box.GetPoint().GetX(),box.GetPoint().GetY()+1);
}

/**
 * Calls the right action Thor must do, depending on the situation
 */
void Thor::Act()
{
    if (actionState == CLIMBING)
        ReleasesStairs();
    else if (canHoldStairs)
        HoldStairs();
    actionButton = false;
}

void Thor::Hit()
{

}

void Thor::UpdateVerticalState()
{
	// If character center is below 8/9 of the barrier, then the character is suspended.
	if ( ((Barrier::barrier->GetCorner().GetY()+(Barrier::barrier->DIAMETER*9/10)) < box.Center().GetY())
			&& (box.Center().Distance(Loki::characterLoki->box.Center()) >= Barrier::barrier->DIAMETER - Barrier::barrier->DRAG_TOLERANCE))
		barrierSuspended = true;
	else
		barrierSuspended = false;


    if (vertical == 1 and vState == STANDING)
        SetVState(JUST_JUMPED);
}

void Thor::Move(float dt)
{
    if (actionState == CLIMBING)
        Climb(dt);
    else
        UpdateSpeed(dt);
    box.MoveRect(speed.GetX()*dt,speed.GetY()*dt);
    Barrier::barrier->CheckCollision(this);
}

void Thor::UpdatesStateOnTheFall()
{
    if (vState == JUST_JUMPED) SetVState(JUMPING);
    if (speed.GetY() > 0) SetVState(FALLING);
}

void Thor::Update(float dt)
{
    Input();
    if (actionButton) Act();
    UpdateState();
    if (actionState == CLIMBING and !canHoldStairs) SetActionState(NONE);
    Move(dt);
    UpdatesStateOnTheFall();
    UpdateSprite(dt);
    CheckMovementLimits();
    UpdatePrevState();
}

void Thor::NotifyCollision(GameObject& other)
{
    if (other.Is("Stairs"))
    {
        //if the character is close enough in the y axis to go down the stairs
        if (box.GetY()<=(other.box.GetY()-box.GetH()+50) and box.GetY()>=(other.box.GetY()-box.GetH()-2) and actionState != CLIMBING)
        {
            box.SetPoint(box.GetX(),other.box.GetPoint().GetY()-box.GetH()); //corrects bugs
            SetVState(STANDING);
        }
    }
    if (other.Is("Spikes"))
        hp -= HP/20;
}

bool Thor::Is(std::string type)
{
    return type == "Thor" ? true : false;
}

/**
 * Updates the sprite based on the state of the character
 */
void Thor::UpdateSprite(float dt)
{
    if (vState == STANDING)
    {
        if (hState == MOVING_RIGHT or hState == MOVING_LEFT)
            prevHState != hState ? ChangeSp("walkSp","img/characters/thor_walk.png",7) : walkSp.Update(dt);
        else if (hState == STANDING_RIGHT or hState == STANDING_LEFT)
            ChangeSp("characterSp","img/characters/thor.png",1);
    }
    else if (actionState == CLIMBING)
    {
        if (prevActionState != CLIMBING)
            ChangeSp("climbSp","img/characters/lokiStairs.png",2);
        else if (vertical != 0)
            climbSp.Update(dt);
    }
    else if (vState == FALLING)
                ChangeSp("jumpSp","img/characters/thor_jump.png",4,4,false);
            else if (vState == JUMPING)
            {
                if (prevVState == JUST_JUMPED)
                    ChangeSp("jumpSp","img/characters/thor_jump.png",4,1,false);
                else
                    jumpSp.Update(dt);
            }
}
