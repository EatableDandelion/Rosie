#include "InterpreterObjects.h"

namespace Rosie
{
	
	AddressId::AddressId(const int id)
	{
		ids.push_back(id);
	}
	
	AddressId::AddressId(const AddressId& copyId):ids(copyId.ids)
	{}
	
	AddressId::AddressId(const std::string& textId)
	{
		for(const std::string& id : Rosie::split(textId, "."))
		{
			ids.push_back(std::stoi(id));
		}
	}
	
	AddressId::AddressId(const int id, const AddressId& parentId):ids(parentId.ids)
	{
		ids.push_back(id);
	}
			
	AddressId AddressId::operator++(int)
	{
		ids[ids.size()-1] = ids[ids.size()-1]+1;
		return *this;
	}
	
	void AddressId::scopeIn()
	{
		ids.push_back(0);
	}
	
	std::string AddressId::toString() const
	{
		std::string res = std::to_string(ids[0]);
		for(int i = 1; i<ids.size(); i++)
		{
			res+="."+std::to_string(ids[i]);
		}
		return res;
	}
	
	
	
	Address::Address(const AddressId& id, const Category& category, const std::string& name, const TokenType& type):id(id), category(category), name(name), type(type), scope(0)
	{}
	
	Address::Address(const Address& address):id(address.id), name(address.name), category(address.category), type(address.type), scope(address.scope)
	{}
	
	Address::Address():id(0), name(""), category(Category::CONSTANT), type(TokenType::UNDEFINED), scope(0)
	{}
	
	AddressId Address::getId() const
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
		return id.toString()+"/"+std::to_string(category);
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
	
	
	
	AddressMap::AddressMap(const Category& category, const int& startIndex):category(category), currentScope(startIndex)
	{
		//scope.push(startIndex);
	}
	
	Address AddressMap::newAddress(const std::string& name)//, const int& size)
	{
		return newAddress(name, TokenType::UNDEFINED);
	}
	
	Address AddressMap::newAddress(const std::string& name, const TokenType& tokenType)
	{
		std::size_t nameId = Rosie::getId(name);
		
		AddressId index(currentScope);
		currentScope++;
		Address newAddress = Address(index, category, name, tokenType);
		addresses.insert(std::pair<std::size_t, Address>(nameId, newAddress));

		return addresses[nameId];
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
	
	void AddressMap::startScope(const AddressId& parentId)
	{
		//scope.push(head);
		
		previousScopes.push(currentScope);
		currentScope = parentId;
		currentScope.scopeIn();
	}
			
	void AddressMap::endScope()
	{
		//head = scope.top();
		//scope.pop();
		
		currentScope = previousScopes.top();
		previousScopes.pop();
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
