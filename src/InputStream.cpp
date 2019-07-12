#include "InputStream.h"

namespace Rosie
{
	CharStream::CharStream()
	{}
	
	CharStream::CharStream(const std::string& line):m_line(line)
	{
		index = 0;
	}
	
	bool CharStream::nextChar(char& ch)//increment read character and returns false if eof, true otherwise
	{
		if(hasNextChar())
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
	
	char CharStream::getChar() const
	{
		return m_line[index];
	}
	
	bool CharStream::hasNextChar()
	{
		return index<m_line.size();
	}
	
	int CharStream::getCharIndex() const
	{
		return index;
	}
	
	LineStream::LineStream(const std::string& fileName)
	{
		openFile(fileName);
		lineIndex = 0;
	}
	
	LineStream::~LineStream()
	{
		stream.close();
	}
	
	void LineStream::openFile(const std::string& fileName)
	{
		stream.open(fileName);
		if(stream.fail())
		{
			throw FileError(fileName);
		}
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
	
	
	
	FileStream::FileStream(const std::string& fileName):fileName(fileName), lineStream(fileName)
	{}
	
	bool FileStream::nextChar(char& ch)
	{
		return charStream.nextChar(ch);
	}
	
	char FileStream::getChar() const
	{
		return charStream.getChar();
	}
	
	bool FileStream::hasNextChar()
	{
		return charStream.hasNextChar();
	}
	
	bool FileStream::nextLine()
	{
		bool res = lineStream.nextLine();
		if(res)
		{
			charStream = CharStream(lineStream.getLine());
		}
		return res;
	}
	
	bool FileStream::hasNextLine()
	{
		return lineStream.hasNextLine();
	}
	
	std::string FileStream::getLine() const
	{
		return lineStream.getLine();
	}
	
	int FileStream::getLineIndex() const
	{
		return lineStream.getLineIndex();
	}
	
	std::string FileStream::getFileName() const
	{
		return fileName;
	}
	
	int FileStream::getCharIndex() const
	{
		return charStream.getCharIndex();
	}
	
	
	FileError::FileError(const std::string& fileName):fileName(fileName)
	{}
	
	const char*  FileError::what() const throw()
	{
		ErrorDisplay display;
		display.show("File \"" + fileName + "\" not found.");
		return "";
	}
	
}