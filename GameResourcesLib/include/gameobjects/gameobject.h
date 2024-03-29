#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <list>
#include <algorithm>
#include "property.h"
#include "basicobject.h"
#include "transform.h"

struct GameObject
{
public:
	typedef property_map<GameObject>::value PropertyMap;
	typedef std::map<std::string, std::shared_ptr<GameObject> > GameObjectMap;
	typedef std::map<std::string, std::shared_ptr<BasicObject> > ModelMap;

	GameObject();
	GameObject(const std::string& name);
	virtual ~GameObject();

	virtual void setActive(bool val);
	bool isActive() const;
	virtual void setPropertiesFlag(bool val);

	void setName(const std::string& name);
	std::string getName()const;
	Transform& getTransform();

	glm::mat4 getParentTransform()const;
	void setParentTransform(const glm::mat4& parentTransform);

	template<class T>
	void addProperty(const std::string& name);
	std::list<std::string> getPropertiesNames() const;
	bool removeProperty(const std::string& name);
	std::shared_ptr<Property<GameObject> > getProperty(const std::string& name);

	void addChild(std::shared_ptr<GameObject> child);
	void removeChild(std::string name);
	std::list<std::string> getChildrenNames() const;
	std::shared_ptr<GameObject> getChild(std::string name);

	bool hasParent()const;
	GameObject& getParent();

	void addModel(BasicObjectPtr model);
	void removeModel(std::string name);
	std::list<std::string> getModelsNames() const;
	BasicObjectPtr getModel(std::string name);

	virtual bool isUsable()const;

	virtual void init();
	virtual void process();
	virtual void invalidate();

	void deepCopy(const GameObject& object);

protected:
	GameObject* parent;
	Transform transform;
	glm::mat4 parentTransform;
	bool active;
	std::string name;
	bool processPropertiesFlag;

	PropertyMap properties;

	GameObjectMap children;

	ModelMap models;

	bool isChildNameUnique(std::string name);
	bool isModelNameUnique(std::string name);
};

template<class T>
void GameObject::addProperty(const std::string& name)
{
	//if class T inherits from Property<GameObject> then add an object of this class to GameObject
	register_property<GameObject, T>::add_to_map::add(properties, name, *this);
}

struct NullGameObject : public GameObject
{
	bool isUsable()const override
	{
		return false;
	}
};

struct NullGameObjectProperty : public Property<GameObject>
{
	NullGameObjectProperty(GameObject& obj)
		: Property<GameObject>("", obj)
	{}

	void init()override {}
	void process()override {}
	void invalidate()override {}
	bool isUsable()const override
	{
		return false;
	}
};

typedef std::shared_ptr<GameObject> GameObjectPtr;

#endif GAMEOBJECT_H
