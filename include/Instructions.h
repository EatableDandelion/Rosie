#pragma once

#include <unordered_map>
#include <cstdio>
#include <string>
#include <initializer_list>
#include <queue>
#include "VMObjects.h"
#include "InterpreterObjects.h"
#include "Syntax.h"

namespace Rosie
{
	
	class AddressTranslator
	{
		public:
			std::string toString(const std::initializer_list<Address>& addresses) const;

			std::vector<Handle> toHandles(const std::string& string) const;
			
	};
	
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
			virtual std::string getName() const = 0;
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
			
			TemplateInstruction(const std::initializer_list<Address>& addresses)
			{
				for(const Address& address : addresses)
				{
					text+=address.getId().toString()+" "+std::to_string(address.getCategory())+" ";
				}
				text.pop_back();//remove last character (white space).
			}

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

		protected:
			std::string text;
			
			std::vector<Handle> getHandles(const std::string& string) const
			{
				std::vector<Handle> res;
				
				std::queue<std::string> args;
				for(const std::string& token : Rosie::split(string, " "))
				{
					args.push(token);
				}
				
				while(!args.empty())
				{
					/*int id = std::stoi(args.front());
					args.pop();*/
					std::string textId = args.front();
					args.pop();
					
					int category = std::stoi(args.front());
					args.pop();
					res.push_back(Handle(textId, Category(category)));
				}
				
				return res;
			}

		private:
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
			
		private:
			AddressTranslator translator;
	};
	
	//Push variable onto stack for function call
	class ArgumentInstruction : public TemplateInstruction<ArgumentInstruction>
	{
		  public:
		  	ArgumentInstruction(const Address& address);
			ArgumentInstruction();
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
	};

	//Function call
	class CallInstruction : public TemplateInstruction<CallInstruction>
	{
		public:
		  	CallInstruction(const AddressId& functionId);
			CallInstruction(const Syntax& syntax);
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
		
		private:
			Syntax syntax;
	};
	
	//This instruction deals with non-primitive variabless
	/*class ScopeInstruction : public TemplateInstruction<ScopeInstruction>
	{
		public:
		  	ScopeInstruction(const Address& parent);
			ScopeInstruction();
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
		
		private:
			Address parent;
	};*/
	
	
	class ConstantHeader : public TemplateInstruction<ConstantHeader>
	{
		public:
		  	ConstantHeader(const std::string& value, const int& type);
			ConstantHeader();
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
		
		private:
			void defineConstant(State& state, const std::string& value, const int& typeId) const;
	};
	
	class VariableHeader : public TemplateInstruction<VariableHeader>
	{
		public:
		  	VariableHeader(const AddressId& stringId, const std::string& name, const int& type);
			VariableHeader();
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
		
		private:
			void defineVariable(State& state, const std::string& name, const std::string& textId, const int& typeId) const;
	};
	
	class FunctionHeader : public TemplateInstruction<FunctionHeader>
	{
		public:
		  	FunctionHeader(const AddressId& id, const std::string& name);
			FunctionHeader();
			virtual void read(const std::string& command, State& state) const;
			virtual std::string getName() const;
		
		private:
			void defineFunction(State& state, const std::string& name, const int& id) const;
	};
}
