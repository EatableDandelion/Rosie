#include "InterpreterObjects.h"

namespace Rosie
{
	
	Member::Member(const std::size_t typeId, const int& offset): typeId(typeId), offset(offset)
	{}
	
	Type::Type()
	{}
	
	Type::Type(const Type& other):id(other.id), members(other.members)
	{}
	
	Type::Type(const std::size_t& id):id(id)
	{}
	
	bool Type::operator==(const Type& other)
	{
		return id == other.id;
	}
	
	void Type::addMember(const std::size_t& name, const std::size_t& typeId)
	{
		members.insert(std::pair<std::size_t, Member>(name, Member(typeId, members.size()+1)));
	}
	
	TypeCollection::TypeCollection()
	{
		addType("int");
		addType("float");
		addType("string");
		addType("boolean");
		addType("function");
	}
	
	void TypeCollection::addType(const std::string& name)
	{
		types.insert(std::pair<std::size_t, Type>(Rosie::getId(name), Type(Rosie::getId(name))));
	}
	
	void TypeCollection::addMemberToType(const Type& type, const std::string& memberName, const std::string& memberType)
	{
		types[type.id].addMember(Rosie::getId(memberName), Rosie::getId(memberType));
	}
	
	int TypeCollection::getSize(const Type& type)
	{
		int result = 1;
		for(const auto& pair : type.members)
		{
			result += getSize(types.at(pair.second.typeId));
		}
		return result;
	}
	
	bool TypeCollection::hasType(const std::string& name) const
	{
		return types.find(Rosie::getId(name)) != types.end();
	}
	
	Type TypeCollection::getType(const std::string& name) const
	{
		return types.at(Rosie::getId(name));
	}

	
	Address::Address(const int& id, const Category& category, const std::string& name):id(id), category(category), name(name)
	{}
	
	Address::Address(const Address& address):id(address.id), name(address.name), category(address.category)//, type(address.type)
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
	
	/*Type Address::getType() const
	{
		return type;
	}*/
	
	std::string Address::getString() const
	{
		return std::to_string(id)+"/"+std::to_string(category);
	}
	
	
	
	Memory::Memory(const Category& category, const int& startIndex):category(category), head(startIndex)
	{
		scope.push(startIndex);
	}
	
	Address Memory::newAddress(const std::string& name)//, const int& size)
	{
		std::size_t id = Rosie::getId(name);
		
		int index = head;
		head+=1;//size;
		addresses.insert(std::pair<std::size_t, Address>(id, Address(index, category, name)));

		return addresses[id];
	}
			
	Address Memory::getAddress(const std::string& name)
	{
		return addresses[Rosie::getId(name)];
	}
	
	bool Memory::hasAddress(const std::string& name)
	{
		return addresses.find(Rosie::getId(name)) != addresses.end();
	}

	void Memory::startScope()
	{
		scope.push(head);
	}
			
	void Memory::endScope()
	{
		head = scope.top();
		scope.pop();
	}
	
	
	
	Constant::Constant(const std::string& name, const std::string& value):typeId(Rosie::getId(name)), value(value)
	{}
	
	std::size_t Constant::getTypeId() const
	{
		return typeId;
	}
	
	std::string Constant::getValue() const
	{
		return value;
	}
}
