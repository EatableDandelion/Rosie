#include "Objects.h"

namespace Rosie
{
	void Token::setAddress(const Address& address)
	{
		m_address = address;
	}
	
	Address Token::getAddress() const
	{
		return m_address;
	}
	
	int Token::length() const
	{
		return value.length();
	} 
	
	void Token::operator+=(const char c)
	{
		value.push_back(c);
	}
	
	bool Token::operator==(const std::string& stringValue)
	{
		return value == stringValue;
	}
	
	bool Token::operator!=(const std::string& stringValue)
	{
		return value != stringValue;
	}
	
	void Token::clear()
	{
		type = UNDEFINED;
		value = "";
	}
	
	Address::Address(const std::size_t& location, const std::string& name, const std::size_t& type):id(location), name(name), type(type)
	{}
	
	Address::Address(const Address& address):id(address.id), name(address.name), type(address.type)
	{}
	
	Address::Address():id(0), name(""), type(0)
	{}
	
	Variable::Variable(const float& floatValue):type(0)
	{
		//f = floatValue;
		value = floatValue;
		type = 0;
	}

	Variable::Variable(const int& integerValue):type(1)
	{
		//i = integerValue;
		value = integerValue;
		type = 1;
	}
	
	Variable::Variable(const bool& booleanValue):type(2)
	{
		//b = booleanValue;
		value = booleanValue;
		type = 2;
	}
	
	Variable::Variable()
	{}
	
	Variable Variable::operator+(const Variable& other)
	{
		return performOperation(other, '+');
	}
	
	Variable Variable::performOperation(const Variable& other, const char c)
	{
		if(type != other.type)error;
		if(type == 0)
		{
			return performOperationOnType<float>(other, c);
		}
		else if(type == 1)
		{
			return performOperationOnType<int>(other, c);
		}
		else if(type == 2)
		{
			return performOperationOnType<bool>(other, c);
		}
		else(type == 3)
		{
			return performOperationOnType<std::string>(other, c);
		}
	}
}
