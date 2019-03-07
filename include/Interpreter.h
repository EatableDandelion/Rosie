#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <unordered_map>
#include "Lexer.h"
#include "Instruction.h"

namespace Rosie{
	
	enum Opcode
	{
		SETK,		//set variable from constant: 	SETK	destAddress	cstSrcAddress
		SETV,		//set variable from variable: 	SETV	destAddress	varSrcAddress
		CALL,		//call function:				CALL	funcAddress	nbArguments		returnAddress
		PRINT		//print value to console		PRINT	varAddress
	};
	
	const std::vector<std::string> OpcodeNames {"SETK", 
												"SETV", 
												"CALL",
												"PRINT"};
	
	struct Memory //collection of addresses
	{
		public:	
			Memory(const int& startIndex = 1);//address 0 is reserved for default, dummy address (ex: return address of function returning nothing)
		
			Address newAddress(const std::string& name);
			
			void destroy(const Address& address);
			
			Address getAddress(const std::string& name);
			
			bool hasAddress(const std::string& name);
			
			void startScope();
			
			void endScope();
			
			
		private:
			std::unordered_map<std::size_t, Address> addresses;
			std::stack<Address> available;
			std::size_t getId(const std::string& name);
			int head;
			std::shared_ptr<Memory> child;
			bool isLeaf() const;
			bool hasAddressInScope(const std::string& name);
			Address newAddressInScope(const std::string& name);
	};
	
	class Program
	{
		public:
			Program();
			
			template<typename... As>
			void addInstruction(const Opcode& command, As... addresses)
			{
				
				std::string instruction = translateInstruction(addresses...);
				std::cout << OpcodeNames[command] << " " << instruction << std::endl;
				instructions.push_back(std::to_string(command)+" "+instruction);
			}
			
			Address newCstAddress(const Token& token);
			
			Address newVarAddress(const std::string& name);
			Address newVarAddress(const Token& token);
			Address getVarAddress(const Token& token);
			bool hasVarAddress(const Token& token);
			
			Address newFunctionAddress(const std::string& name);
			Address getFunctionAddress(const Token& token);
			bool hasFunctionAddress(const Token& token);

			void startScope();
			void endScope();
			
		private:
			std::vector<Variable> constants;
			Memory variables;
			Memory functions;
			std::vector<std::string> instructions;
			
			template<typename A>
			std::string translateInstruction(A address)
			{
				return std::to_string(address.id);
			}
			
			template<typename A, typename... As>
			std::string translateInstruction(A address, As... addresses)
			{
				return std::to_string(address.id)+" "+translateInstruction(addresses...);
			}
	};
	
	class Parser
	{
		public:
			void parse(Lexer& lexer, Program& program);
		
		private:
			void parseFunction(const Address& destAddress, Lexer& lexer, Program& program);
			void parseAssignment(Lexer& lexer, Program& program);
			void parseDeclaration(Lexer& lexer, Program& program);
			void varInitialization(const Address& destAddress, Lexer& lexer, Program& program);
			Address parseVariable(Lexer& lexer, Program& program);
			bool isFunction(Lexer& lexer);
			bool isVariable(Lexer& lexer);
			
			void error(const std::string& text, const Lexer& lexer);
			void checkToken(const std::string& expectedToken, const Lexer& lexer);
			
	};
	
	class Interpreter
	{
		public:
			Interpreter();
			
			Program read(const std::string& fileName);
	};	

}
