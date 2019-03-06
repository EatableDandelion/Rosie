#pragma once

#include <string>
#include <fstream>
#include <iostream>

namespace Rosie
{

	class InputStream
	{
		public:
			InputStream(const std::string& line);
			
			bool next(char& ch);
			bool hasNext();
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

}