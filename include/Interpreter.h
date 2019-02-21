#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include "Lexer.h"
#include "Instruction.h"

namespace Rosie{
	
	class Program
	{
		public:
			void addInstruction(const std::string& instruction);
			bool hasVariable(const std::string& name);
			int getVarIndex(const std::string& name);
			int addConstant(const Variable& variable);
			
		private:
			std::vector<std::string> instructions;
			std::vector<std::size_t> variables; // hashed name
			std::vector<Variable> constants;
			
			std::size_t getId(const std::string& name);
	};
	
	class Parser
	{
		public:
			void parse(Lexer& lexer, Program& program);
		
		private:
			void varDeclaration(Lexer& lexer, Program& program);
			void varInitialization(const std::size_t& varIndex, const std::string& type, Lexer& lexer, Program& program);
			int parseConstant(const std::string& type, Lexer& lexer, Program& program);
			int parseVariable(Lexer& lexer, Program& program);
	};
	
	class Interpreter
	{
		public:
			Interpreter();
			
			Program read(const std::string& fileName);
	};	

}
