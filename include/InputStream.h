#pragma once

#include <string>
#include <fstream>
#include <iostream>

namespace Rosie
{

	class CharStream
	{
		public:
			CharStream();
			CharStream(const std::string& line);
			
			bool nextChar(char& ch);
			bool hasNextChar();
			char getChar() const;
		
		private:
			std::string m_line;
			int index;
			
	};
	
	class LineStream
	{
		public:
			LineStream(const std::string& fileName);
			~LineStream();
			
			bool nextLine();
			bool hasNextLine();
			std::string getLine() const;
			int getLineIndex() const;
		
		private:
			std::ifstream stream;
			std::string line;
			int lineIndex;
	};
	
	class FileStream
	{
		public:
			FileStream(const std::string& fileName);
			
			bool nextChar(char& ch);
			bool hasNextChar();
			char getChar() const;
			
			bool nextLine();
			bool hasNextLine();
			std::string getLine() const;
			int getLineIndex() const;
			
		private:
			CharStream charStream;
			LineStream lineStream;
	};

}