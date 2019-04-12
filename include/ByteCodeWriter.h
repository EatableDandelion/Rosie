#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include "Lexer.h"
#include "InterpreterObjects.h"
#include "Syntax.h"
#include "Instructions.h"

namespace Rosie
{
	
	void error(const std::string& text, const Lexer& lexer);
	
	class Program
	{
		public:
			Program();
			
			template<typename T, typename... Args>
			std::string addInstruction(Args&&... args)
	  		{
				return instructions.addInstruction<T>(args...);
			}
			
			Address getAddress(const Token& token, const Lexer& lexer);
			
			Address newCstAddress(const Token& token);
			
			Address newVarAddress(const std::string& name);//, const Type& type);
			Address newVarAddress(const Token& token);//, const Type& type);
			Address getVarAddress(const Token& token);
			bool hasVarAddress(const Token& token);
			
			Address newFunctionAddress(const std::string& name);
			Address getFunctionAddress(const Token& token, const Lexer& lexer);
			bool hasFunctionAddress(const Token& token);
			bool isConstructor(const Token& token);
			
			void addMemberToType(const Type& type, const std::string& memberName, const std::string& memberType);

			void startScope();
			void endScope();
			
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
			InstructionCollection instructions;
			TypeCollection types;
	};
	
	class ByteCodeWriter
	{
		public:
			void write(const Program& program) const;
	};
	
	class ByteCodeReader
	{
		public:
			ByteCodeReader();
			void read(const std::string& command, State& state) const;
		
		private:
			template<typename T>
			void readInstruction(const std::string& command, State& state)
			{
				T instruction;
				instruction.read(command.substr(command.find("|", std::size_t(0))+1, command.size()), state);
			}
			
			std::map<int, Instruction> instructions;
	};
 }
