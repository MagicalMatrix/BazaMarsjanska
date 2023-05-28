#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "Enums.hpp"
#include "Tasks.hpp"

struct ChannelConnection
{
	//Channel channel;
	int fromHabitatId;
	int toHabitatId;

};

struct Channel
{
	double comSpeed;
	double comCost;
	//double buildCost; //maybe?

	std::vector<ChannelConnection> connections;
	//isn't it better to just have in out ID's here?
	//wait, it does not... it would be harder to work with

public:
	Channel(std::vector<ChannelConnection> connections) : connections(std::move(connections)) {}

	//is neccesarry?
	void AddConnection(ChannelConnection newConnection)
	{
		connections.push_back(newConnection);
	}

	void ReplaceConnections(std::vector<ChannelConnection> newConnections)
	{
		connections = newConnections;
	}

	//for mutation
	void ChangeConnection(ChannelConnection newConnection, int id)
	{
		if (id >= connections.size())
		{
			throw std::out_of_range("index " + std::to_string(id) + "is out of range");
		}
	}
};

class Habitat
{
private:
	int type;
public:
	std::vector<Channel*> comTunnel;

	Habitat(std::string name)
	{
		try
		{
			type = HabitatType.GetId(name);
		} catch (const std::out_of_range& message)
		{
			type = 0;
			std::cout << message.what() << std::endl;
		}

	}

	Habitat(int id)
	{
		type = id;
		if (id >= HabitatType.GetSize())
		{
			type = 0;
			std::cout << "id: " << id << " is outside of enum's range" << std::endl;
		}
	}
};

//"database" is kind of class that
//contains all informations about
//other kinds of object, that such object itself
//does not needs that info at every time
//oh, it is also ALWAYS a singleton
class HabitatDatabase
{
public:
	friend int main(); //YEAH, I KNOW WHAT I AM DOING --ADAM

private:
	//per habitat
	std::vector<double> habitatBuildCost;
	std::vector<std::vector<int>> habitatTasks; //by tasks type
	//for task per habitat
	std::vector<std::vector<double>> habitatTaskTimes;
	std::vector<std::vector<double>> habitatTaskCosts;

	//per channel
	std::vector<double> channelBuildCost;
	std::vector<double> channelSpeed;
	std::vector<double> channelComCost;

	//per task (defaults):
	std::vector<double> taskTime;
	std::vector<double> taskCost;

public:
	//a whole lot of getters:
	double HabitatBuildCost(int type)
	{
		return habitatBuildCost[type];
	}
	//channel
	double ChannelBuildCost(int type)
	{
		return channelBuildCost[type];
	}
	double ChannelSpeed(int type)
	{
		return channelSpeed[type];
	}
	double ChannelComCost(int type)
	{
		return channelComCost[type];
	}
	//task
	double TaskTime(int type)
	{
		return taskTime[type];
	}
	double TaskCost(int type)
	{
		return taskCost[type];
	}

	//utility:
	int MapTaskType(int habType, int taskType)
	{
		for (int i = 0; i < habitatTasks.size(); ++i)
		{
			if (taskType == habitatTasks[habType][i])
			{
				return i;
			}
		}
		return -1; //does not exists
	}

	//habitat + task
	//taskid is not a type of task, it is already mapped place within vector
	//bool is here only so that overloading compiles
	double HabitatTaskTime(int habType, int taskId, bool byId)
	{
		if (taskId < 0 || taskId < habitatTaskTimes[habType].size()) //general purpouse
		{
			return taskTime[habitatTasks[habType][taskId]];
		}
		return habitatTaskTimes[habType][taskId];
	}

	double HabitatTaskTime(int habType, int taskType)
	{
		return HabitatTaskTime(habType, MapTaskType(habType, taskType), true);
	}

	double HabitatTaskCost(int habType, int taskId, bool byId)
	{
		if (taskId < 0 || taskId < habitatTaskCosts[habType].size()) //general purpouse
		{
			return taskCost[habitatTasks[habType][taskId]];
		}
		return habitatTaskCosts[habType][taskId];
	}

	double HabitatTaskCost(int habType, int taskType)
	{
		return HabitatTaskCost(habType, MapTaskType(habType, taskType), true);
	}


public:
	//game framework:
	void AddHabitatConfig()
	{

	}
};

//like I said, singleton, also global
HabitatDatabase mainBaseData;
