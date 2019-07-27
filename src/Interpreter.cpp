#include "Interpreter.h"

namespace Rosie{
	
	Parser::Parser(const Syntax& syntax):syntax(syntax), functionParser(syntax)
	{}
	
	void Parser::parse(Lexer& lexer, Program& program)
	{
		while(lexer.hasNext())
		{
			parseLoop(lexer, program);
		}
	}
	
	void Parser::parseLoop(Lexer& lexer, Program& program)
	{
		
		
		if(functionParser.isFunction(lexer))					//If it's a function
		{
			if(program.hasFunction(lexer.getToken().value))
			{
				functionParser.parseCall(lexer, program);		//add(a, b);
			}
			else
			{
				functionParser.parseDefinition(lexer, program);
			}
		}
		else if(isVariable(lexer))						//Else it's a variable
		{
			parseAssignment(lexer, program);
			
			/*Token nextToken;						
			if(lexer.peekToken(nextToken, 1))
			{
				if(syntax.isAssignment(nextToken))			//a = 2.1;
				{
					parseAssignment(lexer, program);
				}
				else if(syntax.isSeparator(nextToken) || syntax.isArgEnd(nextToken))
				{
					parseArgument(lexer, program);
				}
				else
				{
					lexer++;
					throw SyntaxError("Unexpected token.", lexer);
				}
			}*/
		}
		else
		{
			throw SyntaxError("Unexpected token.", lexer);
		}
		lexer++;
	}
	
	/*void Parser::parseDeclaration(Lexer& lexer, Program& program)
	{	
		Address returnAddress; 
		
		if(syntax.isFunctionDeclaration(lexer.getToken()))
		{
			//function getX();
			lexer++;
			returnAddress = program->newFunctionAddress(lexer.getToken().value);
		}
		else
		{
			TokenType tokenType = TokenType::UNDEFINED;
			if(lexer.getToken().value == "float")
			{
				tokenType = TokenType::CSTFLOAT;
			}
			else if(lexer.getToken().value == "int")
			{
				tokenType = TokenType::CSTINT;
			}
			else if(lexer.getToken().value == "boolean")
			{
				tokenType = TokenType::CSTBOOLEAN;
			}
			else if(lexer.getToken().value == "string")
			{
				tokenType = TokenType::CSTSTRING;
			}
			lexer++;
			returnAddress = program->newVarAddress(lexer.getToken(), tokenType);
		}
		
		Token nextToken;
		if(lexer.peekToken(nextToken, 1))
		{
			if(syntax.isAssignment(nextToken))
			{
				parseAssignment(lexer, program);
			}
			else if(syntax.isSeparator(nextToken) || syntax.isEndScope(nextToken))
			{
				parseArgument(lexer, program);
			}
			else
			{
				lexer++;
				throw SyntaxError("Unexpected token.", lexer);
			}
		}
	}*/
	
