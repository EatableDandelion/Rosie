#pragma once

#include "VMObjects.h"

namespace Rosie
{
  template<typename T>
  class Instruction
  {
    public:
      template<typename... Args>
      Instruction(Args&&... args):instruction(std::forward<Args>(args)...)
      {}
    
      void runInstruction(const std::string& command, State& state)
      {
        instruction.run(command, state);
      }
    
    private:
      T instruction;
  };
  
  class SetInstruction
  {
    public:
      SetInstruction(const int& destId, const int& srcId,  const int& srcType);
      void run(const std::string& command, State& state) const;
  };
  
  
}
