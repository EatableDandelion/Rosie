#include "InputStream.h"

namespace Rosie
{
	InputStream::InputStream(const std::string& line):m_line(line)
	{
		index = 0;
	}
	
	bool InputStream::next(char& ch)//increment read character and returns false if eof, true otherwise
	{
		if(hasNext())
		{
			index++;
			ch = m_line[index];
			return true;
		}
		else
		{	
			return false;
		}
	}
	
	char InputStream::getChar() const
	{
		return m_line[index];
	}
	
	bool InputStream::hasNext()
	{
		return index<m_line.size();
	}
	
	
	
	LineStream::LineStream(const std::string& fileName)
	{
		stream.open(fileName);
		lineIndex = 0;
	}
	
	LineStream::~LineStream()
	{
		stream.close();
	}
	
	bool LineStream::nextLine()
	{
		
		if(!hasNextLine())return false;
		std::getline(stream, line);
		lineIndex++;
		return true;
	}
	
	bool LineStream::hasNextLine()
	{
		return !stream.eof();
	}
	
	std::string LineStream::getLine() const
	{
		return line;
	}
	
	int LineStream::getLineIndex() const
	{
		return lineIndex;
	}
}