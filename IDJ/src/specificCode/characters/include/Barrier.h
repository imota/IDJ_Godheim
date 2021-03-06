/*
 * Barrier.h
 *
 *  Created on: May 22, 2014
 *      Author: Icaro Mota
 *              Gabriel Naves
 *
 * Description: An object that keeps Thor and Loki from spliting up too much.
 *
 */

#ifndef BARRIER_H_
#define BARRIER_H_

#include "../../../engine/include/GameObject.h"
#include "Thor.h"
#include "Loki.h"

class Barrier : public GameObject
{
  public:
	Barrier();
	virtual ~Barrier();

	void Update(float dt);
	void Render();
	void NotifyCollision(GameObject& other);
	bool IsDead();
	bool Is(std::string type);
	void CheckCollision(Character* character);

	static Barrier* barrier;

	Point GetCorner();
	const float DIAMETER = 250;
	const float DRAG_TOLERANCE = 0;
  private:
	Point FindCenter();
	Sprite sp;
};

#endif /* BARRIER_H_ */
