#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <unordered_map>
#include "Lexer.h"
#include "Objects.h"
#include "Syntax.h"

namespace Rosie{
	
	void error(const std::string& text, const Lexer& lexer);
	
	struct Memory //collection of addresses
	{
		public:	
			Memory(const std::size_t& category, const int& startIndex = 0);//address 0 is reserved for default, dummy address (ex: return address of function returning nothing)
		
			Address newAddress(const std::string& name);
			
			Address getAddress(const std::string& name);
			
			bool hasAddress(const std::string& name);
			
			void startScope();
			
			void endScope();
						
		private:
			std::unordered_map<std::size_t, Address> addresses;
			int head;
			std::stack<int> scope;
			std::size_t category; //category can be register, stack, temp address...
	};
	
	class Program
	{
		public:
			Program();
			
			template<typename... As>
			void addInstruction(const std::string& command, As... addresses)
			{		
				std::string instruction = translateInstruction(addresses...);
				std::cout << command << " " << instruction << std::endl;
				instructions.push_back(std::to_string(syntax.getOpcodeId(command))+" "+instruction);
			}
			
			Address getAddress(const Token& token, const Lexer& lexer);
			
			Address newCstAddress(const Token& token);
			
			Address newVarAddress(const std::string& name);
			Address newVarAddress(const Token& token);
			Address getVarAddress(const Token& token);
			bool hasVarAddress(const Token& token);
			
			Address newFunctionAddress(const std::string& name);
			Address getFunctionAddress(const Token& token, const Lexer& lexer);
			bool hasFunctionAddress(const Token& token);
			bool isConstructor(const Token& token);

			void startScope();
			void endScope();
			
			std::vector<Variable> getConstants() const;
			std::vector<std::string> getCommands() const;
			
			Address getStackAddress() const;
			
			void addType(const Type& type);
			
			Type getType(const std::string& name) const;
			
			Type getType(const Token& token) const;
	
			bool hasTypeName(const Token& token) const;
			
		private:
			Syntax syntax;
			std::vector<Variable> constants;
			Memory variables;
			Memory functions;
			std::vector<std::string> instructions;
			int allTypeIds = 0;
			std::unordered_map<std::size_t, Type> types;
			
			template<typename A>
			std::string translateInstruction(A& address)
			{
				return address.getString();
			}
			
			template<typename A, typename... As>
			std::string translateInstruction(A& address, As&... addresses)
			{
				return address.getString()+" "+translateInstruction(addresses...);
			}
	};
	
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
