#pragma once

#include <string>
#include <fstream>
#include <iostream>

namespace Rosie
{

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

}