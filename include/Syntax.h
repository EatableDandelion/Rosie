#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include "Token.h"


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
	
	class Syntax
	{
		public:
			//usage: addMethod("print", [&](std::vector<Variable>& args){std::cout << args[0] << std::endl;});
			void addMethod(Function<Variable> func);
			
			bool hasMethod(const std::string& name) const;
			/*
			void execute(const std::string& name, std::vector<Variable>& arguments, State& state) const;
			
			void execute(const int& id, State& state);
			*/
			
			Function<Variable> getMethod(const int& id) const;
			
			std::vector<Function<Variable>> getNativeMethods() const;
			
			bool isFunctionDeclaration(const Token& token);	
			bool isAssignment(const Token& token);
			bool isClassDeclaration(const Token& token);
			bool isStartScope(const Token& token);	
			bool isEndScope(const Token& token);
			bool isTerminator(const Token& token);
			bool isListSeparator(const Token& token);
			bool isListStart(const Token& token);	
			bool isListEnd(const Token& token);
			
		private:
			DualMap<int, std::string, Function<Variable>> methods;
	};
	
	
}
