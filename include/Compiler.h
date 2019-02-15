#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

namespace Rosie{
	
	enum TokenType{
		IDENTIFIER, //all the variables
		KEYWORD,	//if, while, return
		SEPARATOR, 	//( ) ;
		OPERATOR,	//+ =
		LITERAL		//"a", 58.0, true
	};
	
	struct Token
	{
		Rosie::TokenType type;
		std::string value;
		
		void operator+=(const char c);
		
		int length() const;
	};
	
	class InputStream
	{
		public:
			InputStream(const std::string& fileName);
			~InputStream();
			
			char next();
			char peek();
			bool hasNext();
			char getCurrentChar();
		
		private:
			char c;
			std::ifstream stream;
	};
	
	struct Lex
	{
		
		bool matches(InputStream& stream, std::vector<Token>& tokens);
		
		virtual bool matches(InputStream& stream) = 0;
		
		virtual void reset();
		
		virtual void resetChild(){};
		
		bool addChar(InputStream& stream); //return true if can continue, false if eof
		
		protected:
			Token token;
			bool isWhiteSpace(const char c);
			bool isSpecialChar(const char c);
			bool isDigit(const char c);
			bool isLetter(const char c);
			bool wasMatch = false;
	};
	
	struct StringLex : Lex //IDENTIFIER or KEYWORD or LITERAL
	{
		virtual bool matches(InputStream& stream);
		
		virtual void resetChild();
		
		private:
			bool inQuote = false;
	};
	
	struct LiteralLex : Lex //LITERAL string
	{
		virtual bool matches(InputStream& stream);
	};
	
	struct NumeralLex : Lex //LITERAL number
	{
		virtual bool matches(InputStream& stream);
		
		virtual void resetChild();
		
		private:
			bool hadDot = false;
	};
	
	struct SpecialCharLex : Lex // SEPARATOR or OPERATOR
	{
		virtual bool matches(InputStream& stream);
	};
	
	struct IgnoreLex : Lex // white spaces and comments
	{
		virtual bool matches(InputStream& stream);
		
		virtual void resetChild();
		
		private:
			bool commented = false;
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
	
	struct Variable
	{
		Variable(const std::string& type, const std::string& name, const std::string& value);
		std::string m_type;
		std::string m_name;
		std::string m_value;
	};
	
	class Compiler
	{
		public:
			Compiler();
			
			void read(const std::string& fileName);
			

		private:
			std::unordered_map<std::string, Variable> variables;
			
			void parseLine(const std::string& line);
			
			bool containsString(const std::string& line, const std::string& subline);
			
			std::string removeWhiteChars(const std::string& line);
			
			std::vector<std::string> tokenize(const std::string& line);
			
			int getType(const std::string& line); //0 = comment, 1 = assignement, 2 = instruction
	
			void parseAssigment(const std::vector<std::string>& tokens);
	};
	

}
