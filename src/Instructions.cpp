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
		std::cout << command.c_str() << std::endl;
		std::vector<std::string> args = Rosie::split(command, " ");
		int destId = args[0];
		int srcId = args[1];
		int srcCategory = args[2];
		
		//scanf(command.c_str(), "%d %d %d", &destId, &srcId, &srcCategory);

		//Handle destHandle(destId, Category::VARIABLE);
		//Handle srcHandle(srcId, Category(srcCategory));
		//state.copyVariable(destHandle, srcHandle);
	}
	
	std::string SetInstruction::getName() const
	{
		return "SET";
	}

	ConstantInstruction::ConstantInstruction(const int& index, const Constant& constant):TemplateInstruction(std::to_string(index)+" "+std::to_string(constant.getTypeId())+" "+constant.getValue())
	{}

	ConstantInstruction::ConstantInstruction()
	{}
	
	void ConstantInstruction::read(const std::string& command, State& state) const
	{
		/*int index;
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
		}*/
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
