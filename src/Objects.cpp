#include "Objects.h"

namespace Rosie
{
	std::size_t getId(const std::string& name)
	{
		return std::hash<std::string>{}(name);
	}
	
	Type::Type(const std::string& name):name(name)
	{}
	
	Type::Type()
	{}
	
	void Type::setId(const std::size_t& typeId)
	{
		id = typeId;
	}
	
	void Type::addMember(const int& id, const std::string& name)
	{
		members.insert(std::pair<std::size_t, int>(Rosie::getId(name), id));
	}
	
	std::size_t Type::getSize() const
	{
		return size;
	}
	
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
	
	
	Address::Address(const std::size_t& id, const int& category, const std::string& name):id(id), category(category), name(name)
	{}
	
	Address::Address(const Address& address):id(address.id), name(address.name), category(address.category)
	{}
	
	Address::Address():id(0), name(""), category(0)
	{}
	
	std::size_t Address::getId() const
	{
		return id;
	}
	
	std::string Address::getName() const
	{
		return name;
	}
	
	int Address::getCategory() const
	{
		return category;
	}
	
	Type Address::getType() const
	{
		return type;
	}
	
	std::string Address::getString() const
	{
		return std::to_string(id)+"/"+std::to_string(category);
	}
	
	/*std::size_t Address::addMember(const std::size_t& location, const std::string& name, const Type& type)
	{//TODO correct type here, not the same.
		members.add(members.size(), Rosie::getId(name), std::make_shared<Address>(location, name, type.id));
		return type.getSize();
	}
		
	Address Address::getMemberAddress(const std::string& name) const
	{
		return *members[Rosie::getId(name)];
	}
		
	Address Address::getMemberAddress(const int& offset) const
	{
		return *members[offset];
	}
		
	std::size_t Address::size() const
	{
		std::size_t result = type.size;
		for(std::shared_ptr<Address> member : members.values)
		{
			result+=member->size();
		}
		return result;
	}*/
	
	
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
		variables[dest.getId()] = getVariable(src);
	}
	
	Variable State::getVariable(const Address& address)
	{
		if(address.getCategory() == 0)
		{
			return constants[address.getId()];			
		}
		else if(address.getCategory() == 1)
		{
			if(address.getId() == 0)
			{
				return pop();
			}
			else
			{
				if(variables.find(address.getId()) == variables.end())
				{
					variables.insert(std::pair<int, Variable>(address.getId(), Variable()));
				}
				return variables[address.getId()];				
			}
		}
		else //type = 2
		{
			return Variable(static_cast<int>(address.getId()));
		}
	}
	
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
	
	std::string Token::getString() const
	{
		return value+" ("+Rosie::typeNames[type]+")";
	}
}
