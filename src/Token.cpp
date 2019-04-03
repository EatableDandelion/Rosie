#include "Token.h"

namespace Rosie
{
	
	bool SpecialCharCaster::isSpecialChar(const char c) const
	{
		std::string s;
		s.push_back(c);
		return isOperator(s) || isSeparator(s) || isComparator(s);
	}
	
	bool SpecialCharCaster::isOperator(const std::string& c) const
	{
		return 
		c == "=" || c == "+" || c == "-" || c == "*" ||
		c == "/" || c == "^" || c == "%";
	}
	
	bool SpecialCharCaster::isSeparator(const std::string& c) const
	{
		return 
		c == "," || c == ";" || c == "(" || 
		c == ")" ||	c == "{" || c == "}";
	}
	
	bool SpecialCharCaster::isComparator(const std::string& c) const
	{
		return 
		c == "<" || c == ">" || c == "&&" ||
		c == "||"|| c == "=="|| c == ">=" ||
		c == "<=";
	}

	void SpecialCharCaster::assign(Token& token)
	{
		if(isOperator(token.value))
		{
			token.type = TokenType::OPERATOR;
		}
		else if(isSeparator(token.value))
		{
			token.type = TokenType::SEPARATOR;
		}
		else if(isComparator(token.value))
		{
			token.type = TokenType::COMPARATOR;
		}
	}

	LiteralCaster::LiteralCaster()
	{
		types.insert("int");
		types.insert("float");
		types.insert("double");
		types.insert("boolean");
		types.insert("string");
		types.insert("var");
		types.insert("function");
	}
	
	void LiteralCaster::assign(Token& token)
	{
		if(isKeyword(token.value))
		{
			token.type = TokenType::KEYWORD;
		}
		else if(isType(token.value))
		{
			token.type = TokenType::VARTYPE;
		}
		else if(isBoolean(token.value))
		{
			token.type = TokenType::CSTBOOLEAN;
		}
		else
		{
			token.type = TokenType::VARNAME;
		}
	}
	
	bool LiteralCaster::isKeyword(const std::string& name)
	{
		return
		name == "if" || name == "while" || name == "for" || 
		name == "return" || name == "define";
	}
	
	bool LiteralCaster::isType(const std::string& name)
	{
		if(types.find(name) != types.end())
		{
			return true;
		}
		return false;
	}
	
	bool LiteralCaster::isBoolean(const std::string& name)
	{
		return name == "true" || name == "false";
	}
	
	void StringCaster::assign(Token& token)
	{
		token.type = TokenType::CSTSTRING;
	}
	
	void VoidCaster::assign(Token& token)
	{}
	
}