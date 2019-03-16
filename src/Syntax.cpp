#include "Syntax.h"

namespace Rosie
{
	Opcode::Opcode(const std::string& name, const std::function<void(std::vector<Address>&)> func):name(name), func(func)
	{}
	
	void Opcode::execute(const std::vector<Address>& arguments)
	{
		func(arguments);
	}
	
	std::string Opcode::getName() const
	{
		return name;
	}

	Syntax::Syntax()
	{	
		addOpcode("ADD", [](std::vector<Address>& args){args(0).setValue(args(1).getValue()+args(2).getValue());});
	}
	//TODO add the opcodes here
	//TODO add the value to the address directly: setValue and getValue
	
	void Syntax::addOpcode(const std::string& name, const std::function<void(std::vector<Address>&)> func)
	{
		opcodes[name] = Opcode(name, func);
	}
	
	void Syntax::execute(const std::string& name, const std::vector<Address>& arguments)
	{
		opcodes[name].execute(arguments);
	}

}
