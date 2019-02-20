#pragma once

#include <string>
#include <iostream>
#include "Lexer.h"
#include "Instruction.h"

namespace Rosie{
	
	struct Variable
	{
		int type;
		std::string value;
	};
	
	class Program
	{
		public:
			void appendInstruction(const int& instruction);
			void addVariable(const std::string& name, const Variable& variable);
			
		private:
			std::vector<int> instructions;
			std::unordered_map<std::string, Variable> variables;
	};
	
	struct Parser
	{
		virtual void parse(Lexer& lexer, Program& program) = 0;
	};
	
	struct AssignementParser : public Parser
	{
		virtual void parse(Lexer& lexer, Program& program);
	};
	
	class Interpreter
	{
		public:
			Interpreter();
			
			Program read(const std::string& fileName);
	};	

}
