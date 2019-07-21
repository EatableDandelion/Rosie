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
		else
		{
			state.endScope();
		}
	}
	
	std::string ScopeInstruction::getName() const
	{
		return "SCOPE";
	}
	
	
	
	ConstantHeader::ConstantHeader(const std::string& value, const int& type):TemplateInstruction(value+" "+std::to_string(type))
	{}
	
	ConstantHeader::ConstantHeader()
	{}
	
	void ConstantHeader::read(const std::string& command, State& state) const
	{
		char name[20];
		int type;
		sscanf(command.c_str(), "%s %d", &name, &type);
		defineConstant(state, std::string(name), type);
	}
	
	void ConstantHeader::defineConstant(State& state, const std::string& value, const int& typeId) const
	{
		TokenType type = TokenType(typeId);
		if(type == TokenType::CSTINT)
		{			
			state.addConstant(Variable(std::stoi(value)));
		}
		else if(type == TokenType::CSTFLOAT)
		{
			state.addConstant(Variable(std::stof(value)));
		}
		else if(type == TokenType::CSTBOOLEAN)
		{
			if(value == "true")
			{
				state.addConstant(Variable(true));
			}
			else
			{
				state.addConstant(Variable(false));
			}
		}
		else if(type == TokenType::CSTSTRING)
		{
			state.addConstant(Variable(value));
		}
		else if(type == TokenType::CSTARRAY)
		{
			state.addConstant(Variable());
		}
	}
	
	std::string ConstantHeader::getName() const
	{
		return "CONSTANT DECLARATION";
	}
	
	
	VariableHeader::VariableHeader(const int& id, const std::string& name, const int& type):TemplateInstruction(std::to_string(id)+" "+name+" "+std::to_string(type))
	{}
	
	VariableHeader::VariableHeader()
	{}
	
	void VariableHeader::read(const std::string& command, State& state) const
	{
		int id;
		char name[20];
		int type;
		sscanf(command.c_str(), "%d %s %d", &id, &name, &type);
		defineVariable(state, std::string(name), id, type);
	}
	
	void VariableHeader::defineVariable(State& state, const std::string& name, const int& id, const int& typeId) const
	{
		state.addVariable(name, typeId, Handle(id, Category::VARIABLE));
	}
	
	std::string VariableHeader::getName() const
	{
		return "VARIABLE DECLARATION";
	}
	
	
	FunctionHeader::FunctionHeader(const int& id, const std::string& name):TemplateInstruction(std::to_string(id)+" "+name)
	{}
	
	FunctionHeader::FunctionHeader()
	{}
	
	void FunctionHeader::read(const std::string& command, State& state) const
	{
		char name[20];
		int id;
		sscanf(command.c_str(), "%d %s", &id, &name);
		defineFunction(state, std::string(name), id);
	}
	
	void FunctionHeader::defineFunction(State& state, const std::string& name, const int& id) const
	{
		state.addFunction(id, name);
	}
	
	std::string FunctionHeader::getName() const
	{
		return "FUNCTION DECLARATION";
	}
}
