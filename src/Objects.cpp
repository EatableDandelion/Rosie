#include "Objects.h"

namespace Rosie
{
	std::size_t getId(const std::string& name)
	{
		return std::hash<std::string>{}(name);
	}
	
	Type::Type(const std::size_t& id, const int& size):id(id), size(size)
	{}
	
	bool Type::operator==(const Type& other)
	{
		return id == other.id;
	}
	
	void Type::addMember(const std::size_t& name, const std::size_t& typeId)
	{
		members.insert(std::pair<std::size_t, std::size_t>(name, typeId));
	}
	
	TypeCollection::TypeCollection()
	{
		addType("int", 1);
		addType("float", 1);
		addType("string", 1);
		addType("boolean", 1);
	}
	
	int TypeCollection::addType(const std::string& name, const int& size)
	{
		types.insert(std::pair<std::size_t, Type>(Rosie::getId(name), Type(Rosie::getId(name), size)));
	}
	
	void TypeCollection::addMemberToType(const Type& type, const std::string& memberName, const std::string& memberType)
	{
		types[type.id].addMember(Rosie::getId(memberName), Rosie::getId(memberType));
	}
	
	int TypeCollection::getSize(const Type& type)
	{
		return types[type.id].size;
	}
	
	bool TypeCollection::hasType(const std::string& name) const
	{
		return types.find(Rosie::getId(name)) != types.end();
	}
	
	Type Type::getType(const std::string& name) const
	{
		return types.at(Rosie::getId(name));
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
	
	
	Address::Address(const int& id, const Category& category, const Type& type, const std::string& name):id(id), category(category), type(type), name(name)
	{}
	
	Address::Address(const Address& address):id(address.id), name(address.name), category(address.category), type(address.type)
	{}
	
	Address::Address():id(0), name(""), category(Category::CONSTANT)
	{}
	
	int Address::getId() const
	{
		return id;
	}
	
	std::string Address::getName() const
	{
		return name;
	}
	
	Category Address::getCategory() const
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
	
	void Address::addMember(const std::string& name, const Category& category, const Type& type)
	{		
		int currentSize = size();
		members.add(members.size(), Rosie::getId(name), std::make_shared<Address>(id+currentSize, category, type, name));
	}
		
	Address Address::getMemberAddress(const std::string& name) const
	{
		return *members[Rosie::getId(name)];
	}
		
	Address Address::getMemberAddress(const int& offset) const
	{
		return *members[offset];
	}
		
	int Address::size() const
	{
		int res = type.size();
		for(const std::shared_ptr<Address>& member : members.getValues())
		{
			res += member->size();
		}
		return res;
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
		variables[dest.getId()] = getVariable(src);
	}
	
	Variable State::getVariable(const Address& address)
	{
		if(address.getCategory() == Category::CONSTANT)
		{
			return constants[address.getId()];			
		}
		else if(address.getCategory() == Category::VARIABLE)
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
		else if(address.getCategory() == Category::INTEGER)
		{
			return Variable(address.getId());
		}
		else //to be implemented
		{
			return Variable(address.getId());
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
