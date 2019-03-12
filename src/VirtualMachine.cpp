#include "VirtualMachine.h"

namespace Rosie
{
	VirtualMachine::VirtualMachine()	
	{}
	
	void VirtualMachine::execute(const std::vector<Variable>& constants, const std::vector<std::string>& commands)
	{
		std::map<int, Variable> variables;
		for(const std::string command : commands)
		{
			std::vector<int> tokens = split(command);//setk setv call print
			if(tokens[0] == 0)//set constant
			{
				variables[tokens[1]] = constants[tokens[2]];
			}
			else if(tokens[0] == 1)//set variable
			{
				variables[tokens[1]] = variables[tokens[2]];
			}
			else if(tokens[0] == 2)//call
			{}
			else if(tokens[0] == 3)
			{
				std::cout << variables[tokens[1]].f << std::endl;
			}
		}
	}
	
	std::vector<int> VirtualMachine::split(const std::string& line)
	{
		std::vector<int> res;
		std::size_t startPos = 0;
		std::size_t endPos = 0;
		while((endPos = line.find(" ", startPos)) != std::string::npos)
		{
			res.push_back(std::stoi(line.substr(startPos, endPos-startPos)));
			startPos = endPos+1;
		}
		res.push_back(std::stoi(line.substr(startPos, line.length()-startPos)));
		return res;
	}
}
