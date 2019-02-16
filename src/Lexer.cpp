#include "Lexer.h"

namespace Rosie
{	
	bool Lex::isWhiteSpace(const char c)
	{
		return c == ' ' || c == '\t' || c == '\n';
	}
	
	bool Lex::isSpecialChar(const char c)
	{
		return isOperator(c) || isSeparator(c);
	}
	
	bool Lex::isOperator(const char c)
	{
		return 
		c == '=' || c == '<' || c == '>' ||	
		c == '+' || c == '-' || c == '*' ||
		c == '/' || c == '^' || c == '%';
	}
	
	bool Lex::isSeparator(const char c)
	{
		return 
		c == ',' || c == ';' || c == '.' || 
		c == ':' || c == '(' || c == ')' ||
		c == '{' || c == '}';
	}
	
	bool Lex::isDigit(const char c)
	{
		return isdigit(c);
	}
	
	bool Lex::isLetter(const char c)
	{
		return isalpha(c);
	}
	
	bool Lex::matches(char& c, InputStream& stream, std::vector<Token>& tokens)
	{
		if(matches(c, stream))
		{
			std::cout << token.value << "\t \t \t " << token.type << std::endl;
			tokens.push_back(token);
			token.value = "";
			token.type = TokenTypes::UNDEFINED;
			return true;
		}
		return false;
	}
	
	bool StringLex::matches(char& c, InputStream& stream)
	{
		if(c == '\"')
		{
			token.type = TokenTypes::LITERAL;
			if(!stream.next(c))
				return false;
			
			while(c != '\"' && stream.hasNext())
			{
				token += c;
				stream.next(c);
			}
			stream.next(c);
			return true;
		}
		return false;
	}
	
	bool LiteralLex::matches(char& c, InputStream& stream)
	{
		if(isLetter(c))
		{
			token += c;
			//token.type = TokenType::LITERAL;
			if(!stream.next(c))
				return true;
			
			while((isLetter(c) || isDigit(c)) && stream.hasNext())
			{
				token += c;
				stream.next(c);		
			}
			return true;
		}
		return false;
	}

	bool CommentLex::matches(char& c, InputStream& stream)
	{		
		if(c == '#')
		{
			if(!stream.next(c))
				return false;
			
			while(c != '#' && stream.hasNext())
			{
				stream.next(c);
			}
			stream.next(c);
		}
		return false;
	}
	
	bool WhiteSpaceLex::matches(char& c, InputStream& stream)
	{
		while(isWhiteSpace(c) && stream.hasNext())
		{
			stream.next(c);
		}
		return false;
	}
	
	bool NumeralLex::matches(char& c, InputStream& stream)
	{
		if(isDigit(c))
		{
			bool hadDot = false;
			token+=c;
			token.type = TokenTypes::LITERAL;
			if(!stream.next(c))
				return true;
			
			while(isDigit(c) || (c == '.' && !hadDot))
			{
				if(c == '.')
				{
					hadDot = true;
				}
				token += c;
				
				if(!stream.next(c))
					return true;
			}
			
			return true;
		}
		return false;
	}
	
	bool SpecialCharLex::matches(char& c, InputStream& stream)
	{
		if(isSpecialChar(c))
		{
			if(isOperator(c))
			{
				token.type = TokenTypes::OPERATOR;
			}else if(isSeparator(c))
			{
				token.type = TokenTypes::SEPARATOR;
			}
			token+=c;
			stream.next(c);
			return true;
		}
		return false;
	}
	
	
	Lexer::Lexer(const std::string& fileName): stream(fileName)
	{
		rules.push_back(std::make_shared<StringLex>());
		rules.push_back(std::make_shared<LiteralLex>());
		rules.push_back(std::make_shared<NumeralLex>());
		rules.push_back(std::make_shared<SpecialCharLex>());
		rules.push_back(std::make_shared<CommentLex>());
		rules.push_back(std::make_shared<WhiteSpaceLex>());
	}
	
	void Lexer::getTokens(std::vector<Token>& tokens)
	{
		char c;
		stream.next(c);
		while(stream.hasNext())
		{	
			for(std::shared_ptr<Lex> lex : rules)
			{
				lex->matches(c, stream, tokens);			
			}
		}
	}
}