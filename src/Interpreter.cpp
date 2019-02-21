#include "Interpreter.h"

namespace Rosie{
	
	void Program::addInstruction(const std::string& instruction)
	{
		std::cout << instruction << std::endl;
		//instructions.push_back(instruction);
	}
	
	bool Program::hasVariable(const std::string& name)
	{
		std::size_t id = getId(name);
		return variables.find(id) != variables.end();
	}
	
	int Program::getVarIndex(const std::string& name)
	{
		std::size_t id = getId(name);
		std::vector<std::size_t>::iterator it = std::find(variables.begin(), variables.end(), id);
		
		if(it != variables.end())
		{
			return std::distance(variables.begin(), it)+1;
		}
		else
		{
			variables.push_back(id);
			return getVarIndex(name);
		}
	}
	
	int Program::addConstant(const Variable& variable)
	{
		constants.push_back(variable);
		return -constants.size();//Constants have a negative index to differentiate from addresses.
	}
	
	std::size_t Program::getId(const std::string& name)
	{
		return std::hash<std::string>{}(name);
	}
	
	void Parser::parse(Lexer& lexer, Program& program)
	{
		while(lexer.hasNext())
		{
			lexer++;
			if(lexer.getToken().type == TokenTypes::VARTYPE)
			{
				varDeclaration(lexer, program);
			}
		}
	}
	
	void Parser::varDeclaration(Lexer& lexer, Program& program)
	{		
		Token token = lexer.getToken();
		
		std::string type = token.value;
		
		lexer++;
		std::size_t index = program.getVarIndex(lexer.getToken().value);
		
		lexer++;
		token = lexer.getToken();
		if(token.value == "=")
		{
			varInitialization(index, type, lexer, program);
		}
		else if(token.value != ";")
		{
			std::cout << "Error: unexpected character " << token.value << std::endl; 
		}	
	}
	
	void Parser::varInitialization(const std::size_t& varIndex, const std::string& type, Lexer& lexer, Program& program)
	{
		lexer++;
		
		int valueIndex;
		if(lexer.getToken().type == TokenTypes::VARVALUE)
		{
			valueIndex = parseConstant(type, lexer, program);
		}
		else if(lexer.getToken().type == TokenTypes::VARNAME)
		{
			valueIndex = parseVariable(lexer, program);
		}
		
		program.addInstruction("SET "+std::to_string(varIndex)+" "+std::to_string(valueIndex));
	}
	
	int Parser::parseConstant(const std::string& type, Lexer& lexer, Program& program)
	{
		Variable variable;
		std::string value = lexer.getToken().value;
		if(type == "float")
		{
			variable = Variable(std::stof(value));
		}
		else if(type == "int")
		{
			variable = Variable(std::stoi(value));
		}
		else if(type == "boolean")
		{
			if(value == "true")
			{
				variable = Variable(true);
			}
			else
			{
				variable = Variable(false);
			}
		}
		
		return program.addConstant(variable);
	}
	
	int Lexer::parseVariable(Lexer& lexer, Program& program)
	{
		std::string varName = lexer.getToken().value;
		if(!program.hasVariable())
		{
			std::cout << "Error, variable " << varName << " undefined." <<std::endl;
			return 0;
		}
		else
		{
			return program.getVarIndex(varName);
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

		Parser parser;
		
		parser.parse(stream, program);
		
	/*	while(stream.next())
		{
			std::cout << stream.getToken().value <<std::endl;
		}
	*/	
		return program;
	}

}
