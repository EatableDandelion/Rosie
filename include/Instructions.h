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
				std::string command = std::to_string(instruction.getId())+"|"+instruction.write();
				commands.push_back(command);
				return command;
			}
		
		private:
			std::vector<std::string> commands;
	};
	
	class InstructionCounter
	{
		public:
			static int createId();
			
		private:
			static int allIds;
	};
  
	template<typename T>
	class Instruction : InstructionCounter
	{
		public:
			Instruction(const std::string& text):text(text)
			{}

			Instruction()
			{}

			std::string write() const
			{
				return text;
			}

			int getId() const
			{
				return id;
			}

			virtual void read(const std::string& command, State& state) const = 0;

			virtual std::string getName() const = 0;

		private:
			std::string text;
			static int id; 
	};

	template<typename T> int Instruction<T>::id(InstructionCounter::createId());

	class SetInstruction : public Instruction<SetInstruction>
	{
		public:
			SetInstruction(const int& destId, const int& srcId,  const int& srcType);
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
	};

	class ConstantInstruction : public Instruction<ConstantInstruction>
	{
		  public:
			ConstantInstruction(const int& index, const Constant& constant);
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
	};

	class TestInstruction : public Instruction<TestInstruction>
	{
		  public:
			TestInstruction();
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
	};
}
