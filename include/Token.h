#pragma once

#include <string>
#include <set>

namespace Rosie
{
	enum TokenTypes
	{
	/*0*/	KEYWORD,	//if, while, return
	/*1*/	SEPARATOR, 	//( ) ;
	/*2*/	OPERATOR,	//+ =
	/*3*/	COMPARATOR, //>, <, ==, &&
	/*4*/	VARNAME, 	//all the variables		
	/*5*/	VARTYPE,	//float, int, double, string, var
	/*6*/	VARVALUE,	//"a", 58.0, true
	/*7*/	UNDEFINED	//base value
	};
	
	/*enum Keywords
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
		STRING,	DOUBLE, INT, FLOAT, BOOLEAN, VAR
	};*/
	
	struct Token
	{
		Rosie::TokenTypes type = UNDEFINED;
		std::string value;
		
		void operator+=(const char c);
		
		int length() const;
	};
	
	class TypeCaster
	{
		public:
			virtual void assign(Token& token) = 0;

	};
	
	class SpecialCharCaster : public TypeCaster//Separator or operator
	{
		public:
			virtual void assign(Token& token);
			
			bool isSpecialChar(const char c) const;
			
		private:
			bool isOperator(const std::string& name) const;
			bool isSeparator(const std::string& name) const;
			bool isComparator(const std::string& name) const;
	};
	
	class LiteralCaster : public TypeCaster//Keyword, name, type or boolean value
	{
		public:
			LiteralCaster();
			
			virtual void assign(Token& token);
			
		private:
			bool isKeyword(const std::string& name);
			bool isType(const std::string& name);
			bool isValue(const std::string& name);
			std::set<std::string> types;
	};
	
	class ValueCaster : public TypeCaster//Value
	{
		public:
			virtual void assign(Token& token);
	};
	
	class VoidCaster : public TypeCaster //White spaces and comments
	{
		public:
			virtual void assign(Token& token);
	};
}
