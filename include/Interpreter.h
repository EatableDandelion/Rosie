#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include "Lexer.h"

namespace Rosie{
	
	struct Variable
	{
		Variable(const std::string& type, const std::string& name, const std::string& value);
		std::string m_type;
		std::string m_name;
		std::string m_value;
	};
	
	class Interpreter
	{
		public:
			Interpreter();
			
			void read(const std::string& fileName);
			

		private:
			std::unordered_map<std::string, Variable> variables;
	};
	

}
