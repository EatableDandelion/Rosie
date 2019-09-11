#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include <memory>
#include <stdio.h>
#include <stack>
#include <algorithm>
#include "Lexer.h"
#include "InterpreterObjects.h"
#include "Syntax.h"
#include "Instructions.h"

namespace Rosie
{
	
	
	struct Memory
	{
		public:
			Memory();
		
			Address getAddress(const Token& token, const Lexer& lexer);
			
			Address newCstAddress(const Token& token);
			
			Address newVarAddress(const std::string& name, const TokenType& type);
			Address newVarAddress(const Token& token, const TokenType& type);
			Address getVarAddress(const Token& token);
			bool hasVarAddress(const Token& token);
			Address setAddress(const Token& destToken, const Address& srcAddress);
			Address setAddress(const Token& destToken, const std::stack<Address>& srcAdresses);
			Address setAddress(const std::string& newName, const Address& srcAddress);
			
			Address newFunctionAddress(const std::string& name);
			Address getFunctionAddress(const Token& token, const Lexer& lexer);
			bool hasFunctionAddress(const Token& token);
			
			std::vector<Constant> getConstants() const;
			std::vector<Address> getVariables() const;
			std::vector<Address> getFunctions() const;
			std::vector<std::string> getCommands() const;
			std::vector<std::string> getHeader() const;
			
			template<typename T, typename... Args>
			std::string addDeclaration(Args&&... args)
	  		{
				return header.addInstruction<T>(args...);
			}
			
			template<typename T, typename... Args>
			std::string addInstruction(Args&&... args)
	  		{
				return instructions.addInstruction<T>(args...);
			}
			
			void setArgument(const Address& argument);
			void setArguments(std::stack<Address>& activeStack, const int& nbArgs = -1);
			void callFunction(std::stack<Address>& activeStack, const Token& token, const Lexer& lexer, const TokenType& returnType = TokenType::UNDEFINED);
			
			void startScope(const Address& destAddress);
			void endScope();
			
			
		private:
			std::vector<Constant> constants;
			AddressMap variables;
			AddressMap functions;
			InstructionCollection header;
			InstructionCollection instructions;
			std::string scopePrefix;
			std::deque<std::string> scopes;
			std::stack<int> scopeArgIndex;
			
			std::string rename(const std::string& name) const;
			std::vector<std::string> getPossibleNames(const std::string& name) const;
	};
	
	class Program
	{
		public:
			Program(const std::string& fileName, const std::vector<std::string>& nativeFunctions);
			
			void startScope(const Address& destAddress);
			void endScope();
			
			Address getStackAddress() const;
			Address getStackAddress(const TokenType& type);
			
			std::shared_ptr<Memory> operator->()
			{
				return memory;
			}
			
			bool hasFunction(const std::string& name);
			
			std::string getFileName() const;
			
		private:
			Syntax syntax;
			std::shared_ptr<Memory> memory;
			std::vector<std::string> functionNames;
			std::string fileName;
	};
	
	class ByteCodeWriter
	{
		public:
			ByteCodeWriter(const std::string& fileName, const std::string& extension);
			void write(const std::vector<std::string>& lines) const;
		
		private:
			std::string fileName;
	};
	
	class ByteCodeReader
	{
		public:
			ByteCodeReader(const std::string& extension, const bool& verbose = false);
		
			void read(State& state) const;
			
			template<typename T, typename... Args>
			void addInstruction(Args&&... args)
			{
				instructions.insert(std::pair<int, std::shared_ptr<T>>(T::getId(), std::make_shared<T>(std::forward<Args>(args)...)));
			}
		
		private:
			std::string extension;
			std::unordered_map<int, std::shared_ptr<Instruction>> instructions;
			bool verbose;
	};
 }
