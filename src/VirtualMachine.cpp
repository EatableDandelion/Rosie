#include "VirtualMachine.h"

namespace Rosie
{
	VirtualMachine::VirtualMachine()	
	{}
	
	void VirtualMachine::execute(const std::vector<Variable>& constants, const std::vector<std::string>& commands)
	{
		std::map<int, Variable> variables;
		syntax.setConstants(constants);
		for(const std::string command : commands)
		{
			std::vector<int> tokens = split(command);
			syntax.execute(tokens);
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
