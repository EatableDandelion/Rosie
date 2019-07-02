#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include <memory>
#include <stdio.h>
#include <stack>
#include "Lexer.h"
#include "InterpreterObjects.h"
#include "Syntax.h"
#include "Instructions.h"

namespace Rosie
{
	
	void error(const std::string& text, const Lexer& lexer);
	
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
			
			Address newFunctionAddress(const std::string& name);
			Address getFunctionAddress(const Token& token, const Lexer& lexer);
			bool hasFunctionAddress(const Token& token);
		
			std::vector<Constant> getConstants() const;
			std::vector<Address> getVariables() const;
			std::vector<Address> getFunctions() const;
			std::vector<std::string> getCommands() const;
			
			template<typename T, typename... Args>
			std::string addInstruction(Args&&... args)
	  		{
				return instructions.addInstruction<T>(args...);
			}
			
			void setArguments(std::stack<Address>& activeStack, const int& nbArgs = -1);
			void callFunction(std::stack<Address>& activeStack, const Token& token, const Lexer& lexer, const TokenType& returnType = TokenType::UNDEFINED);
			
			void startScope(const Address& destAddress);
			void endScope();
			
			
		private:
			std::vector<Constant> constants;
			AddressMap variables;
			AddressMap functions;
			InstructionCollection instructions;
			std::string scopePrefix;
			std::deque<std::string> scopes;
			
			std::string rename(const std::string& name) const;
			std::vector<std::string> getPossibleNames(const std::string& name) const;
	};
	
	class Program
	{
		public:
			Program();
			
			void startScope(const Address& destAddress);
			void endScope();
			
			Address getStackAddress() const;
			Address getStackAddress(const TokenType& type);
			
			std::shared_ptr<Memory> operator->()
			{
				return memory;
			}
			
		private:
			Syntax syntax;
			std::shared_ptr<Memory> memory;
	};
	
	
	class HeaderWriter
	{
		public:
			HeaderWriter(const std::string& fileName);
			void write(Program& program);
		private:
			std::string fileName;
	};
	
	class HeaderReader
	{
		public:
			HeaderReader(const std::string& fileName);
			void read(State& state);
		
		private:
			std::string fileName;
			int cstIndex;
			
			void defineConstant(State& state, const std::string& value, const int& type);
			void defineVariable(State& state, const std::string& name, const int& id, const int& typeId) const;
			void defineFunction(State& state, const std::string& name, const int& id) const;
	};
	
	class ByteCodeWriter
	{
		public:
			ByteCodeWriter(const std::string& fileName);
			void write(Program& program) const;
		private:
			std::string fileName;
	};
	
	class ByteCodeReader
	{
		public:
			ByteCodeReader(const std::string& fileName, const Syntax& syntax);
			void read(State& state) const;
		
		private:
			std::string fileName;
			
			template<typename T, typename... Args>
			void addInstruction(Args&&... args)
			{
				instructions.insert(std::pair<int, std::shared_ptr<T>>(T::getId(), std::make_shared<T>(std::forward<Args>(args)...)));
			}
			
			std::unordered_map<int, std::shared_ptr<Instruction>> instructions;
	};
 }
