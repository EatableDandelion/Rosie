#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include "VMObjects.h"

namespace Rosie
{
	template<typename T>
	struct Function
	{
		public:
			Function(const std::string& name, const std::function<void(std::vector<T>&, State&)> func, const int& id):name(name), id(id)
			{
				m_func = func;
			}
			
			Function(const Function<T>& other):name(other.name), m_func(other.m_func), id(other.id)
			{}
		
			void execute(std::vector<T>& arguments, State& resultStack) const
			{
				m_func(arguments, resultStack);
			}
	
			std::string getName() const
			{
				return name;
			}
	
			int getId() const
			{
				return id;
			}
			
		private:
			std::string name;
			int id;
			std::function<void(std::vector<T>&, State&)> m_func;
	};
	
	enum Opcode
	{
		SET,
		ARG,
		CALL,
		ADD,
		NEG,
		SUB,
		MULT,
		DIV
	};
	
	class Syntax
	{
		public:
			Syntax();
			
			int getOpcodeId(const Opcode& name) const;
			
			void addOpcode(const Opcode& name, const std::function<void(std::vector<Handle>&, State&)> func);
			
			void addMethod(const std::string& name, const std::function<void(std::vector<Variable>&, State&)> func);
			
			bool hasMethod(const std::string& name) const;
			
			void runOpcode(const int& id, std::vector<Handle>& args, State& state);
			
			void execute(const std::string& name, std::vector<Variable>& arguments, State& state) const;
			
			void execute(const int& id, State& state);
			
			std::vector<Function<Variable>> getNativeMethods() const;
			
		private:
			DualMap<int, Opcode, Function<Handle>> opcodes;
			DualMap<int, std::string, Function<Variable>> methods;
	};
	
	
}
