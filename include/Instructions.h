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
      void read(const std::string& command, State& state)
      {
        T instruction;
        instruction.read(command.substr(command.find("|", std::size_t(0))+1, command.size()), state);
      }
  };
  
  template<typename T>
  class Instruction
  {
    public:
      Instruction(const std::string& text);
      Instruction();
    
      std::string write() const;
      int getId() const;
      virtual void read(const std::string& command, State& state) const = 0;
    
    private:
      std::string text;
      static int id; 
  };
  
  template<typename T> int Instruction<T>::id(Instruction<T>::id+1);
  
  class SetInstruction : public Instruction<SetInstruction>
  {
    public:
      SetInstruction(const int& destId, const int& srcId,  const int& srcType);
      virtual void read(const std::string& command, State& state) const;
  };
  
  
}
