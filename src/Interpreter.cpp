#include "Interpreter.h"

namespace Rosie{
	
	void Program::appendInstruction(const int& instructionId)
	{
		instructions.push_back(instructionId);
	}
	
	void Program::addVariable(const std::string& name, const Variable& variable)
	{
		variables.insert(name, variable);
	}
	
	void AssignementParser::parse(Lexer& lexer, Program& program)
	{
		Token token = lexer.getToken();
		if(token.type == TokenTypes::VARTYPE)
		{
			Variable newVar;
			newVar.type = token.type;
			
			lexer.next();
			std::string name = lexer.getToken().name;
			
			
			lexer.next();
			lexer.next();
			newVar.value = lexer.getToken().value;
			
			program.addVariable(name, newVar);
		}
	}
	
	Interpreter::Interpreter()	
	{
		//addFunction()
	}
	
	Program Interpreter::read(const std::string& fileName)
	{
		Program program;
		
		Lexer stream(fileName);
		
		while(stream.next())
		{
			std::cout << stream.getToken().value <<std::endl;
		}
		
		return program;
	}

}
