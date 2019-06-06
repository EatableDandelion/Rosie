#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include "Lexer.h"
#include "InterpreterObjects.h"
#include "ByteCodeWriter.h"
#include "Syntax.h"

namespace Rosie{
	
	
	
	class FunctionParser
	{	
		public:
			Address parse(Lexer& lexer, Program& program);
		
		private:
			std::vector<Token> getRPN(const std::vector<Token>& input);//reverse polish notation
			bool isNumber(Token& token);
			bool isUnary(Token& token, Token& previousToken);
			int getOperatorPrecedence(Token& token);
			bool isLeftAssociative(Token& token);
	};
	

	class Parser
	{
		public:
			void parse(Lexer& lexer, Program& program);
		
		private:
			FunctionParser functionParser;
			
			void parseAssignment(Lexer& lexer, Program& program);
			void parseDeclaration(Lexer& lexer, Program& program);
			void parseKeyword(Lexer& lexer, Program& program);
			void parseComposite(Lexer& lexer, Program& program, Address& destAddress);
			Address getVariable(const Token& token, Program& program);
			bool isVariable(Lexer& lexer);
			
			void checkToken(const std::string& expectedToken, const Lexer& lexer);
			
	};
	
	class Interpreter
	{
		public:
			Interpreter();
			
			Program read(const std::string& fileName);
	};	

}
