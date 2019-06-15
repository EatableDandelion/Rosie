#pragma once

#include <memory>
#include <vector>
#include <deque>
#include <string>
#include <fstream>
#include <iostream>
#include "InputStream.h"
#include "Token.h"
#include "InterpreterObjects.h"

namespace Rosie
{	

	struct Rule
	{
		virtual bool nextToken(char& c, FileStream& stream, Token& token) = 0;
	};
	
	template<typename Caster>
	struct Lex : public Rule
	{
		virtual bool nextToken(char& c, FileStream& stream, Token& token)
		{
			if(appendToToken(c, stream, token))
			{
				assignType(token);

				return true;
			}
			return false;
		}
		
		/** return true if finish token with character, false to continue appending on current token */
		virtual bool appendToToken(char& c, FileStream& stream, Token& token) = 0;
		
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
		
	struct StringLex : Lex<StringCaster> //CSTSTRING
	{
		virtual bool appendToToken(char& c, FileStream& stream, Token& token);
	};
	
	struct LiteralLex : Lex<LiteralCaster> //CSTBOOLEAN, VARTYPE, VARNAME or KEYWORD
	{
		virtual bool appendToToken(char& c, FileStream& stream, Token& token);
	};
	
	struct NumeralLex : Lex<VoidCaster> //CSTFLOAT, CSTINT number
	{
		virtual bool appendToToken(char& c, FileStream& stream, Token& token);
	};
	
	struct SpecialCharLex : Lex<SpecialCharCaster> //SEPARATOR or OPERATOR
	{
		virtual bool appendToToken(char& c, FileStream& stream, Token& token);
		
		private:
			bool isSpecialChar(const char c);
	};
	
	struct WhiteSpaceLex : Lex<VoidCaster> // white spaces
	{
		virtual bool appendToToken(char& c, FileStream& stream, Token& token);
	};
	
	struct CommentLex : Lex<VoidCaster> // comments
	{
		virtual bool appendToToken(char& c, FileStream& stream, Token& token);
	
		
		private:
			bool runMultiLineComment(char& c, FileStream& stream);
	};

	class Lexer
	{
		public:
			Lexer(const std::string& fileName);
					
			void operator++(int);
			
			void operator+=(const int& nbIterations);
			
			Token getToken() const;
			
			bool hasNext();
		
			bool peekToken(Token& token, const int& index);
			
			int getLineIndex() const;
			
			std::string getLine() const;
			
		private:
			FileStream fileStream;
			std::vector<std::shared_ptr<Rule>> rules;
			std::deque<Token> tokens;
			bool m_hasNext;
			
			bool next();
			bool loadNextLine();
			bool nextToken(Token& token);
	};
}
