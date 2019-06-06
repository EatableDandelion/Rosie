#include "Interpreter.h"

namespace Rosie{
	
	void Parser::parse(Lexer& lexer, Program& program)
	{
		while(lexer.hasNext())
		{
			//if(lexer.getToken().type == TokenType::VARTYPE) 		//If it's a type such as float, int
			if(program.hasTypeName(lexer.getToken()))
			{		
				parseDeclaration(lexer, program); 					//float a = 2.1;
			}
			else if(lexer.getToken().type == TokenType::KEYWORD)
			{
				parseKeyword(lexer, program);
			}
			else if(program.hasFunctionAddress(lexer.getToken())
				 || program.isConstructor(lexer.getToken()))		//If it's a function or a ctor
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
			//function getX();
			lexer++;
			returnAddress = program.newFunctionAddress(lexer.getToken().value);
		}
		else
		{
			Type tokenType = program.getType(lexer.getToken());
			
			lexer++;
			returnAddress = program.newVarAddress(lexer.getToken());//, tokenType);
		}
		
		Token nextToken;
		if(lexer.peekToken(nextToken, 1) && nextToken == "=")
		{
			parseAssignment(lexer, program);
		}
	}
	
	void Parser::parseKeyword(Lexer& lexer, Program& program)
	{
		if(lexer.getToken() == "define")
		{
			lexer++; 
			Type newType = program.addType(lexer.getToken().value);
			lexer++; //"("
			while(lexer.getToken() != ")")
			{
				lexer++;
				std::string memberType = lexer.getToken().value;
				lexer++;
				program.addMemberToType(newType, lexer.getToken().value, memberType);
				lexer++;//"," or ")"
			}
		}
	}
	
	void Parser::parseAssignment(Lexer& lexer, Program& program)
	{

		Address destAddress = program.getVarAddress(lexer.getToken());//token = "variableName"
		
		lexer++;//token = "="
		checkToken("=",lexer);
		
		lexer++;//token = "2.21"
		
		if(lexer.getToken() == "{")
		{
			program.addInstruction<CompositeInstruction>(destAddress);// start scope
			parseComposite(lexer, program, destAddress);
			program.addInstruction<CompositeInstruction>();//end scope
		}
		else
		{	
			Address srcAddress = functionParser.parse(lexer, program);

			program.addInstruction<SetInstruction>(destAddress, srcAddress);

			checkToken(";", lexer);
		}
	}
	
	//A composite is any variable that is not a primitive type (ie an instance)
	void Parser::parseComposite(Lexer& lexer, Program& program)
	{
		while(lexer.getToken() != "}")
		{	
			lexer++;
			parseAssignment(lexer, program);
		}
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
	
	void Parser::checkToken(const std::string& expectedToken, const Lexer& lexer)
	{
		if(lexer.getToken() != expectedToken)
		{
			Rosie::error("Expected token: "+expectedToken, lexer);
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
			}else if(program.isConstructor(token))
			{
				token.type = TokenType::CONSTRUCTOR;
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
				activeStack.push(program.getAddress(token, lexer));
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
						//program.addInstruction(Opcode::NEG, activeStack.top());
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
							//program.addInstruction(Opcode::ADD, arg1, arg2);
							program.addInstruction<AddInstruction>(arg1, arg2);
						}
						else if(token == "-")
						{
							//program.addInstruction(Opcode::SUB, arg1, arg2);
						}
						else if(token == "*")
						{
							//program.addInstruction(Opcode::MULT, arg1, arg2);
						}
						else if(token == "/")
						{
							//program.addInstruction(Opcode::DIV, arg1, arg2);
						}	
					}
					activeStack.pop();
					activeStack.push(program.getStackAddress());
					
				}
				else if(token.type == TokenType::FUNCNAME)//function
				{
					while(!activeStack.empty())
					{
						//program.addInstruction(Opcode::ARG, activeStack.top());
						activeStack.pop();
					}
					
					if(!stack.empty())
					{
						activeStack = stack.top();
						stack.pop();
					}
					
					//program.addInstruction(Opcode::CALL, program.getFunctionAddress(token, lexer));
					activeStack.push(program.getStackAddress());
				}
				else if(token.type == TokenType::CONSTRUCTOR)
				{
					while(!activeStack.empty())
					{
						//program.addInstruction(Opcode::ARG, activeStack.top());
						activeStack.pop();
					}
					
					if(!stack.empty())
					{
						activeStack = stack.top();
						stack.pop();
					}
					
					//program.addInstruction<CtorInstruction>(Opcode::NEW, token.value);
					activeStack.push(program.getStackAddress());
					
				}
				else
				{	
					Rosie::error("Unexpected token "+token.getString(), lexer);
				}
			}
		}
		if(!activeStack.empty())
		{
			return activeStack.top();
		}
		else
		{
			return program.getStackAddress();
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
			else if(token.type == TokenType::FUNCNAME || token.type == TokenType::CONSTRUCTOR)
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
					stack.top().type == TokenType::CONSTRUCTOR ||
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
