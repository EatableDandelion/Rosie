#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include "Token.h"

namespace Rosie{
	
	struct Variable
	{
		Variable(const float& floatValue);
		Variable(const int& integerValue);
		Variable(const bool& booleanValue);
		Variable();
		
		int type;
		
		union
		{
			float f;
			int i;
			bool b;
		};
	};
	
	class Instruction : public std::enable_shared_from_this<Instruction>
	{
		public:
			//Instruction(const Variable& returnValue, const Variable& arg1, const Variable& arg2);
			
			Instruction(const std::shared_ptr<Instruction>& parent);
			
			Instruction();
			
			std::shared_ptr<Instruction> next(std::shared_ptr<Instruction>& nextInstruction);
			
			std::shared_ptr<Instruction> sub(std::shared_ptr<Instruction>& child);
			
			void addToken(const Token& token);
			
			std::shared_ptr<Instruction> getRoot();
			
			void print() const;
			
			int id;
		private:
			std::shared_ptr<Variable> m_returnValue;
			std::weak_ptr<Instruction> m_parent;
			std::vector<std::shared_ptr<Instruction>> m_arguments;
			std::vector<Token> tokens;
			
	};
}