#include "Objects.h"

namespace Rosie
{
	
	Type::Type(const std::string& name, const std::size_t& size, const int& id):name(name), size(size), id(id)
	{}
	
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
		set(floatValue);
	}

	Variable::Variable(const int& integerValue):type(1)
	{
		set(integerValue);
	}
	
	Variable::Variable(const bool& booleanValue):type(2)
	{
		set(booleanValue);
	}
	
	Variable::Variable(const std::string& stringValue):type(3)
	{
		set(stringValue);
	}
	
	Variable::Variable()
	{}
	
	void Variable::set(const float& newValue)
	{
		value = newValue;
		type = 0;
	}
	
	void Variable::set(const int& newValue)
	{
		value = newValue;
		type = 1;
	}
	
	void Variable::set(const bool& newValue)
	{
		value = newValue;
		type = 2;
	}
	
	void Variable::set(const std::string& newValue)
	{
		value = newValue;
		type = 3;
	}
	
	void Variable::set(const Variable& other)
	{
		value = other.value;
		type = other.type;
	}
	
	void State::addVariable(const int& id)
	{
		variables.insert(std::pair<int, Variable>(id, Variable()));
	}
	
	void State::addConstants(const std::vector<Variable>& csts)
	{
		constants = csts;
	}
	
	void State::push(const Variable& variable)
	{
		callStack.push(variable);
	}
	
	void State::push(const Address& address)
	{
		push(getVariable(address));
	}
	
	Variable State::pop()
	{
		Variable res = callStack.top();
		callStack.pop();
		return res;
	}
	
	bool State::empty() const
	{
		return callStack.empty();
	}
	
	void State::copyVariable(Address& dest, const Address& src)
	{
		variables[dest.id] = getVariable(src);
	}
	
	Variable State::getVariable(const Address& address)
	{
		if(address.type == 0)
		{
			return constants[address.id];			
		}
		else if(address.type == 1)
		{
			if(address.id == 0)
			{
				return pop();
			}
			else
			{
				if(variables.find(address.id) == variables.end())
				{
					variables.insert(std::pair<int, Variable>(address.id, Variable()));
				}
				return variables[address.id];				
			}
		}
		else //type = 2
		{
			return Variable(static_cast<int>(address.id));
		}
	}
}
