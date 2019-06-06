#include "Instructions.h"

namespace Rosie
{
	
	std::vector<std::string> InstructionCollection::getCommands() const
	{
		return commands;
	}
	
	int Instruction::allIds = 0;
	
	int Instruction::createId()
	{
		return Instruction::allIds++;
	}
	
	SetInstruction::SetInstruction(const Address& dest, const Address& src):TemplateInstruction(std::to_string(dest.getId())+" "+std::to_string(src.getId())+" "+std::to_string(src.getCategory()))
	{}
	
	SetInstruction::SetInstruction()
	{}

	void SetInstruction::read(const std::string& command, State& state) const
	{		
		std::vector<std::string> args = Rosie::split(command, " ");
		int destId = std::stoi(args[0]);
		int srcId = std::stoi(args[1]);
		int srcCategory = std::stoi(args[2]);

		Handle destHandle(destId, Category::VARIABLE);
		Handle srcHandle(srcId, Category(srcCategory));
		state.copyVariable(destHandle, srcHandle);
	}
	
	std::string SetInstruction::getName() const
	{
		return "SET";
	}

	NewInstruction::NewInstruction(const std::string& name, const Address& var):TemplateInstruction(name+" "+std::to_string(var.getId()))
	{}
	
	NewInstruction::NewInstruction()
	{}

	void NewInstruction::read(const std::string& command, State& state) const
	{
		std::vector<std::string> args = Rosie::split(command, " ");
		state.addVariable(args[0], Handle(std::stoi(args[1]), Category::VARIABLE));
	}
	
	std::string NewInstruction::getName() const
	{
		return "NEW";
	}
	
	ConstantInstruction::ConstantInstruction(const int& index, const Constant& constant):TemplateInstruction(std::to_string(index)+" "+std::to_string(constant.getTypeId())+" "+constant.getValue())
	{}

	ConstantInstruction::ConstantInstruction()
	{}
	
	void ConstantInstruction::read(const std::string& command, State& state) const
	{
		std::vector<std::string> args = Rosie::split(command, " ");
		int index = std::stoi(args[0]);
		TokenType type = TokenType(std::stoi(args[1]));
		std::string value = args[2];

		if(type == TokenType::CSTINT)
		{
			state.addConstant(index, Variable(std::stoi(value)));
		}
		else if(type == TokenType::CSTFLOAT)
		{
			state.addConstant(index, Variable(std::stof(value)));
		}
		else if(type == TokenType::CSTBOOL)
		{
			if(value == "true")
			{
				state.addConstant(index, Variable(true));
			}
			else
			{
				state.addConstant(index, Variable(false));
			}
			
		}
		else if(type == TokenType::CSTSTRING)
		{
			state.addConstant(index, Variable(value));
		}
	}
	
	std::string ConstantInstruction::getName() const
	{
		return "CST";
	}

	CallInstruction::CallInstruction(const int& callId):TemplateInstruction(std::to_string(callId))
	{}
	
	CallInstruction::CallInstruction()
	{}

	void CallInstruction::read(const std::string& command, State& state) const
	{
		//state.execute(std::stoi(command));
	}
	
	std::string CallInstruction::getName() const
	{
		return "CALL";
	}
	
	AddInstruction::AddInstruction(const Address& a, const Address& b):TemplateInstruction(std::to_string(a.getId())+" "+std::to_string(a.getCategory())+" "+std::to_string(b.getId())+" "+std::to_string(b.getCategory()))
	{}
	
	AddInstruction::AddInstruction()
	{}
	
	void AddInstruction::read(const std::string& command, State& state) const
	{
		/*int aId;
		int aCategory;
		int bId;
		int bCategory;
		scanf(command.c_str(), "%i %i %i %i", &aId, &aCategory, &bId, &bCategory);
		Handle aHandle(aId, Category(bCategory));
		Handle bHandle(bId, Category(bCategory));
		state.push(Variable(state.getVariable(aHandle).get<float>()+state.getVariable(bHandle).get<float>()));
	*/}
	
	std::string AddInstruction::getName() const
	{
		return "ADD";
	}
}
