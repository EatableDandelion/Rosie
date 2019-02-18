#pragma once

#include <memory>
#include <vector>
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
		virtual bool matches(char& c, InputStream& stream, std::shared_ptr<Instruction>& instruction) = 0;
	};
	
	template<typename Caster>
	struct Lex : public Rule
	{
		virtual bool matches(char& c, InputStream& stream, std::shared_ptr<Instruction>& instruction)
		{
			if(matches(c, stream))
			{
				assignType();
				
				if(token.value == ")" || token.value == "," || token.value == ";")
				{
					std::shared_ptr<Instruction> nextInstruction = std::make_shared<Instruction>(instruction);
					instruction = instruction->next(nextInstruction);
				}
				else if(token.value == "(")
				{
					std::shared_ptr<Instruction> child = std::make_shared<Instruction>(instruction);
					instruction = instruction->sub(child);
				}
				else
				{
					std::cout << "adding " << token.value << " to " << instruction->id <<std::endl;
					instruction->addToken(token);
				}
				
				token.value = "";
				token.type = TokenTypes::UNDEFINED;
				return true;
			}
			return false;
		}
		
		virtual bool matches(char& c, InputStream& stream) = 0; //returns true if has token to stack, false otherwise
		
		protected:
			Caster typeCaster;
			Token token;
			
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
			
			
			void assignType()
			{
				typeCaster.assign(token);
			}
	};
		
	struct StringLex : Lex<ValueCaster> //VARVALUE string
	{
		virtual bool matches(char& c, InputStream& stream);
	};
	
	struct LiteralLex : Lex<LiteralCaster> //VARVALUE, VARTYPE, VARNAME or KEYWORD
	{
		virtual bool matches(char& c, InputStream& stream);
	};
	
	struct NumeralLex : Lex<ValueCaster> //VARVALUE number
	{
		virtual bool matches(char& c, InputStream& stream);
	};
	
	struct SpecialCharLex : Lex<SpecialCharCaster> //SEPARATOR or OPERATOR
	{
		virtual bool matches(char& c, InputStream& stream);
		
		private:
			bool isSpecialChar(const char c);
	};
	
	struct WhiteSpaceLex : Lex<VoidCaster> // white spaces
	{
		virtual bool matches(char& c, InputStream& stream);
	};
	
	struct CommentLex : Lex<VoidCaster> // comments
	{
		virtual bool matches(char& c, InputStream& stream);
	};
	
	class Lexer
	{
		public:
			Lexer(const std::string& fileName);
			
			void getTokens(std::shared_ptr<Instruction>& instruction);
			
		private:
			InputStream stream;
			std::vector<std::shared_ptr<Rule>> rules;
	};
}