#pragma once

#include "VMObjects.h"

namespace Rosie
{
  template<typename T>
  class Instruction
  {
    public:
      Instruction(const Opcodes& opcode):opcode(opcode)
      {}
    
      template<typename... Args>
      std::string getString(Args&&... args) const
      {
        return opcode+"|"+instruction.getString(std::forward<Args>(args)...);
      }
    
      void runInstruction(const std::string& command, State& state)
      {
        instruction.run(command.substr(line.find("|", command)+1, line.size()), state);
      }
    
    private:
      T instruction;
      Opcode opcode;
  };
  
  class SetInstruction
  {
    public:
      std::string getString(const int& destId, const int& srcId,  const int& srcType) const;
      void run(const std::string& command, State& state) const;
  };
  
  
}
