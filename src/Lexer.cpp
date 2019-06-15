#include "Lexer.h"

namespace Rosie
{		
	bool StringLex::appendToToken(char& c, FileStream& stream, Token& token)
	{
		if(c == '\"')
		{
			if(!stream.nextChar(c))
				return false;
			
			while(c != '\"' && stream.hasNextChar())
			{
				token += c;
				stream.nextChar(c);
			}
			stream.nextChar(c);
			return true;
		}
		return false;
	}
	
	bool LiteralLex::appendToToken(char& c, FileStream& stream, Token& token)
	{
		if(isLetter(c))
		{
			token += c;
			if(!stream.nextChar(c))
				return true;
			
			while((isLetter(c) || isDigit(c)) && stream.hasNextChar())
			{
				token += c;
				stream.nextChar(c);		
			}
			return true;
		}
		return false;
	}

	bool CommentLex::appendToToken(char& c, FileStream& stream, Token& token)
	{	
		if(c == '#')
		{
			if(!stream.nextChar(c))
				return false;
			
			//If ## then it is the start of a multi-line comment.
			if(c == '#')
			{
				stream.nextChar(c);
				return runMultiLineComment(c, stream);
			}
			else //otherwise it is an inline comment.
			{
				while(stream.hasNextChar())
				{std::cout << c;
					stream.nextChar(c);
				}
				stream.nextChar(c);
				return false;
			}
		}
		return false;
	}
	
	bool CommentLex::runMultiLineComment(char& c, FileStream& stream)
	{
		while(stream.hasNextChar())
		{
			if(c != '#')
			{
				//Append char to comment
				std::cout << c;
				stream.nextChar(c);
			}
			else
			{
				//Finish comment;
				std::cout << std::endl;
				return false;
			}					
		}
		stream.nextChar(c);
		
		if(!stream.nextLine())
		{
			return false;
		}
		
		runMultiLineComment(c, stream);
		stream.nextChar(c);
		
		return true;
	}
	
	bool WhiteSpaceLex::appendToToken(char& c, FileStream& stream, Token& token)
	{
		while(isWhiteSpace(c) && stream.hasNextChar())
		{
			stream.nextChar(c);
		}
		return false;
	}
	
	bool NumeralLex::appendToToken(char& c, FileStream& stream, Token& token)
	{
		if(isDigit(c))
		{
			bool hadDot = false;
			token.type = TokenType::CSTINT;
			token+=c;
			if(!stream.nextChar(c))
				return true;
			
			while(isDigit(c) || (c == '.' && !hadDot))
			{
				if(c == '.')
				{
					hadDot = true;
					token.type = TokenType::CSTFLOAT;
				}
				token += c;
				
				if(!stream.nextChar(c))
					return true;
			}
			
			return true;
		}
		return false;
	}
	
	bool SpecialCharLex::appendToToken(char& c, FileStream& stream, Token& token)
	{
		if(isSpecialChar(c))
		{			
			token+=c;
			stream.nextChar(c);
			return true;
		}
		return false;
	}
	
	bool SpecialCharLex::isSpecialChar(const char c)
	{
		return typeCaster.isSpecialChar(c);
	}
	
	
	Lexer::Lexer(const std::string& fileName): fileStream(fileName)
	{
		rules.push_back(std::make_shared<StringLex>());
		rules.push_back(std::make_shared<LiteralLex>());
		rules.push_back(std::make_shared<CommentLex>());
		rules.push_back(std::make_shared<NumeralLex>());
		rules.push_back(std::make_shared<SpecialCharLex>());	
		rules.push_back(std::make_shared<WhiteSpaceLex>());
		
		m_hasNext = true;
		loadNextLine();
	}
	
	bool Lexer::next() //returns true if has next line, false otherwise.
	{
		tokens.pop_front();
		if(tokens.empty())
		{	
			return loadNextLine();
		}
		
		return true;
	}
	
	bool Lexer::loadNextLine() //returns false if eof, true otherwise
	{
		while(tokens.empty())
		{
			if(!fileStream.nextLine())
			{
				return false;
			}
			
			Token token;
			while(fileStream.hasNextChar())
			{
				if(nextToken(token))
				{
					tokens.push_back(token);
					token.clear();				
				}
			}
		}
		return true;
	}
	
	bool Lexer::nextToken(Token& token)
	{
		char c = fileStream.getChar();

		for(std::shared_ptr<Rule> rule : rules)
		{
			if(rule->nextToken(c, fileStream, token))
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
		return fileStream.getLineIndex();
	}
	
	std::string Lexer::getLine() const
	{
		return fileStream.getLine();
	}
}
