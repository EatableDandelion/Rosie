#pragma once

#include <string>

namespace Rosie
{
	enum TokenTypes{
		IDENTIFIER, //all the variables
		KEYWORD,	//if, while, return
		SEPARATOR, 	//( ) ;
		OPERATOR,	//+ =
		LITERAL,	//"a", 58.0, true
		UNDEFINED	//base value
	};
	
	enum Keywords
	{
		IF, WHILE, FOR, RETURN
	};
	
	enum Operators
	{
		EQUAL,		LESSTHAN,	GREATERTHAN,
		PLUS,		MINUS,		MULTIPLY,
		DIVIDE,		POWER,		MODULO
	};
	
	enum Separators
	{
		COMMA,	SEMICOLON,		DOT,
		COLON,	LPARENTHESIS,	RPARENTHESIS,
		LBKT,	RBKT
	};
	
	enum Primitives
	{
		STRING,	DOUBLE, INT, FLOAT, BOOLEAN
	};
	
	struct Token
	{
		Rosie::TokenTypes type = UNDEFINED;
		std::string value;
		
		void operator+=(const char c);
		
		int length() const;
	};
	
	/*class TypeCaster
	{
		public:
			void assignType(Token& token) const;
		
		private:
			void assignKeyword(Token& token) const;
			void assignOperator(Token& token) const;
			void assignSeparator(Token& token) const;
			void assignPrimitive(Token& token) const;
	};*/
}
