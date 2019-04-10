#include "Instructions.h"

namespace Rosie
{
  Instruction::Instruction(const std::string& text, const int& id):text(text), id(id)
  {}
  
  Instruction::Instruction()
  {}
    
  std::string Instruction::getString() const
  {
    return text;
  }
  
  int Instruction::getId() const
  {
    return id;
  }
  
  SetInstruction::SetInstruction(const int& destId, const int& srcId,  const int& srcCategory):Instruction(std::to_string(destId)+" "+std::to_string(srcId)+"/"+std::to_string(srcCategory), 0)
  {}
  
  void SetInstruction::run(const std::string& command, State& state) const
  {
    int destId;
    int srcId;
    int srcCategory;
   // scanf(command, "%i %i/%i", destId, srcId, srcCategory); 
    //state.copyVariable();
  }
}
