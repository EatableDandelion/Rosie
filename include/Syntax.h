#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include "Token.h"


namespace Rosie
{
	template<typename S, typename T>
	struct Function
	{
		public:
			Function(const std::string& name, const std::function<S(std::vector<T>&)> func, const int& id):name(name), id(id)
			{
				m_func = func;
			}
			
			Function(const Function<T>& other):name(other.name), m_func(other.m_func), id(other.id)
			{}
		
			S execute(std::vector<T>& arguments) const
			{
				return m_func(arguments);
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
			std::function<S(std::vector<T>&)> m_func;
	};
	
	class Syntax
	{
		public:
			//usage: addMethod("print", [&](std::vector<Variable>& args){std::cout << args[0] << std::endl;});
			//void addMethod(Function<Variable, Variable> func);
			
			//bool hasMethod(const std::string& name) const;
			/*
			void execute(const std::string& name, std::vector<Variable>& arguments, State& state) const;
			
			void execute(const int& id, State& state);
			*/
			
			//Function<Variable, Variable> getMethod(const int& id) const;
			
			//std::vector<Function<Variable, Variable>> getNativeMethods() const;
			
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
			//DualMap<int, std::string, Function<Variable, Variable>> methods;
			std::vector<std::string> methodNames;
	};
	
	
}
