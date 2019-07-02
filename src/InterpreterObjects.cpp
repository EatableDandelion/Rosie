#include "InterpreterObjects.h"

namespace Rosie
{
	
	Address::Address(const int& id, const Category& category, const std::string& name, const TokenType& type):id(id), category(category), name(name), type(type), scope(0)
	{}
	
	Address::Address(const Address& address):id(address.id), name(address.name), category(address.category), type(address.type), scope(address.scope)
	{}
	
	Address::Address():id(0), name(""), category(Category::CONSTANT), type(TokenType::UNDEFINED), scope(0)
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
	
	std::string Address::getString() const
	{
		return std::to_string(id)+"/"+std::to_string(category);
	}
	
	void Address::setType(const TokenType& tokenType)
	{
		type = tokenType;
	}
	
	int Address::getTypeId() const
	{
		return int(type);
	}
	
	void Address::setScope(const int& scope0)
	{
		scope = scope0;
	}
	
	int Address::getScope()
	{
		return scope;
	}
	
	TokenType Address::getType() const
	{
		return type;
	}
	
	
	
	AddressMap::AddressMap(const Category& category, const int& startIndex):category(category), head(startIndex)
	{
		scope.push(startIndex);
	}
	
	Address AddressMap::newAddress(const std::string& name)//, const int& size)
	{
		return newAddress(name, TokenType::UNDEFINED);
	}
	
	Address AddressMap::newAddress(const std::string& name, const TokenType& tokenType)
	{
		std::size_t id = Rosie::getId(name);
		
		int index = head;
		head+=1;
		//newAddress.setScope(scope.top()-1);
		Address newAddress = Address(index, category, name, tokenType);
		addresses.insert(std::pair<std::size_t, Address>(id, newAddress));

		return addresses[id];
	}
			
	Address AddressMap::getAddress(const std::string& name)
	{
		return addresses[Rosie::getId(name)];
	}
	
	bool AddressMap::hasAddress(const std::string& name)
	{
		return addresses.find(Rosie::getId(name)) != addresses.end();
	}
	
	std::vector<Address> AddressMap::getAddresses() const
	{
		std::vector<Address> res;
		for(std::pair<std::size_t, Address> pair : addresses)
		{
			res.push_back(pair.second);
		}
		return res; 
	}
	
	void AddressMap::startScope()
	{
		scope.push(head);
	}
			
	void AddressMap::endScope()
	{
		head = scope.top();
		scope.pop();
	}
	
	
	Constant::Constant(const TokenType& type, const std::string& value):type(type), value(value)
	{}
	
	int Constant::getTypeId() const
	{
		return int(type);
	}
	
	std::string Constant::getValue() const
	{
		return value;
	}
}
