#include "Lexer.h"

namespace Rosie
{		
	bool StringLex::matches(char& c, InputStream& stream)
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
	
	bool LiteralLex::matches(char& c, InputStream& stream)
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
	
	
	Lexer::Lexer(const std::string& fileName): stream(fileName)
	{
		rules.push_back(std::make_shared<StringLex>());
		rules.push_back(std::make_shared<LiteralLex>());
		rules.push_back(std::make_shared<NumeralLex>());
		rules.push_back(std::make_shared<SpecialCharLex>());
		rules.push_back(std::make_shared<CommentLex>());
		rules.push_back(std::make_shared<WhiteSpaceLex>());
	}
	
	void Lexer::getTokens(std::shared_ptr<Instruction>& instruction)
	{
		char c;
		stream.next(c);
		while(stream.hasNext())
		{	
			for(std::shared_ptr<Rule> rule : rules)
			{
				rule->matches(c, stream, instruction);			
			}
		}
	}
}