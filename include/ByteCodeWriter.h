#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include "Lexer.h"
#include "InterpreterObjects.h"
#include "Syntax.h"

namespace Rosie
{
	
	void error(const std::string& text, const Lexer& lexer);
	
  class Program
	{
		public:
			Program();
			
			template<typename... As>
			void addInstruction(const Opcode& command, As... addresses)
			{		
				std::string instruction = translateInstruction(addresses...);
				std::cout << command << " " << instruction << std::endl;
				instructions.push_back(std::to_string(command)+" "+instruction);
			}
			
			Address getAddress(const Token& token, const Lexer& lexer);
			
			Address newCstAddress(const Token& token);
			
			Address newVarAddress(const std::string& name, const Type& type);
			Address newVarAddress(const Token& token, const Type& type);
			Address getVarAddress(const Token& token);
			bool hasVarAddress(const Token& token);
			
			Address newFunctionAddress(const std::string& name);
			Address getFunctionAddress(const Token& token, const Lexer& lexer);
			bool hasFunctionAddress(const Token& token);
			bool isConstructor(const Token& token);
			
			void addMemberToType(const Type& type, const std::string& memberName, const std::string& memberType);

			void startScope();
			void endScope();
			
			//std::vector<Constant> getConstants() const;
			std::vector<std::string> getCommands() const;
			
			Address getStackAddress() const;
			
			Type addType(const std::string& name);
			
			Type getType(const std::string& name) const;
			
			Type getType(const Token& token) const;
	
			bool hasTypeName(const Token& token) const;
			
		private:
			Syntax syntax;
			std::vector<Constant> constants;
			Memory variables;
			Memory functions;
			std::vector<std::string> instructions;
			TypeCollection types;
			
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
 }