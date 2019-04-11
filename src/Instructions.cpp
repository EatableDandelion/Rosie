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
   // scanf(command, "%i %i/%i", destId, srcId, srcCategory); 
    //state.copyVariable();
  }
  
  ConstantInstruction::ConstantInstruction(const int& index, const Constant& constant):Instruction(std::to_string(index)+" "+std::to_string(constant.getTypeId())+" "+constant.getValue())
  {
	  std::cout << getId() << std::endl;
  }
  
  void ConstantInstruction::read(const std::string& command, State& state) const
  {
   
  }
  
  TestInstruction::TestInstruction():Instruction(" ")
  {
	  std::cout << getId() << std::endl;
  }
  
  void TestInstruction::read(const std::string& command, State& state) const
  {
   
  }
}
