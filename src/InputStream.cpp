#include "InputStream.h"

namespace Rosie
{
	InputStream::InputStream(const std::string& fileName)
	{
		stream.open(fileName);
	}
	
	InputStream::~InputStream()
	{
		stream.close();
	}
	
	bool InputStream::next(char& ch)//increment read character and returns false if eof, true otherwise
	{
		if(hasNext())
		{
			stream.get(c);
			ch = c;
			return true;
		}
		else
		{	
			return false;
		}
	}
	
	char InputStream::getChar() const
	{
		return c;
	}
	
	char InputStream::peek()
	{
		return stream.peek();
	}
	
	bool InputStream::hasNext()
	{
		return !stream.eof();
	}
}