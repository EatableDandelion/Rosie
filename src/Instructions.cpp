#include "Instructions.h"

namespace Rosie
{
	
	std::string AddressTranslator::toString(const std::initializer_list<Address>& addresses) const
	{
		std::string res = "";
		for(const Address& address : addresses)
		{
			res+=std::to_string(address.getId())+" "+std::to_string(address.getCategory())+" ";
		}
		return res;
	}
	
	std::vector<Handle> AddressTranslator::toHandles(const std::string& string) const
	{
		std::vector<Handle> res;
		
		std::queue<std::string> args;
		for(const std::string& token : Rosie::split(string, " "))
		{
			args.push(token);
		}
		
		while(!args.empty())
		{
			int id = std::stoi(args.front());
			args.pop();
			int category = std::stoi(args.front());
			args.pop();
			res.push_back(Handle(id, Category(category)));
		}
		
		return res;
	}
	
	std::vector<std::string> InstructionCollection::getCommands() const
	{
		return commands;
	}
	
	int Instruction::allIds = 0;
	
	int Instruction::createId()
	{
		return Instruction::allIds++;
	}
	
	SetInstruction::SetInstruction(const Address& dest, const Address& src):TemplateInstruction({dest, src})
	{}
	
	SetInstruction::SetInstruction()
	{}

	void SetInstruction::read(const std::string& command, State& state) const
	{			
		std::vector<Handle> handles = getHandles(command);
		state.copyVariable(handles[0], handles[1]);
	}
	
	std::string SetInstruction::getName() const
	{
		return "SET";
	}
	
	
	ArgumentInstruction::ArgumentInstruction(const Address& address):TemplateInstruction({address})
	{}
	
	ArgumentInstruction::ArgumentInstruction()
	{}

	void ArgumentInstruction::read(const std::string& command, State& state) const
	{
		state.push(state.getVariable(getHandles(command)[0]));
	}
	
	std::string ArgumentInstruction::getName() const
	{
		return "ARG";
	}
	
	
	CallInstruction::CallInstruction(const int& functionId):TemplateInstruction(std::to_string(functionId))
	{}
	
	CallInstruction::CallInstruction(const Syntax& syntax):syntax(syntax)
	{}

	void CallInstruction::read(const std::string& command, State& state) const
	{
		state.execute(std::stoi(command));
	}
	
	std::string CallInstruction::getName() const
	{
		return "CALL";
	}

	
	ScopeInstruction::ScopeInstruction(const Address& parent):TemplateInstruction({parent}), parent(parent)
	{}
	
	ScopeInstruction::ScopeInstruction()
	{}
	
	void ScopeInstruction::read(const std::string& command, State& state) const
	{
		if(command != "")
		{
			Handle handle = getHandles(command)[0];
			state.startScope(handle);
		}
	}
	
	std::string ScopeInstruction::getName() const
	{
		return "SCOPE";
	}
}
