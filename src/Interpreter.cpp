#include "Interpreter.h"

namespace Rosie{
	
	Memory::Memory(const int& startIndex):head(startIndex)
	{}
	
	Address Memory::newAddress(const std::string& name)
	{
		if(isLeaf())
		{
			return newAddressInScope(name);
		}
		else
		{
			return child->newAddress(name);
		}
	}
	
	Address Memory::newAddressInScope(const std::string& name)
	{
		std::size_t id = getId(name);
		if(available.empty())
		{
			int index = head++;
			addresses.insert(std::pair<std::size_t, Address>(id, Address(index)));
		}
		else
		{
			Address address = available.top();
			available.pop();
			addresses.insert(std::pair<std::size_t, Address>(id, address));
		}
		return addresses[id];
	}
			
	void Memory::destroy(const Address& address)
	{
		//TODO
	}
			
	Address Memory::getAddress(const std::string& name)
	{
		if(hasAddressInScope(name))
		{
			return addresses[getId(name)];
		}
		else
		{
			return child->getAddress(name);
		}
	}
	
	bool Memory::hasAddress(const std::string& name)
	{
		if(hasAddressInScope(name))
		{
			return true;
		}
		else
		{
			if(!isLeaf())
			{
				return child->hasAddress(name);
			}
			else
			{
				return false;
			}
		}
	}
	
	std::size_t Memory::getId(const std::string& name)
	{
		return std::hash<std::string>{}(name);
	}

	void Memory::startScope()
	{
		if(child == nullptr)
		{
			child = std::make_shared<Memory>(head);
		}
		else
		{
			child->startScope();
		}
	}
			
	void Memory::endScope()
	{
		if(child->isLeaf())
		{
			child.reset();
		}
		else
		{
			child->endScope();
		}
	}

	bool Memory::isLeaf() const
	{
		return child == nullptr;
	}
	
	bool Memory::hasAddressInScope(const std::string& name)
	{
		return addresses.find(getId(name)) != addresses.end();
	}
	
	
	Program::Program()
	{}
	
	/*void Program::addInstruction(const std::string& instruction)
	{
		std::cout << instruction << std::endl;
	}*/
	
	Address Program::newCstAddress(const Token& token)
	{
		std::string value = token.value;
		TokenType type = token.type;
		
		Address address(constants.size());
		
		if(type == TokenType::CSTFLOAT)
		{
			 constants.push_back(Variable(std::stof(value)));
		}
		else if(type == TokenType::CSTINT)
		{
			constants.push_back(Variable(std::stoi(value)));
		}
		else if(type == TokenType::CSTBOOLEAN)
		{
			if(value == "true")
			{
				constants.push_back(Variable(true));
			}
			else if(value == "false")
			{
				constants.push_back(Variable(false));
			}
		}
		return address;//Constants have a negative index to differentiate from addresses.	
	}
	
	Address Program::newVarAddress(const std::string& name)
	{
		return variables.newAddress(name);
	}
	
	Address Program::newVarAddress(const Token& token)
	{
		return newVarAddress(token.value);
	}
	
	Address Program::getVarAddress(const Token& token)
	{
		if(!hasVarAddress(token))
		{
			std::cout << "Error, variable " << token << " undefined." <<std::endl;
		}
		return variables.getAddress(token.value);
	}
	
	bool Program::hasVarAddress(const Token& token)
	{
		return variables.hasAddress(token.value);
	}
	
	Address Program::newFunctionAddress(const std::string& name)
	{
		return functions.newAddress(name);
	}
	
	Address Program::getFunctionAddress(const Token& token)
	{
		if(!hasFunctionAddress(token))
		{
			std::cout << "Error, function " << token << " undefined." <<std::endl;
		}
		return functions.getAddress(token.value);
	}
	
	bool Program::hasFunctionAddress(const Token& token)
	{
		return functions.hasAddress(token.value);
	}
	
	void Program::startScope()
	{
		std::cout << "Starting scope" << std::endl;
		variables.startScope();
	}
	
	void Program::endScope()
	{
		std::cout << "End scope" << std::endl;
		variables.endScope();
	}
	
	
	
	
	
