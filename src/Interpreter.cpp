#include "Interpreter.h"

namespace Rosie{
	
	Memory::Memory(const std::size_t& type, const int& startIndex):type(type), head(startIndex)
	{
		scope.push(startIndex);
	}
	
	Address Memory::newAddress(const std::string& name)
	{
		std::size_t id = getId(name);

		int index = head++;
		addresses.insert(std::pair<std::size_t, Address>(id, Address(index, name, type)));

		return addresses[id];
	}
			
	Address Memory::getAddress(const std::string& name)
	{
		return addresses[getId(name)];
	}
	
	bool Memory::hasAddress(const std::string& name)
	{
		return addresses.find(getId(name)) != addresses.end();
	}

	void Memory::startScope()
	{
		scope.push(head);
	}
			
	void Memory::endScope()
	{
		head = scope.top();
		scope.pop();
	}
	
	std::size_t Memory::getId(const std::string& name)
	{
		return std::hash<std::string>{}(name);
	}
	
	
	Program::Program():variables(1), functions(2)
	{
		for(const auto& pair : syntax.getNativeMethods())
		{
			functions.newAddress(pair.first);
		}
	}
	
	Address Program::getAddress(const Token& token)
	{
		if(hasFunctionAddress(token))
		{
			return getFunctionAddress(token);
		}
		else if(hasVarAddress(token))
		{
			return getVarAddress(token);
		}
		else
		{
			return newCstAddress(token);
		}
	}
	
