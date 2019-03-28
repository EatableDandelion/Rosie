#pragma once

#include <vector>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <stack>
#include "Objects.h"

namespace Rosie
{
	template<typename T>
	struct Function
	{
		public:
			Function(const std::string& name, const std::function<void(std::vector<T>&)> func, const int& id):name(name), id(id)
			{
				m_func = func;
			}
			
			Function(const Function<T>& other):name(other.name), m_func(other.m_func), id(other.id)
			{}
		
			void execute(std::vector<T>& arguments) const
			{
				m_func(arguments);
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
			std::function<void(std::vector<T>&)> m_func;
	};
	
	class Syntax
	{
		public:
			Syntax();
			
			int getOpcodeId(const std::string& name) const;
			
			void addOpcode(const std::string& name, const std::function<void(std::vector<int>&)> func);

			void addMethod(const std::string& name, const std::function<void(std::vector<Variable>&)> func);
			
			bool hasMethod(const std::string& name) const;
			
			void execute(const std::vector<int>& args);
			
			void execute(const std::string& name, std::vector<Variable>& arguments) const;
			
			void execute(const int& id);
			
			std::unordered_map<std::string, Function<Variable>> getNativeMethods() const;
			
		private:
			std::unordered_map<std::string, Function<int>> opcodes;
			std::unordered_map<std::string, Function<Variable>> methods;
			std::unordered_map<int, Variable> variables;
			std::unordered_map<int, Variable> constants;
			std::stack<int> callStack;
	};
	
	
}
