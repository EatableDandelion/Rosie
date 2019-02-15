#include "Compiler.h"

namespace Rosie{
	
	InputStream::InputStream(const std::string& fileName)
	{
		stream.open(fileName);
	}
	
	InputStream::~InputStream()
	{
		stream.close();
	}
	
	char InputStream::next()
	{
		stream.get(c);
		return c;
	}
	
	char InputStream::getChar()
	{
		return c;
	}
	
	char InputStream::peek()
	{
		return stream.peek();
	}
	
	bool InputStream::hasNext()
	{
		return !stream.eof();
	}
	
	int Token::length() const
	{
		return value.length();
	}
	
	void Token::operator+=(const char c)
	{
		value.push_back(c);
	}
	
	bool Lex::isWhiteSpace(const char c)
	{
		return c == ' ' || c == '\t' || c == '\n';
	}
	
	bool Lex::isSpecialChar(const char c)
	{
		return 
		c == ',' || c == ';' || c == '.' || 
		c == ':' || c == '=' || c == '<' || 
		c == '>' || c == '(' || c == ')' || 
		c == '\"' || c == '+' || c == '-'|| 
		c == '*';
	}
	
	bool Lex::isDigit(const char c)
	{
		return isdigit(c);
	}
	
	bool Lex::isLetter(const char c)
	{
		return isalpha(c);
	}
	
	bool Lex::matches(InputStream& stream, std::vector<Token>& tokens)
	{
		bool match = matches(stream);
		if(!match && wasMatch && token.length()>0)
		{
			std::cout << token.value << std::endl;
			tokens.push_back(token);
			reset();
		}

		wasMatch = match;
		return match;
	}
	
	void Lex::reset()
	{
		token = Token();
		wasMatch = false;
		resetChild();
	}
	
	bool Lex::addChar(InputStream& stream)
	{
		token+=stream.getCurrentChar();
		if(stream.hasNext())
		{
			stream.next();
			return true;
		}
		return false;
	}
	
	/*bool StringLex::matches(InputStream& stream)
	{
		if(c == '\"')
		{
			if(inQuote)
			{
				inQuote = false;
			}
			else
			{
				inQuote = true;				
			}
			return true;
		}
		if(inQuote)
		{
			token+=c;
			return true;
		}
		else
		{
			return false;
		}
	}*/
	
	bool StringLex::matches(InputStream& stream)
	{
		char c = stream.getChar();
		if(c == '\"')
		{
			if(!stream.hasNext())
			{
				return false;
			}
			c = stream.next();
			
			while(c != '\"')
			{
				token += c;
				if(!stream.hasNext())
				{
					return true;
				}
				else
				{
					c = stream.next();
				}
			}
			return true;
		}
		return false;
	}
	
	void StringLex::resetChild()
	{
		inQuote = false;
	}
	
	/*bool LiteralLex::matches(InputStream& stream)
	{
		if(isDigit(c))
		{
			if(token.length()>0){
				token+=c;
				return true;
			}
			return false;
		}
		
		if(isSpecialChar(c) || isWhiteSpace(c))
		{
			return false;
		}
		token+=c;
		return true;
	}*/
	
	bool LiteralLex::matches(InputStream& stream)
	{
		char c = stream.getChar();
		if(isLetter(c))
		{
			token += c;
			if(!stream.hasNext())
			{
				return true;
			}
			else
			{
				c = stream.next();
			}
			
			while(isLetter(c) || isDigit(c))
			{
				token += c;
				
				if(!stream.hasNext())
				{
					return true;
				}
				else
				{
					c = stream.next();
				}	
			}
			return true;
		}
		return false;
	}
	
	bool IgnoreLex::matches(InputStream& stream)
	{
		if(isWhiteSpace(c))
		{
			return true;
		}
		if(c == '#')
		{
			commented = !commented;
			return true;
		}
		if(commented)
		{
			return true;
		}
		return false;
	}
	
	void IgnoreLex::resetChild()
	{
		commented = false;
	}
	
	bool NumeralLex::matches(InputStream& stream)
	{
		if(isDigit(c))
		{
			token+=c;
			return true;
		}
		if(c == '.' && !hadDot)
		{
			hadDot = true;
			token+=c;
			return true;
		}
		return false;
	}
	
	void NumeralLex::resetChild()
	{
		hadDot = false;
	}
	
	bool SpecialCharLex::matches(InputStream& stream)
	{//return true if continue, false otherwise
		if(isSpecialChar(c))
		{
			token+=c;
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
		rules.push_back(std::make_shared<IgnoreLex>());
	}
	
	void Lexer::getTokens(std::vector<Token>& tokens)
	{
		char c = stream.next();
		while(stream.hasNext())
		{	
			for(std::shared_ptr<Lex> lex : rules)
			{
				while(lex->matches(c, tokens) && stream.hasNext())
				{
					c = stream.next();	
				}				
			}
		}
	}
	
	Variable::Variable(const std::string& type, const std::string& name, const std::string& value):m_type(type), m_name(name), m_value(value)
	{}
	
	Compiler::Compiler()	
	{}
	
	void Compiler::read(const std::string& fileName)
	{
		Lexer stream(fileName);
		std::vector<Token> tokens;
		stream.getTokens(tokens);
	}
	
	
	
	bool Compiler::containsString(const std::string& line, const std::string& subline)
	{
		return line.find(subline) != std::string::npos;
	}
	
	std::vector<std::string> Compiler::tokenize(const std::string& line)
	{	
		std::vector<std::string> res;
		std::string token;
		bool inQuote = false;
		for(const char& c : line)
		{
			if(c=='\"'){
				
				inQuote = !inQuote;
				continue;
				
			}
			
			if(inQuote){
				
				token.push_back(c);
				continue;
				
			}
			
			if(c=='\t' || c==' '){
	
				if(token.length()>0){
					res.push_back(token);
					token="";
				}
				
			}else if(c=='=' || c==',' || c=='(' || c==')' || c==';'){
				
				if(token.length()>0){
					res.push_back(token);
					token="";
				}
				token="";
				res.push_back(std::string(1,c));
				
			}else{
				
				token.push_back(c);
				
			}
		}
		res.push_back(token);
		
		return res;
	}

	

}
