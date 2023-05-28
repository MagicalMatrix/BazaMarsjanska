#pragma once
#include <stdexcept>
#include <iostream>
#include "Base.hpp"

//"manager" is kind of class
//that well manages some sort of objects
//and that's it
//every "base" in abstract thinking
//schould have exacly ONE manager
class BaseManager
{
	//HabitatDatabase* database;

	std::vector<Habitat> habitats;
	std::vector<Channel> channels;

public:
	Habitat GetHabitat(int Id)
	{
		if (Id > habitats.size())
		{
			throw std::out_of_range("index " + std::to_string(Id) + "is out of range");
		}
		return habitats[Id];
	}

	Channel GetChannel(int Id)
	{
		if (Id >= channels.size())
		{
			throw std::out_of_range("index " + std::to_string(Id) + "is out of range");
		}
		return channels[Id];
	}

	void AddHabitat(Habitat newHabitat)
	{
		habitats.push_back(newHabitat);
	}

	void AddChannel(Channel newChannel)
	{
		channels.push_back(newChannel);
	}

	//Time and cost calculation (else algorithm quality check)
	//-------------------------------------------------------
	
	//returns time required to made such task
	//throws exception if cannot made such task
private:
	double TryDoTask(int taskType)
	{

	}
};