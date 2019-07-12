#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "Utils.h"

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
			int getCharIndex() const;
		
		private:
			std::string m_line;
			int index;
			
	};
	
	class LineStream
	{
		public:
			LineStream(const std::string& fileName);
			~LineStream();
			
			void openFile(const std::string& fileName);
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
			std::string getFileName() const;
			int getCharIndex() const;
			
		private:
			CharStream charStream;
			LineStream lineStream;
			const std::string fileName;
	};
	
	
	class FileError : public std::exception
	{
		public:
			FileError(const std::string& fileName);
			
			const char* what() const throw();
		
		private:
			const std::string fileName;
	};

}