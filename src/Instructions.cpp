#include "Instructions.h"

namespace Rosie
{
	
	std::vector<std::string> InstructionCollection::getCommands() const
	{
		return commands;
	}
	
	int InstructionCounter::allIds = 0;
	
	int InstructionCounter::createId()
	{
		return InstructionCounter::allIds++;
	}
	
	SetInstruction::SetInstruction(const int& destId, const int& srcId,  const int& srcCategory):Instruction(std::to_string(destId)+" "+std::to_string(srcId)+"/"+std::to_string(srcCategory))
	{}

	void SetInstruction::read(const std::string& command, State& state) const
	{
		int destId;
		int srcId;
		int srcCategory;
		scanf(command.c_str(), "%i %i/%i", &destId, &srcId, &srcCategory);
		Handle destHandle(destId, Category::VARIABLE);
		Handle srcHandle(srcId, Category(srcCategory));
		state.copyVariable(destHandle, srcHandle);
	}
	
	std::string SetInstruction::getName() const
	{
		return "SET";
	}

	ConstantInstruction::ConstantInstruction(const int& index, const Constant& constant):Instruction(std::to_string(index)+" "+std::to_string(constant.getTypeId())+" "+constant.getValue())
	{}

	void ConstantInstruction::read(const std::string& command, State& state) const
	{
		int index;
		int typeId;
		std::string value;
		scanf(command.c_str(), "%i %i %s", &index, &typeId, &value);
		if(typeId == Rosie::getId("float"))
		{
			state.addConstant(index, Variable(std::stoi(value)));
		}
		else if(typeId == Rosie::getId("int"))
		{
			state.addConstant(index, Variable(std::stof(value)));
		}
		else if(typeId == Rosie::getId("boolean"))
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
		else if(typeId == Rosie::getId("string"))
		{
			state.addConstant(index, Variable(value));
		}
		else
		{
			std::cout << "Constant type " << typeId << " unknown" << std::endl;
		}
	}
	
	std::string ConstantInstruction::getName() const
	{
		return "CST";
	}

	TestInstruction::TestInstruction():Instruction(" ")
	{}

	void TestInstruction::read(const std::string& command, State& state) const
	{

	}
	
	std::string TestInstruction::getName() const
	{
		return "TEST";
	}
}
