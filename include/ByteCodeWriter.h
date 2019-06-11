#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include <memory>
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
			Address getFunctionAddress(const Token& token, const Lexer& lexer) const;
			bool hasFunctionAddress(const Token& token) const;
			
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
			ByteCodeWriter(const std::string& fileName);
			void write(const Program& program) const;
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
