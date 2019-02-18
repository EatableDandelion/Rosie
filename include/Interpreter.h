#pragma once

#include <string>
#include <iostream>
#include "Lexer.h"
#include "Instruction.h"

namespace Rosie{
	
	struct Parser
	{
		void parse();
	};
	
	struct FunctionParser : public Parser
	{
		
	};
	
	class Interpreter
	{
		public:
			Interpreter();
			
			void read(const std::string& fileName);
	};	

}
