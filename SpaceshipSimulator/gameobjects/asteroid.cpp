#include "asteroid.h"

Asteroid::Asteroid()
	: destroyedFlag(false)
	, health(0.0f)
	, asteroidCollisionAttachFlag(false)
{
	setName("asteroid");

	explosionParticleSystemData = std::make_shared<ParticleSystemData>();
	explosionParticleSystemData->init();

	explosionFragmentsParticleSystemData = std::make_shared<ParticleSystemData>();
	explosionFragmentsParticleSystemData->init();
}

void Asteroid::init()
{
	StandardGameObject::init();
}

void Asteroid::process()
{
	StandardGameObject::process();

	glm::vec3 pos = transform.getPosition();
	glm::vec3 rot = transform.getRotation();

	glm::vec3 worldSpeedVec(0.0f, -(*worldSpeed), 0.0f);
	pos += (linearSpeed + worldSpeedVec) * Time::deltaTime;
	rot += rotSpeed * Time::deltaTime;

	for (int i = 0; i < 3; ++i)
	{
		if (rot[i] > 360.0f) rot[i] = rot[i] - 360.0f;
		if (rot[i] < -360.0f) rot[i] = rot[i] + 360.0f;
	}

	transform.setPosition(pos);
	transform.setRotation(rot);

	if (destroyedFlag  && !explosionFragmentsParticleSystemData->launchFlag)
	{
		setActive(false);
		destroyedFlag = false; 
	}

	if (explosionParticleSystemData->launchFlag)
	{
		*explosionParticleSystemData->modelTransform = transform.getTransformMat();
		*explosionParticleSystemData->runTime += Time::deltaTime;
		if ((*explosionParticleSystemData->runTime) > explosionParticleSystemData->maxLifeTime)
		{
			explosionParticleSystemData->launchFlag = false;
			*explosionParticleSystemData->runTime = 0.0f;
			*explosionParticleSystemData->shutDownTime = 0.0f;
		}
	}
	if (explosionFragmentsParticleSystemData->launchFlag)
	{
		*explosionFragmentsParticleSystemData->modelTransform = transform.getTransformMat();
		*explosionFragmentsParticleSystemData->runTime += Time::deltaTime;
		if ((*explosionFragmentsParticleSystemData->runTime) > explosionFragmentsParticleSystemData->maxLifeTime)
		{
			explosionFragmentsParticleSystemData->launchFlag = false;
			*explosionFragmentsParticleSystemData->runTime = 0.0f;
			*explosionFragmentsParticleSystemData->shutDownTime = 0.0f;
		}
	}
}

void Asteroid::invalidate()
{
	StandardGameObject::invalidate();
}

void Asteroid::deepCopy(const Asteroid& asteroid)
{
	StandardGameObject::deepCopy(asteroid);

	this->worldSpeed = asteroid.worldSpeed;
	this->rotSpeed = asteroid.rotSpeed;
	this->linearSpeed = asteroid.linearSpeed;
	this->camera = asteroid.camera;
	this->projection = asteroid.projection;
}

void Asteroid::setActive(bool val)
{
	StandardGameObject::setActive(val);

	setHitboxActive(val);

	destroyedFlag = false;
}

void Asteroid::restart()
{
	setActive(false);
}

void Asteroid::setHitboxActive(bool val)
{
	GameObjectPtr hitbox = getChild("hitbox_main");
	if (hitbox->isUsable())
	{
		hitbox->setActive(val);
	}
}

void Asteroid::registerWorldSpeed(std::shared_ptr<float> speed)
{
	worldSpeed = speed;
}

void Asteroid::registerCamera(CameraPtr camera)
{
	this->camera = camera;
}

void Asteroid::registerProjectionMatrixPtr(ConstMat4Ptr projection)
{
	this->projection = projection;
}

void Asteroid::setRotSpeed(glm::vec3 rotSpeed)
{
	this->rotSpeed = rotSpeed;
}

glm::vec3 Asteroid::getLinearSpeed()const
{
	return linearSpeed;
}

void Asteroid::setLinearSpeed(glm::vec3 linSpeed)
{
	this->linearSpeed = linSpeed;
}

void Asteroid::dealDamage(float val)
{
	StandardGameObject::dealDamage(val);
	if (health > 0.0f)
	{
		health -= val;

		if (health <= 0.0f)
		{
			health = 0.0f;
			renderer->setActive(false);
			destroyedFlag = true;

			setHitboxActive(false);

			*explosionParticleSystemData->continuous = 0;
			*explosionParticleSystemData->runTime = 0.0f;
			*explosionParticleSystemData->shutDownTime = 0.0f;
			explosionParticleSystemData->launchFlag = true;

			*explosionFragmentsParticleSystemData->continuous = 0;
			*explosionFragmentsParticleSystemData->runTime = 0.0f;
			*explosionFragmentsParticleSystemData->shutDownTime = 0.0f;
			explosionFragmentsParticleSystemData->launchFlag = true;
		}
	}
}

void Asteroid::destroy()
{
	dealDamage(1.05*health);
}

void Asteroid::setHealth(float val)
{
	health = val;
}

float Asteroid::getHealth()const
{
	return health;
}

ParticleSystemDataPtr Asteroid::getExplosionParticleSystemData()const
{
	return explosionParticleSystemData;
}

ParticleSystemDataPtr Asteroid::getExplosionFragmentsParticleSystemData()const
{
	return explosionFragmentsParticleSystemData;
}

bool Asteroid::isParticleSystemRunning()const
{
	return (explosionFragmentsParticleSystemData->launchFlag);
}

bool Asteroid::isDestroyed()const
{
	return destroyedFlag;
}

void Asteroid::initParticleSystemsData(float explosionParticlesysMaxLifeTime, float explosionFragmentsParticlesysMaxLifeTime)
{
	*explosionParticleSystemData->baseColor = glm::vec3(1.0f, 0.55f, 0.1f);
	*explosionParticleSystemData->destColor = glm::vec3(1.0f, 0.55f, 0.1f);
	explosionParticleSystemData->maxLifeTime = explosionParticlesysMaxLifeTime;
	*explosionFragmentsParticleSystemData->baseColor = glm::vec3(0.8f, 0.8f, 0.8f);
	*explosionFragmentsParticleSystemData->destColor = glm::vec3(0.3f, 0.3f, 0.3f);
	explosionFragmentsParticleSystemData->maxLifeTime = explosionFragmentsParticlesysMaxLifeTime;
}
