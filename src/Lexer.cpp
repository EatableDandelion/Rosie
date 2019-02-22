#include "Lexer.h"

namespace Rosie
{		
	bool StringLex::appendToToken(char& c, InputStream& stream, Token& token)
	{
		if(c == '\"')
		{
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
	
	bool LiteralLex::appendToToken(char& c, InputStream& stream, Token& token)
	{
		if(isLetter(c))
		{
			token += c;
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

	bool CommentLex::appendToToken(char& c, InputStream& stream, Token& token)
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
	
	bool WhiteSpaceLex::appendToToken(char& c, InputStream& stream, Token& token)
	{
		while(isWhiteSpace(c) && stream.hasNext())
		{
			stream.next(c);
		}
		return false;
	}
	
	bool NumeralLex::appendToToken(char& c, InputStream& stream, Token& token)
	{
		if(isDigit(c))
		{
			bool hadDot = false;
			token+=c;
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
	
	bool SpecialCharLex::appendToToken(char& c, InputStream& stream, Token& token)
	{
		if(isSpecialChar(c))
		{			
			token+=c;
			stream.next(c);
			return true;
		}
		return false;
	}
	
	bool SpecialCharLex::isSpecialChar(const char c)
	{
		return typeCaster.isSpecialChar(c);
	}
	
	
	Lexer::Lexer(const std::string& fileName): lineStream(fileName)
	{
		rules.push_back(std::make_shared<StringLex>());
		rules.push_back(std::make_shared<LiteralLex>());
		rules.push_back(std::make_shared<NumeralLex>());
		rules.push_back(std::make_shared<SpecialCharLex>());
		rules.push_back(std::make_shared<CommentLex>());
		rules.push_back(std::make_shared<WhiteSpaceLex>());
	}
	
	bool Lexer::next()
	{
		bool hasTokens = true;
		if(tokens.empty())
		{
			hasTokens = loadNextLine();
		}
		if(hasTokens)
		{
			m_token = token.pop();
		}
		return hasTokens;
		/*InputStream charStream;
		while(lineStream.nextLine())
		{
			charStream.setLine(lineStream.getLine());
			
			char c = charStream.getChar();
			Token token;
			while(charStream.hasNext())
			{
				for(std::shared_ptr<Rule> rule : rules)
				{
					if(rule->nextToken(c, charStream, token))
					{
						m_token = token;
						
					}
				}
			}
		}
		return false;*/
		
		/*
		char c = stream.getChar();
		Token token;
		while(stream.hasNext())
		{	
			for(std::shared_ptr<Rule> rule : rules)
			{
				if(rule->nextToken(c, stream, token))
				{
					m_token = token;
					return true;
				}
			}
		}
		return false;*/
	}
	
	bool Lexer::loadNextLine()
	{
		if(!lineStream.hasNextLine())return false;
		
		InputStream charStream(lineStream.getLine());
		
		char c = charStream.getChar();
		Token token;

		while(charStream.hasNext())
		{
			nextToken(c, stream, token);
		}
		
		return !tokens.empty();
	}
	
	bool Lexer::nextToken(char& c, InputStream& stream, Token& token)
	{
		for(std::shared_ptr<Rule> rule : rules)
		{
			if(rule->nextToken(c, charStream, token))
			{
				tokens.push(token);
				token.clear();
			}
		}
	}
	
	void Lexer::operator++(int)
	{
		next();
	}
			
	void Lexer::operator+=(const int& nbIterations)
	{
		for(int i = 0; i<nbIterations; i++)
		{
			next();
		}
	}
			
	bool Lexer::hasNext()
	{
		return lineStream.hasNextLine();
	}
	
	Token Lexer::getToken()
	{
		return m_token;
	}
}