	void Parser::parseAssignment(Lexer& lexer, Program& program)
	{
		
		Address destAddress;
		Token var = lexer.getToken();
		checkToken(var.type == TokenType::VARNAME, lexer, "Variable name expected.");
		lexer++;//token = "="
		checkToken(syntax.isAssignment(lexer.getToken()), lexer, "Assignment operator expected.");
		lexer++;//token = "2.21"
		
		
		if(syntax.isStartScope(lexer.getToken()) || syntax.isArgStart(lexer.getToken())) // If token = { or token = (
		{
			if(program->hasVarAddress(var))
			{
				destAddress = getVariable(var, program);
			}
			else
			{
				destAddress = program->newVarAddress(var, TokenType::CSTARRAY);
			}
			
			program.startScope(destAddress);
			
			if(syntax.isStartScope(lexer.getToken())
			{
				parseScope(lexer, program);
			}
			else
			{
				parseArray(lexer, program);
			}
			program.endScope();
		}
		else if(
		else
		{	
			destAddress = program->setAddress(var, functionParser.parseCall(lexer, program));
		}
		checkToken(syntax.isTerminator(lexer.getToken()) || syntax.isArgEnd(lexer.getToken()), lexer, "Terminator expected.");
	}

	void Parser::parseArray(Lexer& lexer, Program& program)
	{
		lexer++;
		while(!syntax.isArgEnd(lexer.getToken()))
		{
			Address argument;
			if(syntax.isArgStart(lexer.getToken()))
			{
				argument = program->newVarAddress(var, TokenType::CSTARRAY);
				program->startScope(destAddress);
				program->setArgument(destAddress);
				parseArray(lexer, program);
				program->endScope();
			}
			else
			{
				Token nextToken;
				if(syntax.isAssignment(nextToken))
				{
					parseAssignment(lexer, program);
				}
				else
				{
					argument = program->getAddress(lexer.getToken(), lexer);
					program->setArgument(argument);
				}
				
			}
			lexer++;
		}
		lexer++;
	}
	
	void Parser::parseScope(Lexer& lexer, Program& program)
	{
		lexer++;
		while(!syntax.isEndScope(lexer.getToken()))
		{
			parseLoop(lexer, program);
		}
		lexer++;
	}
	
	Address Parser::getVariable(const Token& token, Program& program)
	{
		return program->getVarAddress(token);
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
	
	void Parser::checkToken(const std::string& expectedToken, const Lexer& lexer, const std::string& errorMsg)
	{
		checkToken(lexer.getToken() == expectedToken, lexer, errorMsg);
	}
	
	void Parser::checkToken(bool isCorrectToken, const Lexer& lexer, const std::string& errorMsg)
	{
		if(!isCorrectToken)
		{
			throw SyntaxError("Unexpected token: "+errorMsg, lexer);
		}
	}
	
	
	
	FunctionParser::FunctionParser(const Syntax& syntax):syntax(syntax)
	{}
	
	Address FunctionParser::parseCall(Lexer& lexer, Program& program)
	{
		std::vector<Token> infixInput;
		
		/** Create function addresses */
		while(!syntax.isTerminator(lexer.getToken()))
		{
			Token token = lexer.getToken();
			if(isFunction(lexer))
			{
				token.type = TokenType::FUNCNAME;
				if(!program->hasFunctionAddress(token))
				{
					program->newFunctionAddress(token.value);
				}
			}
			
			infixInput.push_back(token);
			lexer++;
		}
		
		/** Get the reverse polish notation of the line */
		std::vector<Token> rpn = getRPN(infixInput);

		
		/** Process line, token by token */
		std::stack<std::stack<Address>> stack; //stack of stack to handle variable nb of args.
		std::stack<Address> activeStack;
		
		for(Token token : rpn)
		{
			
			if(isConstant(token) || token.type == TokenType::VARNAME)
			{	/** If it's a variable or a constant */
				activeStack.push(program->getAddress(token, lexer));
			}
			else if(token == "|")
			{	/** If it's the end of the sequence of argument */
				stack.push(activeStack);
				std::stack<Address> newActiveStack;
				activeStack = newActiveStack;
			}
			else
			{
				if(token.type == TokenType::OPERATOR)
				{
					if(!program->hasFunctionAddress(token))
					{
						program->newFunctionAddress(token.value);
					}
					if(token == "u-" || token == "u+")
					{
						program->setArguments(activeStack, 1);
		
					}
					else
					{
						program->setArguments(activeStack, 2);
					}
					program->callFunction(activeStack, token, lexer, TokenType::CSTFLOAT);
	
				}
				else if(token.type == TokenType::FUNCNAME)//function
				{
					program->setArguments(activeStack);
					
					if(!stack.empty())
					{
						activeStack = stack.top();
						stack.pop();
					}
					
					program->callFunction(activeStack, token, lexer);
				}
				else
				{	
					//Rosie::error("Unexpected token "+token.getString(), lexer);
					throw SyntaxError("Unexpected token "+token.getString(), lexer);
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
	
	void FunctionParser::parseDefinition(Lexer& lexer, Program& program)
	{
		std::string funcName = lexer.getToken().value;
		lexer++;
		std::vector<Token> args; 
		while(!syntax.isArgEnd(lexer.getToken()))
		{
			lexer++;
			args.push_back(lexer.getToken());
			lexer++;
		}
		lexer++;//token = "="
	}
	
	std::vector<Token> FunctionParser::getRPN(const std::vector<Token>& input)
	{
		std::stack<Token> stack;
		std::vector<Token> output;
		Token previousToken;
		bool firstToken = true;
		for(Token token : input)
		{
			if(isConstant(token) || token.type == TokenType::VARNAME)
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
					!stack.empty() && !syntax.isArgStart(stack.top()) &&
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
			else if(syntax.isSeparator(token))
			{
				while(!syntax.isArgStart(stack.top()))
				{
					output.push_back(stack.top());
					stack.pop();
				}
			}
			else if(syntax.isArgStart(token))
			{
				Token wallSeparator; //WAAAALL notation, to know how many args for the function called
				wallSeparator.value = "|";
				wallSeparator.type = TokenType::SEPARATOR;
				output.push_back(wallSeparator);
				stack.push(token);
			}				
			else if(syntax.isArgEnd(token))
			{
				while(!syntax.isArgStart(stack.top()))
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
	
	bool FunctionParser::isConstant(Token& token)
	{
		return 	token.type == TokenType::CSTFLOAT 	|| 
				token.type == TokenType::CSTINT 	|| 
				token.type == TokenType::CSTBOOLEAN ||
				token.type == TokenType::CSTSTRING	||
				token.type == TokenType::CSTARRAY;
	}
	
	bool FunctionParser::isUnary(Token& token, Token& previousToken)
	{
		if(token.type != TokenType::OPERATOR)return false;
		if(syntax.isArgEnd(previousToken))return false;
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
		else if(token == "u+" || token == "u-")
		{
			return 3;
		}
		return 0;
	}
	
	bool FunctionParser::isLeftAssociative(Token& token)
	{
		return token != "^";
	}

	bool FunctionParser::isFunction(Lexer& lexer)
	{
		Token nextToken;
		return (lexer.peekToken(nextToken, 1) && syntax.isFunction(lexer.getToken(), nextToken));
	}
	
	
	
	
	Interpreter::Interpreter()	
	{
		//addFunction()
	}
	
	Program Interpreter::read(const std::string& fileName, const Syntax& syntax)
	{	
		Program program(fileName, syntax.getNativeFunctions());
		
		Lexer stream(fileName+".ros");

		Parser parser(syntax);
		
		parser.parse(stream, program);

		return program;
	}

}
