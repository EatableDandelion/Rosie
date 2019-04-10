#pragma once

#include <unordered_map>
#include <cstdio>
#include <string>
#include "VMObjects.h"

namespace Rosie
{
  class InstructionCollection
  {
    public:
      template<typename T, typename... Args>
      std::string addInstruction(Args&&... args)
      {
        T instruction(std::forward<Args>(args)...);
        return std::to_string(instruction.getId())+"|"+instruction.getString();
      }
    
      template<typename T>
      void runInstruction(const std::string& command, State& state)
      {
        T instruction;
        instruction.run(command.substr(command.find("|", std::size_t(0))+1, command.size()), state);
      }
  };
  
  class Instruction
  {
    public:
      Instruction(const std::string& text, const int& id);
      Instruction();
    
      std::string getString() const;
      int getId() const;
      virtual void run(const std::string& command, State& state) const = 0;
    
    private:
      std::string text;
      int id;  
  };
  
  class SetInstruction : public Instruction
  {
    public:
      SetInstruction(const int& destId, const int& srcId,  const int& srcType);
      virtual void run(const std::string& command, State& state) const;
  };
  
  
}
