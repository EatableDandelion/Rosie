#include "Serialization.h"

namespace Rosie
{
	void Serializable::uploadMember(const std::string& name, const Serializable& member, std::ostream& stream, std::string& indentation) const
	{
		stream << indentation << name << " = {" << std::endl;
		indentation+="\t";
		member.serialize(stream, indentation);
		indentation.pop_back();
		stream << indentation << "};" << std::endl;
	}
	
	void Serializable::uploadMember(const std::string& name, const float& member, std::ostream& stream, std::string& indentation) const
	{
		stream << indentation << name << " = " << member << ";" << std::endl;
	}	
	
	void Serializable::uploadMember(const std::string& name, const int& member, std::ostream& stream, std::string& indentation) const
	{
		stream << indentation << name << " = " << member << ";" << std::endl;
	}
	
	void Serializable::uploadMember(const std::string& name, const bool& member, std::ostream& stream, std::string& indentation) const
	{
		stream << indentation << name << " = " << member << ";" << std::endl;
	}
	
	void Serializable::uploadMember(const std::string& name, const std::string& member, std::ostream& stream, std::string& indentation) const
	{
		stream << indentation << name << " = \"" << member << "\";" << std::endl;
	}
	
	void Serializable::downloadMember(const std::string& name, Serializable* member, const std::shared_ptr<RosieObject>& object)
	{
		member->deserialize(object->getScope(name));
	}
	
	void Serializable::downloadMember(const std::string& name, float* member, const std::shared_ptr<RosieObject>& object)
	{
		*member = object->getValue<float>(name);
	}
	
	void Serializable::downloadMember(const std::string& name, int* member, const std::shared_ptr<RosieObject>& object)
	{
		*member = object->getValue<int>(name);
	}
	
	void Serializable::downloadMember(const std::string& name, bool* member, const std::shared_ptr<RosieObject>& object)
	{
		*member = object->getValue<bool>(name);
	}
	
	void Serializable::downloadMember(const std::string& name, std::string* member, const std::shared_ptr<RosieObject>& object)
	{
		*member = object->getValue<std::string>(name);
	}
	
	
	Serializer::Serializer(const std::string& fileName):fileName(fileName)
	{}
	
	Serializer::~Serializer()
	{
		if(file.is_open())
		{
			file.close();
		}
	}
	
	void Serializer::serialize()
	{
		file.open(fileName+".ros");
		std::string indentation = "";
		serialize(file, indentation);
		file.close();
	}
	
	void Serializer::deserialize()
	{
		RosieVM vm("myTest", Syntax());
		vm.compile();
		vm.run();
		
		deserialize(vm.getState().getActiveScope());
		
		/*for(std::pair<std::string, std::shared_ptr<Serializable>> pair : members)
		{
			pair.second->deserialize(vm.getState().getActiveScope()->getScope(pair.first));
			
		}*/
	}
			
	void Serializer::addMember(const std::string& name, Serializable* member)
	{
		members.insert(std::pair<std::string, std::shared_ptr<Serializable>>(name, std::shared_ptr<Serializable>(member)));
	}
	
	
	void Serializer::serialize(std::ostream& stream, std::string& indentation) const
	{
		for(const std::pair<std::string, std::shared_ptr<Serializable>> pair : members)
		{
			uploadMember(pair.first, *(pair.second), stream, indentation);
		}
	}
	
	void Serializer::deserialize(const std::shared_ptr<RosieObject>& object)
	{
		for(std::pair<std::string, std::shared_ptr<Serializable>> pair : members)
		{
			downloadMember(pair.first, pair.second.get(), object);
		}
	}

	
	Position::Position(const float& x0, const std::string& text):x(x0), text(text)
	{}
	
	float& Position::getX()
	{
		return x;
	}
	
	std::string& Position::getText()
	{
		return text;
	}
	
	void Position::serialize(std::ostream& stream, std::string& indentation) const
	{
		uploadMember("x", x, stream, indentation);
		uploadMember("text", text, stream, indentation);
	}
	
	void Position::deserialize(const std::shared_ptr<RosieObject>& object)
	{
		downloadMember("x", &x, object);
		downloadMember("text", &text, object);
	}
	
	Mesh::Mesh(Position position0):position(position0)
	{}

	Position& Mesh::getPosition()
	{
		return position;
	}
	
	void Mesh::serialize(std::ostream& stream, std::string& indentation) const
	{
		uploadMember("position", position, stream, indentation);
	}
	
	void Mesh::deserialize(const std::shared_ptr<RosieObject>& object)
	{
		downloadMember("position", &position, object);
	}
}
