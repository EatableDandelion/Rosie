#include "VirtualMachine.h"

namespace Rosie
{
	
	VirtualMachine::VirtualMachine()	
	{}
	
	void VirtualMachine::execute(const std::vector<Variable>& csts, const std::vector<std::string>& commands)
	{
		state.addConstants(csts);
		
		std::map<int, Variable> variables;
		
		for(const std::string line : commands)
		{
			std::vector<Handle> tokens;
			
			std::size_t startPos = 0;
			std::size_t endPos = line.find(" ", startPos);
			int opcodeId = std::stoi(line.substr(startPos, endPos-startPos));
			startPos = endPos+1;
			std::size_t sepPos = 0;
			int addressId;
			int category;
			while((endPos = line.find(" ", startPos)) != std::string::npos)
			{
				sepPos = line.find("/", startPos);
				addressId = std::stoi(line.substr(startPos, sepPos-startPos));
				sepPos++;
				category = std::stoi(line.substr(sepPos, endPos-sepPos));
				
				tokens.push_back(Handle(addressId, Category(category)));
				startPos = endPos+1;
			}
			sepPos = line.find("/", startPos);
			addressId = std::stoi(line.substr(startPos, sepPos-startPos));
			sepPos++;
			category = std::stoi(line.substr(sepPos, line.length()-sepPos));
			
			tokens.push_back(Handle(addressId, Category(category)));
			syntax.runOpcode(opcodeId, tokens, state);
		}
	}
	
	
	Handle VirtualMachine::parseHandle(const std::string& line) const
	{
		std::size_t slashPos = line.find("/", std::size_t(0));
		int id = std::stoi(line.substr(std::size_t(0), slashPos-std::size_t(0)));
		slashPos++;
		int category = std::stoi(line.substr(slashPos, line.length()-slashPos));

		return Handle(id, Category(category));	
	}
}
