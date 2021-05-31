#include "Engine/Behaviour.h"
#include "Engine/Entity.h"
#include "Engine/Camera.h"

Behaviour::Behaviour()
{
	behaviourList.push_back(this);
}

Behaviour::Behaviour(const Behaviour& obj)
{
	this->entity = obj.entity;
	this->transform = obj.transform;
}

Behaviour::Behaviour(Behaviour&& obj) noexcept
{
	this->entity = obj.entity;
	obj.entity = nullptr;
	this->transform = obj.transform;
	obj.transform = nullptr;
}

Behaviour::~Behaviour()
{
	std::vector<Behaviour*>::iterator index = std::find(behaviourList.begin(), behaviourList.end(), this);
	behaviourList.erase(index);
}

Behaviour& Behaviour::operator=(const Behaviour& other)
{
	this->entity = other.entity;
	this->transform = other.transform;
	return *this;
}

Behaviour& Behaviour::operator=(Behaviour&& other) noexcept
{
	this->entity = other.entity;
	other.entity = nullptr;
	this->transform = other.transform;
	other.transform = nullptr;
	return *this;
}

void Behaviour::AwakeBehaviours()
{
	for (Behaviour* obj : behaviourList)
		obj->Awake();
}

void Behaviour::StartBehaviours()
{
	for (Behaviour* obj : behaviourList)
		obj->Start();
}

void Behaviour::UpdateBehaviours()
{
	for (Behaviour* obj : behaviourList)
		obj->Update();
}

void Behaviour::ReleaseBehaviours()
{
	for (auto behaviour : behaviourList)
		delete behaviour;
}

std::vector<Behaviour*> Behaviour::behaviourList = std::vector<Behaviour*>();