	Address Program::newCstAddress(const Token& token)
	{
		std::string value = token.value;
		TokenType type = token.type;
		
		Address address(constants.size(), value);
		
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
		else
		{
			std::cout << "Constant type undefined" << std::endl;
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
	
	std::vector<Variable> Program::getConstants() const
	{
		return constants;
	}
	
	std::vector<std::string> Program::getCommands() const
	{
		return instructions;
	}
	
	
	
	
	
	void Parser::parse(Lexer& lexer, Program& program)
	{
		while(lexer.hasNext())
		{
			if(lexer.getToken().type == TokenType::VARTYPE) 		//If it's a type such as float, int
			{		
				parseDeclaration(lexer, program); 					//float a = 2.1;
			}
			else if(program.hasFunctionAddress(lexer.getToken()))	//If it's a function
			{
				functionParser.parse(lexer, program);				//add(a, b);
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
	
	void Parser::parseAssignment(Lexer& lexer, Program& program)
	{
		Address destAddress = program.getVarAddress(lexer.getToken());//token = "variableName"
		
		lexer++;//token = "="
		checkToken("=",lexer);
		
		lexer++;//token = "2.21"
		
		Address srcAddress = functionParser.parse(lexer, program);
		
		program.addInstruction("SET", Address(srcAddress.type), destAddress, srcAddress);
		checkToken(";", lexer);
	}
	
	Address Parser::getVariable(const Token& token, Program& program)
	{
		return program.getVarAddress(token);
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
		std::cout << "Error line "<<std::to_string(lexer.getLineIndex())<<", at token ["<<lexer.getToken()<<"]:"<< std::endl;
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
	
	
	
	
	Address FunctionParser::parse(Lexer& lexer, Program& program)
	{
		std::vector<Token> infixInput;
		while(lexer.getToken() != ";")
		{
			Token token = lexer.getToken();
			if(program.hasFunctionAddress(token))
			{
				token.type = TokenType::FUNCNAME;
			}
			infixInput.push_back(token);
			lexer++;
		}
		std::vector<Token> rpn = getRPN(infixInput);

		std::stack<std::stack<Address>> stack; //stack of stack to handle variable nb of args.
		std::stack<Address> activeStack;
		
		
		for(Token token : rpn)
		{
			if(isNumber(token) || token.type == TokenType::VARNAME)
			{
				activeStack.push(program.getAddress(token));
			}
			else if(token == "|")
			{
				stack.push(activeStack);
				std::stack<Address> newActiveStack;
				activeStack = newActiveStack;
			}
			else
			{
				if(token.type == TokenType::OPERATOR)
				{
					if(token == "u-")
					{												
						program.addInstruction("NEG", activeStack.top());
					}
					else if(token == "u+")
					{}
					else
					{
						Address arg2 = activeStack.top();
						activeStack.pop();
						Address arg1 = activeStack.top();			
						
						if(token == "+")
						{
							program.addInstruction("ADD", arg1, arg2);
						}
						else if(token == "-")
						{
							program.addInstruction("SUB", arg1, arg2);
						}
						else if(token == "*")
						{
							program.addInstruction("MULT", arg1, arg2);
						}
						else if(token == "/")
						{
							program.addInstruction("DIV", arg1, arg2);
						}	
					}
					activeStack.pop();
					activeStack.push(Address(0));
					
				}
				else
				{
					while(!activeStack.empty())
					{
						program.addInstruction("ARG", activeStack.top());
						activeStack.pop();
					}
					
					if(!stack.empty())
					{
						activeStack = stack.top();
						stack.pop();
					}
					program.addInstruction("CALL", program.getFunctionAddress(token));
					activeStack.push(Address(0));
				}
			}
		}
		if(!activeStack.empty())
		{
			return activeStack.top();
		}
		else
		{
			return Address(0);
		}
	}
	
	std::vector<Token> FunctionParser::getRPN(const std::vector<Token>& input)
	{
		std::stack<Token> stack;
		std::vector<Token> output;
		Token previousToken;
		bool firstToken = true;
		for(Token token : input)
		{		
			if(isNumber(token) || token.type == TokenType::VARNAME)
			{
				output.push_back(token);
			}
			else if(token.type == TokenType::FUNCNAME)
			{
				stack.push(token);
			}
			else if((token.type == TokenType::OPERATOR && firstToken) || isUnary(token, previousToken))
			{
				token.value = "u"+token.value;
				stack.push(token);
			}
			else if(token.type == TokenType::OPERATOR)
			{			
				while(
					!stack.empty() && stack.top() != "(" &&
					(stack.top().type == TokenType::FUNCNAME ||
					getOperatorPrecedence(token) < getOperatorPrecedence(stack.top()) ||
					(getOperatorPrecedence(token) == getOperatorPrecedence(stack.top()) && isLeftAssociative(stack.top())))
					)
				{
					output.push_back(stack.top());
					stack.pop();
				}
				stack.push(token);				
			}
			else if(token == ",")
			{
				while(stack.top() != "(")
				{
					output.push_back(stack.top());
					stack.pop();
				}
			}
			else if(token == "(")
			{
				Token wallSeparator; //WAAAALL notation, to know how many args for the function called
				wallSeparator.value = "|";
				wallSeparator.type = TokenType::SEPARATOR;
				output.push_back(wallSeparator);
				stack.push(token);
			}				
			else if(token == ")")
			{
				while(stack.top() != "(")
				{
					output.push_back(stack.top());
					stack.pop();
				}
				stack.pop();
			}
			previousToken = token;
			firstToken = false;
		}
		while(!stack.empty())
		{
			output.push_back(stack.top());
			stack.pop();
		}
		return output;
	}
	
	bool FunctionParser::isNumber(Token& token)
	{
		return token.type == TokenType::CSTFLOAT || token.type == TokenType::CSTINT;
	}
	
	bool FunctionParser::isUnary(Token& token, Token& previousToken)
	{
		if(token.type != TokenType::OPERATOR)return false;
		if(previousToken == ")")return false;
		if(previousToken.type == TokenType::SEPARATOR || previousToken.type == TokenType::OPERATOR) return true;
		return false;
	}
	
	int FunctionParser::getOperatorPrecedence(Token& token)
	{
		if(token == "+" || token == "-")
		{
			return 2;
		}
		else if(token == "*" || token == "/")
		{
			return 3;
		}
		else if(token == "^")
		{
			return 4;
		}
		return 0;
	}
	
	bool FunctionParser::isLeftAssociative(Token& token)
	{
		return token != "^";
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
