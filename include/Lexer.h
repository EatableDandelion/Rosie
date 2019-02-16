#pragma once

#include "InputStream.h"
#include "Token.h"
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace Rosie
{	
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