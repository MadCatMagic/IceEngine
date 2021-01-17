#include "Behaviour.h"
#include "Entity.h"

Behaviour::Behaviour()
{
	//behaviourList.push_back(this);
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

void Behaviour::Update()
{
}

void Behaviour::UpdateBehaviours()
{
	for (Behaviour* obj : behaviourList)
	{
		obj->Update();
	}
}

void Behaviour::AddBehaviour(Behaviour* behaviour)
{
	behaviourList.push_back(behaviour);
}

std::vector<Behaviour*> Behaviour::behaviourList = std::vector<Behaviour*>();