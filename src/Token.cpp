#include "Token.h"

namespace Rosie
{
		
	
	/*
				void assignType(Token& token) const;
		
		private:
			void assignKeyword(Token& token) const;
			void assignOperator(Token& token) const;
			void assignSeparator(Token& token) const;
			void assignPrimitive(Token& token) const;
	*/
	
	int Token::length() const
	{
		return value.length();
	}
	
	void Token::operator+=(const char c)
	{
		value.push_back(c);
	}
}