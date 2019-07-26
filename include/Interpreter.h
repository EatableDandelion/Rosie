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
			Address parseCall(Lexer& lexer, Program& program);
			void parseDefinition(Lexer& lexer, Program& program);
			bool isFunction(Lexer& lexer);
		
		private:
			Syntax syntax;
			std::vector<Token> getRPN(const std::vector<Token>& input);//reverse polish notation
			bool isConstant(Token& token);
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
			
			void parseLoop(Lexer& lexer, Program& program);
			void parseAssignment(Lexer& lexer, Program& program);
			void parseArgument(Lexer& lexer, Program& program);
			void parseDeclaration(Lexer& lexer, Program& program);
			void parseScope(Lexer& lexer, Program& program);
			Address getVariable(const Token& token, Program& program);
			bool isVariable(Lexer& lexer);
			
			void checkToken(const std::string& expectedToken, const Lexer& lexer, const std::string& errorMsg = "");
			void checkToken(bool isCorrectToken, const Lexer& lexer, const std::string& errorMsg = "");
			
	};
	
	class Interpreter
	{
		public:
			Interpreter();
			
			Program read(const std::string& fileName, const Syntax& syntax);
	};	

}
