#include "Syntax.h"

namespace Rosie
{
	Opcode::Opcode(const std::string& name, const std::function<void(std::vector<int>&)> func, const int& id):name(name), func(func), id(id)
	{}
	
	Opcode::Opcode(Opcode&& other):name(other.name), func(other.func), id(other.id)
	{}
	
	void Opcode::execute(std::vector<int>& arguments) const
	{
		func(arguments);
	}
	
	std::string Opcode::getName() const
	{
		return name;
	}
	
	int Opcode::getId() const
	{
		return id;
	}

	Syntax::Syntax()
	{	
		addOpcode("SET", [&](std::vector<int>& args){variables.insert(std::pair<int, Variable>(args[0], variables[args[1]]));});
		addOpcode("ARG", [&](std::vector<int>& args){callStack.push(args[0]);});
		addOpcode("PRINT", [&](std::vector<int>& args){std::cout << variables[args[0]] << std::endl;});
		addOpcode("CALL", [&](std::vector<int>& args){execute(args[0]);});
		addOpcode("ADD", [&](std::vector<int>& args){variables[args[0]].set(variables[args[1]].get<float>()+variables[args[2]].get<float>());});
		addOpcode("NEG", [&](std::vector<int>& args){});
		addOpcode("SUB", [&](std::vector<int>& args){});
		addOpcode("MULT", [&](std::vector<int>& args){});
		addOpcode("DIV", [&](std::vector<int>& args){});
	}
	
	int Syntax::getOpcodeId(const std::string& name) const
	{
		return opcodes.at(name).getId();
	}
	
	void Syntax::addOpcode(const std::string& name, const std::function<void(std::vector<int>&)> func)
	{
		opcodes.insert(std::pair<std::string, Opcode>(name, Opcode(name, func, opcodes.size())));
	}
	
	void Syntax::execute(const std::string& name, std::vector<int>& arguments) const
	{
		if(opcodes.find(name) != opcodes.end())
		{
			opcodes.at(name).execute(arguments);
		}
	}
	
	void Syntax::execute(const int& id)
	{
		for(const auto& pair : opcodes)
		{
			if(pair.second.getId() == id)
			{
				std::vector<int> args;
				while(!callStack.empty())
				{
					args.push_back(callStack.top());
					callStack.pop();
				}
				pair.second.execute(args);
			}
		}
	}

}
