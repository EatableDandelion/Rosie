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
	class Parser;
	class FunctionParser;
	class Interpreter;
	
	class FunctionParser
	{	
		public:
			FunctionParser(const Syntax& syntax);
			Address parse(Lexer& lexer, Program& program);
		
		private:
			Syntax syntax;
			std::vector<Token> getRPN(const std::vector<Token>& input);//reverse polish notation
			bool isNumber(Token& token);
			bool isUnary(Token& token, Token& previousToken);
			int getOperatorPrecedence(Token& token);
			bool isLeftAssociative(Token& token);
	};
	
	class Parser
	{
		public:
			Parser(const Syntax& syntax);
			
			void parse(Lexer& lexer, Program& program);
		
		private:
			Syntax syntax;
			FunctionParser functionParser;
			
			void parseAssignment(Lexer& lexer, Program& program);
			void parseDeclaration(Lexer& lexer, Program& program);
			void parseKeyword(Lexer& lexer, Program& program);
			void parseComposite(Lexer& lexer, Program& program);
			Address getVariable(const Token& token, Program& program);
			bool isVariable(Lexer& lexer);
			
			void checkToken(const std::string& expectedToken, const Lexer& lexer);
			void checkToken(bool isCorrectToken, const Lexer& lexer);
			
	};
	
	class Interpreter
	{
		public:
			Interpreter();
			
			Program read(const std::string& fileName, const Syntax& syntax);
	};	

}
