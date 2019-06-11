#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include "Token.h"


namespace Rosie
{	
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
			
			bool isFunction(const Token& token, const Token& nextToken);
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
	};
	
	
}
