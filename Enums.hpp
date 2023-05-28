#pragma once
#include <stdexcept>
#include <vector>
#include <string>

class Enum
{
	//id is just by position within vector
	std::vector<std::string> label;

private:
	bool CompareLabels(std::string org, std::string other)
	{
		unsigned int orgSize = org.size();
		if (orgSize != other.size())
		{
			return false;
		}
		for (unsigned int i = 0; i < orgSize; ++i)
		{
			if (org[i] != other[i])
			{
				return false;
			}
		}
		return true;
	}

public:
	unsigned int GetSize()
	{
		return label.size();
	}

	unsigned int GetId(std::string name)
	{
		unsigned int labelSize = label.size();
		for (unsigned int i = 0; i < labelSize; ++i)
		{
			if (CompareLabels(label[i], name))
			{
				return i;
			}
		}
		throw std::out_of_range("label " + name + " does not exists within current enum");
	}
};

//here defines global enums for usage within classes
Enum HabitatType;
Enum ChannelType;