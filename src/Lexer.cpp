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
			token.type = TokenType::CSTINT;
			token+=c;
			if(!stream.next(c))
				return true;
			
			while(isDigit(c) || (c == '.' && !hadDot))
			{
				if(c == '.')
				{
					hadDot = true;
					token.type = TokenType::CSTFLOAT;
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
		m_hasNext = true;
		//loadNextLine();
		next();
	}
	
	bool Lexer::next()
	{
		tokens.pop_front();
		if(tokens.empty())
		{	
			if(!lineStream.nextLine())return false;
			while(!loadNextLine())
			{
				if(!lineStream.nextLine())return false;
			}
		}
		
		return true;
	}
	
	bool Lexer::loadNextLine()
	{
		//if(!lineStream.nextLine())return false;
		
		InputStream charStream(lineStream.getLine());
		
		Token token;

		while(charStream.hasNext())
		{
			if(nextToken(charStream, token))
			{
				tokens.push_back(token);
				token.clear();				
			}
		}

		return !tokens.empty();
	}
	
	bool Lexer::nextToken(InputStream& charStream, Token& token)
	{
		char c = charStream.getChar();

		for(std::shared_ptr<Rule> rule : rules)
		{
			if(rule->nextToken(c, charStream, token))
			{
				return true;
			}
		}
		return false;
	}
	
	void Lexer::operator++(int)
	{
		m_hasNext = next();
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
		return m_hasNext;
	}
	
	Token Lexer::getToken() const
	{
		return tokens.front();
	}
	
	bool Lexer::peekToken(Token& token, const int& index)
	{
		if(index >= tokens.size())return false;
		token = tokens[index];
		return true;
	}
	
	int Lexer::getLineIndex() const
	{
		return lineStream.getLineIndex();
	}
	
	std::string Lexer::getLine() const
	{
		return lineStream.getLine();
	}
}
