#include "Interpreter.h"

namespace Rosie{
	
	Parser::Parser(const Syntax& syntax):syntax(syntax), functionParser(syntax)
	{}
	
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
			else if(functionParser.isFunction(lexer))		//If it's a function or a ctor
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
		
		if(syntax.isFunctionDeclaration(lexer.getToken()))
		{
			//function getX();
			lexer++;
			returnAddress = program.newFunctionAddress(lexer.getToken().value);
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
			returnAddress = program.newVarAddress(lexer.getToken(), tokenType);
		}
		
		Token nextToken;
		if(lexer.peekToken(nextToken, 1) && syntax.isAssignment(nextToken))
		{
			parseAssignment(lexer, program);
		}
	}
	
	void Parser::parseKeyword(Lexer& lexer, Program& program)
	{
		if(syntax.isClassDeclaration(lexer.getToken()))
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

		//Address destAddress = program.getVarAddress(lexer.getToken());//token = "variableName"
		Address destAddress;
		Token var = lexer.getToken();
		
		lexer++;//token = "="
		checkToken(syntax.isAssignment(lexer.getToken()),lexer);
		
		lexer++;//token = "2.21"
		
		if(syntax.isStartScope(lexer.getToken()))
		{
			if(program.hasVarAddress(var))
			{
				destAddress = program.getVarAddress(var);
			}
			else
			{
				destAddress = program.newVarAddress(var, TokenType::CSTARRAY);
			}
			
			program.addInstruction<CompositeInstruction>(destAddress);// start scope
			parseComposite(lexer, program);
			program.addInstruction<CompositeInstruction>();//end scope
		}
		else
		{	
			Address srcAddress = functionParser.parse(lexer, program);
			
			if(program.hasVarAddress(var))
			{
				destAddress = program.getVarAddress(var);
			}
			else
			{
				destAddress = program.newVarAddress(var, srcAddress.getType());
			}
			
			program.addInstruction<SetInstruction>(destAddress, srcAddress);
			checkToken(syntax.isTerminator(lexer.getToken()), lexer);
		}
	}
	
	//A composite is any variable that is not a primitive type (ie an instance)
	void Parser::parseComposite(Lexer& lexer, Program& program)
	{
		while(!syntax.isEndScope(lexer.getToken()))
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
		checkToken(lexer.getToken() == expectedToken, lexer);
	}
	
	void Parser::checkToken(bool isCorrectToken, const Lexer& lexer)
	{
		if(!isCorrectToken)
		{
			Rosie::error("Unexpected token.", lexer);
		}
	}
	
	
	
	FunctionParser::FunctionParser(const Syntax& syntax):syntax(syntax)
	{}
	
	Address FunctionParser::parse(Lexer& lexer, Program& program)
	{
		std::vector<Token> infixInput;
		while(!syntax.isTerminator(lexer.getToken()))
		{
			Token token = lexer.getToken();
			if(isFunction(lexer))
			{
				token.type = TokenType::FUNCNAME;
				if(!program.hasFunctionAddress(token))
				{
					program.newFunctionAddress(token.value);
				}
			}
			
			infixInput.push_back(token);
			lexer++;
		}
		std::vector<Token> rpn = getRPN(infixInput);

		std::stack<std::stack<Address>> stack; //stack of stack to handle variable nb of args.
		std::stack<Address> activeStack;
		
		std::cout << std::endl;
		for(Token token : rpn)
		{
			std::cout << token.value;
		}
		std::cout << std::endl;
		
		for(Token token : rpn)
		{
			if(isConstant(token) || token.type == TokenType::VARNAME)
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
					if(!program.hasFunctionAddress(token))
					{
						program.newFunctionAddress(token.value);
					}
					if(token == "u-" || token == "u+")
					{
						program.addInstruction<ArgumentInstruction>(activeStack.top());
					}
					else
					{
						Address arg2 = activeStack.top();
						activeStack.pop();
						Address arg1 = activeStack.top();	
						
						program.addInstruction<ArgumentInstruction>(arg1);
						program.addInstruction<ArgumentInstruction>(arg2);
					}
					activeStack.pop();
					activeStack.push(program.getStackAddress(TokenType::CSTFLOAT));
					program.addInstruction<CallInstruction>(program.getFunctionAddress(token, lexer).getId());
					
				}
				else if(token.type == TokenType::FUNCNAME)//function
				{
					/** Double stack exchange necessary to specify args in correct order */
					std::stack<Address> argumentStack;
					while(!activeStack.empty())
					{
						argumentStack.push(activeStack.top());
						activeStack.pop();
					}
					while(!argumentStack.empty())
					{
						program.addInstruction<ArgumentInstruction>(argumentStack.top());
						argumentStack.pop();
					}
					
					if(!stack.empty())
					{
						activeStack = stack.top();
						stack.pop();
					}
					
					program.addInstruction<CallInstruction>(program.getFunctionAddress(token, lexer).getId());
					
					/** TODO: get type of function output. */
					activeStack.push(program.getStackAddress());
				}
				else if(token.type == TokenType::CONSTRUCTOR)
				{
					while(!activeStack.empty())
					{
						activeStack.pop();
					}
					
					if(!stack.empty())
					{
						activeStack = stack.top();
						stack.pop();
					}
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
			if(isConstant(token) || token.type == TokenType::VARNAME)
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
					!stack.empty() && !syntax.isListStart(stack.top()) &&
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
			else if(syntax.isListSeparator(token))
			{
				while(!syntax.isListStart(stack.top()))
				{
					output.push_back(stack.top());
					stack.pop();
				}
			}
			else if(syntax.isListStart(token))
			{
				Token wallSeparator; //WAAAALL notation, to know how many args for the function called
				wallSeparator.value = "|";
				wallSeparator.type = TokenType::SEPARATOR;
				output.push_back(wallSeparator);
				stack.push(token);
			}				
			else if(syntax.isListEnd(token))
			{
				while(!syntax.isListStart(stack.top()))
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
		return 	token.type == TokenType::CSTFLOAT || 
				token.type == TokenType::CSTINT || 
				token.type == TokenType::CSTBOOLEAN ||
				token.type == TokenType::CSTSTRING;
	}
	
	bool FunctionParser::isUnary(Token& token, Token& previousToken)
	{
		if(token.type != TokenType::OPERATOR)return false;
		if(syntax.isListEnd(previousToken))return false;
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
		Program program;
		
		Lexer stream(fileName);

		Parser parser(syntax);
		
		parser.parse(stream, program);
		
		return program;
	}

}
