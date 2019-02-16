#pragma once

#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>

namespace Rosie
{
	
	enum TokenType{
		IDENTIFIER, //all the variables
		KEYWORD,	//if, while, return
		SEPARATOR, 	//( ) ;
		OPERATOR,	//+ =
		LITERAL,	//"a", 58.0, true
		UNDEFINED	//base value
	};
	
	struct Token
	{
		Rosie::TokenType type = UNDEFINED;
		std::string value;
		
		void operator+=(const char c);
		
		int length() const;
	};
	
	class InputStream
	{
		public:
			InputStream(const std::string& fileName);
			~InputStream();
			
			bool next(char& ch);
			char peek();
			bool hasNext();
			char getChar() const;
		
		private:
			char c;
			std::ifstream stream;
	};
	
	struct Lex
	{
		
		bool matches(char& c, InputStream& stream, std::vector<Token>& tokens);
		
		virtual bool matches(char& c, InputStream& stream) = 0; //returns true if has token to stack, false otherwise
		
		protected:
			Token token;
			bool isWhiteSpace(const char c);
			bool isSpecialChar(const char c);
			bool isOperator(const char c);
			bool isSeparator(const char c);
			bool isDigit(const char c);
			bool isLetter(const char c);
			bool wasMatch = false;
	};
	
	struct StringLex : Lex //IDENTIFIER or KEYWORD or LITERAL
	{
		virtual bool matches(char& c, InputStream& stream);
	};
	
	struct LiteralLex : Lex //LITERAL string
	{
		virtual bool matches(char& c, InputStream& stream);
	};
	
	struct NumeralLex : Lex //LITERAL number
	{
		virtual bool matches(char& c, InputStream& stream);
	};
	
	struct SpecialCharLex : Lex // SEPARATOR or OPERATOR
	{
		virtual bool matches(char& c, InputStream& stream);
	};
	
	struct WhiteSpaceLex : Lex // white spaces
	{
		virtual bool matches(char& c, InputStream& stream);
	};
	
	struct CommentLex : Lex // comments
	{
		virtual bool matches(char& c, InputStream& stream);
	};
	
	class Lexer
	{
		public:
			Lexer(const std::string& fileName);
			
			void getTokens(std::vector<Token>& tokens);
			
		private:
			InputStream stream;
			std::vector<std::shared_ptr<Lex>> rules;
	};
}