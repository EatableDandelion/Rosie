#pragma once

#include <unordered_map>
#include <cstdio>
#include <string>
#include "VMObjects.h"
#include "InterpreterObjects.h"

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
			
			std::vector<std::string> getCommands() const;
		
		private:
			std::vector<std::string> commands;
	};
	
	
	class Instruction
	{
		public:
			virtual void read(const std::string& command, State& state) const = 0;
			static int createId();
			
		private:
			static int allIds;
	};
  
	template<typename T>
	class TemplateInstruction : public Instruction
	{
		public:
			TemplateInstruction(const std::string& text):text(text)
			{}

			TemplateInstruction()
			{}

			std::string write() const
			{
				return text;
			}

			static int getId()
			{
				return id;
			}

			virtual std::string getName() const = 0;

		private:
			std::string text;
			static int id; 
	};

	template<typename T> int TemplateInstruction<T>::id(Instruction::createId());

	class SetInstruction : public TemplateInstruction<SetInstruction>
	{
		public:
			SetInstruction(const Address& dest, const Address& src);
			SetInstruction();
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
	};

	class ConstantInstruction : public TemplateInstruction<ConstantInstruction>
	{
		  public:
			ConstantInstruction(const int& index, const Constant& constant);
			ConstantInstruction();
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
	};

	class CallInstruction : public TemplateInstruction<CallInstruction>
	{
		  public:
		  	CallInstruction(const int& callId);
			CallInstruction();
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
	};
	
	class AddInstruction : public TemplateInstruction<AddInstruction>
	{
		  public:
		  	AddInstruction(const Address& a, const Address& b);
			AddInstruction();
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
	};
}