	void Parser::parse(Lexer& lexer, Program& program)
	{
		while(lexer.hasNext())
		{
			if(lexer.getToken().type == TokenType::VARTYPE) 		//If it's a type such as float, int
			{		
				parseDeclaration(lexer, program); 					//float a = 2.1;
			}
			else if(isFunction(lexer))								//If it's a function
			{
				parseFunction(Address(), lexer, program);					//add(a, b);
			}
			else if(isVariable(lexer))								//Else it's a variable
			{
				parseAssignment(lexer, program);					//a = 2.1;
			}
			lexer++;
		}
	}
	
	void Parser::parseDeclaration(Lexer& lexer, Program& program)
	{	
		Address returnAddress; 
		
		if(lexer.getToken() == "function")
		{
			lexer++;
			returnAddress = program.newFunctionAddress(lexer.getToken().value);
		}
		else
		{
			lexer++;
			returnAddress = program.newVarAddress(lexer.getToken());
		}
		
		Token nextToken;
		if(lexer.peekToken(nextToken, 1) && nextToken == "=")
		{
			parseAssignment(lexer, program);
		}
	}
	
	void Parser::parseFunction(const Address& destAddress, Lexer& lexer, Program& program)
	{
		program.startScope();
		
		Address functionAddress = program.getFunctionAddress(lexer.getToken());
		
		lexer++;
		checkToken("(", lexer);
		
		int argIndex = 0;
		while(lexer.getToken() != ")")
		{
			lexer++;
			Address arg = program.newVarAddress("arg"+std::to_string(argIndex));
			varInitialization(arg, lexer, program);
			
			lexer++;
			argIndex++;
		}
		program.addInstruction(Opcode::CALL, functionAddress, Address(argIndex), destAddress);
		program.endScope();
	}
	
	void Parser::parseAssignment(Lexer& lexer, Program& program)
	{
		Address destAddress = program.getVarAddress(lexer.getToken());//token = "variableName"
		
		lexer++;//token = "="
		checkToken("=",lexer);
		
		lexer++;//token = "2.21"
		
		varInitialization(destAddress, lexer, program);
		lexer++;
		checkToken(";", lexer);
	}
	
	void Parser::varInitialization(const Address& destAddress, Lexer& lexer, Program& program)
	{
		if(isFunction(lexer))
		{
			parseFunction(destAddress, lexer, program);
		}
		else
		{
			if(lexer.getToken().type == TokenType::VARNAME)
			{
				program.addInstruction(Opcode::SETV, destAddress, parseVariable(lexer, program));
			}
			else
			{
				program.addInstruction(Opcode::SETK, destAddress, program.newCstAddress(lexer.getToken()));
			}

		}
	}
	
	Address Parser::parseVariable(Lexer& lexer, Program& program)
	{
		return program.getVarAddress(lexer.getToken());
	}
	
	bool Parser::isFunction(Lexer& lexer)
	{
		Token nextToken;
		if(lexer.getToken().type == TokenType::VARNAME && lexer.peekToken(nextToken, 1) && nextToken == "(")
		{
			return true;
		}
		return false;
	}
	
	bool Parser::isVariable(Lexer& lexer)
	{
		TokenType type = lexer.getToken().type;
		if(type == TokenType::CSTBOOLEAN || type == TokenType::CSTSTRING || type == TokenType::CSTINT || type == TokenType::CSTFLOAT || type == TokenType::VARNAME)
		{
			return true;
		}
		return false;
	}
	
	void Parser::error(const std::string& text, const Lexer& lexer)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Error line "<<std::to_string(lexer.getLineIndex())<<", at token [ "<<lexer.getToken()<<" ]:"<< std::endl;
		std::cout << lexer.getLine() << std::endl;
		std::cout << text << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
	}
	
	void Parser::checkToken(const std::string& expectedToken, const Lexer& lexer)
	{
		if(lexer.getToken() != expectedToken)
		{
			error("Expected token: "+expectedToken, lexer);
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
		
		/*while(stream.hasNext())
		{
			Token test;
			if(stream.peekToken(test, 2))
			{
				std::cout << test.value << " ";
			}
			std::cout << stream.getToken().value <<std::endl;
			stream++;
		}*/
	
		return program;
	}

}
