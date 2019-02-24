#include "Token.h"

namespace Rosie
{
	int Token::length() const
	{
		return value.length();
	} 
	
	void Token::operator+=(const char c)
	{
		value.push_back(c);
	}
	
	void Token::clear()
	{
		type = UNDEFINED;
		value = "";
	}
	
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
			token.type = TokenTypes::OPERATOR;
		}
		else if(isSeparator(token.value))
		{
			token.type = TokenTypes::SEPARATOR;
		}
		else if(isComparator(token.value))
		{
			token.type = TokenTypes::COMPARATOR;
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
	}
	
	void LiteralCaster::assign(Token& token)
	{
		if(isKeyword(token.value))
		{
			token.type = TokenTypes::KEYWORD;
		}
		else if(isType(token.value))
		{
			token.type = TokenTypes::VARTYPE;
		}
		else if(isValue(token.value))
		{
			token.type = TokenTypes::VARVALUE;
		}
		else
		{
			token.type = TokenTypes::VARNAME;
		}
	}
	
	bool LiteralCaster::isKeyword(const std::string& name)
	{
		return
		name == "if" || name == "while" || name == "for" || 
		name == "return";
	}
	
	bool LiteralCaster::isType(const std::string& name)
	{
		if(types.find(name) != types.end())
		{
			return true;
		}
		return false;
	}
	
	bool LiteralCaster::isValue(const std::string& name)
	{
		return
		name == "true" || name == "false";
	}

	void ValueCaster::assign(Token& token)
	{
		token.type = TokenTypes::VARVALUE;
	}
	
	void VoidCaster::assign(Token& token)
	{}
	
}