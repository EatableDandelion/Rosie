#include "Serialization.h"

namespace Rosie
{
	/*
	Serializable::Serializable(const std::string& name):name(name)
	{}
	
	Serializable::Serializable(const Serializable& copyObject):name(copyObject.name)
	{
		for(std::shared_ptr<Serializable> member : copyObject.members)
		{
			members.push_back(std::shared_ptr<Serializable>(member));
		}
	}
	
	Serializable::Serializable(const std::string& name, Serializable* copyObject):name(name), members(copyObject->members)
	{}
	
	void Serializable::addMember(const std::string& name, Serializable* member)
	{
		member->name = name;
		members.push_back(std::shared_ptr<Serializable>(member));
	}
	
	void Serializable::addMember(const std::string& name, float* member)
	{
		members.push_back(std::make_shared<PrimitiveMember<float>>(name, member));
	}
	
	void Serializable::addMember(const std::string& name, int* member)
	{
		members.push_back(std::make_shared<PrimitiveMember<int>>(name, member));
	}
	
	void Serializable::addMember(const std::string& name, bool* member)
	{
		members.push_back(std::make_shared<PrimitiveMember<bool>>(name, member));
	}
	
	void Serializable::addMember(const std::string& name, std::string* member)
	{
		members.push_back(std::make_shared<PrimitiveMember<std::string>>(name, member));
	}
	
	
	void Serializable::deserialize(RosieVM& vm)
	{
		deserialize(vm.getState().getActiveScope());
	}
	
	void Serializable::deserialize(std::shared_ptr<RosieObject> parent)
	{
		for(std::shared_ptr<Serializable> member : members)
		{
			member->deserialize(parent->getScope(name));
		}
	}*/
	
	void Serializable::setMember(const std::string& name, Serializable* member, const std::shared_ptr<RosieObject>& object)
	{
		member->deserialize(object->getScope(name));
	}
	
	void Serializable::setMember(const std::string& name, float* member, const std::shared_ptr<RosieObject>& object)
	{
		*member = object->getValue<float>(name);
	}
	
	void Deserializer::run(RosieVM& vm)
	{
		for(std::pair<std::string, std::shared_ptr<Serializable>> pair : members)
		{
			pair.second->deserialize(vm.getState().getActiveScope()->getScope(pair.first));
		}
	}
			
	void Deserializer::addMember(const std::string& name, Serializable* member)
	{
		members.insert(std::pair<std::string, std::shared_ptr<Serializable>>(name, std::shared_ptr<Serializable>(member)));
	}
	
	
	Position::Position(const float& x0):x(x0)
	{}
	
	void Position::setX(const float& x0)
	{
		x = x0;
	}
	
	float Position::getX() const
	{
		return x;
	}
	
	void Position::deserialize(const std::shared_ptr<RosieObject>& object)
	{
		setMember("x", &x, object);
	}
	
	Mesh::Mesh(Position position0):position(position0)
	{}

	Position Mesh::getPosition() const
	{
		return position;
	}
	
	void Mesh::deserialize(const std::shared_ptr<RosieObject>& object)
	{
		setMember("position", &position, object);
	}
}
