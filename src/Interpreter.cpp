#include "Interpreter.h"

namespace Rosie{
	
	Variable::Variable(const std::string& type, const std::string& name, const std::string& value):m_type(type), m_name(name), m_value(value)
	{}
	
	Interpreter::Interpreter()	
	{}
	
	void Interpreter::read(const std::string& fileName)
	{
		Lexer stream(fileName);
		std::vector<Token> tokens;
		stream.getTokens(tokens);
	}

}
