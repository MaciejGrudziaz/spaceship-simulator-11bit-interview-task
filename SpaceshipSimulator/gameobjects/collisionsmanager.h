#pragma once

#include <GameResources/gameobjects/gameobject.h>
#include "../gamelogic/collisiondetectionalgorithm.h"
#include "../gamelogic/collisions.h"
#include "../gamelogic/laserbeamhitdetection.h"
#include <thread>
#include <mutex>

class CollisionsManager
{
public:
	CollisionsManager();

	void invalidate();

	void run();

	void registerCollisionObject(GameObjectPtr object);
	void registerHitDetectionObject(GameObjectPtr object);

private:
	int warmupCounter;
	const int warmupLimitCount = 10;
	std::vector<std::shared_ptr<Property<GameObject> > > collisionObjects;
	std::vector<std::shared_ptr<Property<GameObject> > > hitDetectionObjects;

	std::thread collisionDetectionThread;
	bool runFlag;

	void processCollisions();

	void checkHitboxCollisions();
	void checkHitDetection();
};

typedef std::shared_ptr<CollisionsManager> CollisionsManagerPtr;
