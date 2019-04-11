#include "Instructions.h"

namespace Rosie
{
	int InstructionCounter::createId()
	{
		return allIds++;
	}
	
	SetInstruction::SetInstruction(const int& destId, const int& srcId,  const int& srcCategory):Instruction(std::to_string(destId)+" "+std::to_string(srcId)+"/"+std::to_string(srcCategory))
	{
		std::cout << getId() << std::endl;
	}

	void SetInstruction::read(const std::string& command, State& state) const
	{
		int destId;
		int srcId;
		int srcCategory;
		scanf(command.c_str(), "%i %i/%i", &destId, &srcId, &srcCategory);
		Handle destHandle(desId, Category::VARIABLE);
		Handle srcHandle(srcId, Category(srcCategory));
		state.copyVariable(destHandle, srcHandle);
	}

	ConstantInstruction::ConstantInstruction(const int& index, const Constant& constant):Instruction(std::to_string(index)+" "+std::to_string(constant.getTypeId())+" "+constant.getValue())
	{
		std::cout << getId() << std::endl;
	}

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

	TestInstruction::TestInstruction():Instruction(" ")
	{
		std::cout << getId() << std::endl;
	}

	void TestInstruction::read(const std::string& command, State& state) const
	{

	}
}
