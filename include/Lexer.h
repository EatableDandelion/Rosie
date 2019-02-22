#pragma once

#include <memory>
#include <vector>
#include <deque>
#include <string>
#include <fstream>
#include <iostream>
#include "InputStream.h"
#include "Token.h"
#include "Instruction.h"

namespace Rosie
{	

	struct Rule
	{
		virtual bool nextToken(char& c, InputStream& stream, Token& token) = 0;
	};
	
	template<typename Caster>
	struct Lex : public Rule
	{
		virtual bool nextToken(char& c, InputStream& stream, Token& token)
		{
			if(appendToToken(c, stream, token))
			{
				assignType(token);

				return true;
			}
			return false;
		}
		
		virtual bool appendToToken(char& c, InputStream& stream, Token& token) = 0; //returns true if has token to stack, false otherwise
		
		protected:
			Caster typeCaster;
			
			bool isWhiteSpace(const char c)
			{
				return c == ' ' || c == '\t' || c == '\n';
			}
			
			bool isDigit(const char c)
			{
				return isdigit(c);
			}
			
			bool isLetter(const char c)
			{
				return isalpha(c);
			}
		private:
			void assignType(Token& token)
			{
				typeCaster.assign(token);
			}
	};
		
	struct StringLex : Lex<ValueCaster> //VARVALUE string
	{
		virtual bool appendToToken(char& c, InputStream& stream, Token& token);
	};
	
	struct LiteralLex : Lex<LiteralCaster> //VARVALUE, VARTYPE, VARNAME or KEYWORD
	{
		virtual bool appendToToken(char& c, InputStream& stream, Token& token);
	};
	
	struct NumeralLex : Lex<ValueCaster> //VARVALUE number
	{
		virtual bool appendToToken(char& c, InputStream& stream, Token& token);
	};
	
	struct SpecialCharLex : Lex<SpecialCharCaster> //SEPARATOR or OPERATOR
	{
		virtual bool appendToToken(char& c, InputStream& stream, Token& token);
		
		private:
			bool isSpecialChar(const char c);
	};
	
	struct WhiteSpaceLex : Lex<VoidCaster> // white spaces
	{
		virtual bool appendToToken(char& c, InputStream& stream, Token& token);
	};
	
	struct CommentLex : Lex<VoidCaster> // comments
	{
		virtual bool appendToToken(char& c, InputStream& stream, Token& token);
	};
	
	class Lexer
	{
		public:
			Lexer(const std::string& fileName);
					
			void operator++(int);
			
			void operator+=(const int& nbIterations);
			
			Token getToken();
			
			bool hasNext();
		
			Token getToken(const int& index);
			
		private:
			LineStream lineStream;
			std::vector<std::shared_ptr<Rule>> rules;
			Token m_token;
			std::deque<Token> tokens;
			
			bool next();
			bool loadNextLine();
			bool nextToken(InputStream& charStream, Token& token);
	};
}
