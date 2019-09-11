#include "Serialization.h"

namespace Rosie
{
	void Serializable::uploadMember(const std::string& name, const Serializable& member, std::ostream& stream, int& depth) const
	{
		stream << getIndentation(depth) << name << " = {" << std::endl;
		depth++;
		member.serialize(stream, depth);
		depth--;
		stream << getIndentation(depth) << "};" << std::endl;
	}
	
	void Serializable::uploadMember(const std::string& name, const float& member, std::ostream& stream, int& depth) const
	{
		stream << getIndentation(depth) << name << " = " << member << ";" << std::endl;
	}	
	
	void Serializable::uploadMember(const std::string& name, const int& member, std::ostream& stream, int& depth) const
	{
		stream << getIndentation(depth) << name << " = " << member << ";" << std::endl;
	}
	
	void Serializable::uploadMember(const std::string& name, const bool& member, std::ostream& stream, int& depth) const
	{
		stream << getIndentation(depth) << name << " = " << member << ";" << std::endl;
	}
	
	void Serializable::uploadMember(const std::string& name, const std::string& member, std::ostream& stream, int& depth) const
	{
		stream << getIndentation(depth) << name << " = \"" << member << "\";" << std::endl;
	}
	
	void Serializable::downloadMember(const std::string& name, Serializable* member, const std::shared_ptr<Scope>& object)
	{
		member->deserialize(object->getScope(name));
	}
	
	void Serializable::downloadMember(const std::string& name, float* member, const std::shared_ptr<Scope>& object)
	{
		*member = object->getMember(name).get<float>();
	}
	
	void Serializable::downloadMember(const std::string& name, int* member, const std::shared_ptr<Scope>& object)
	{
		*member = object->getMember(name).get<int>();
	}
	
	void Serializable::downloadMember(const std::string& name, bool* member, const std::shared_ptr<Scope>& object)
	{
		*member = object->getMember(name).get<bool>();
	}
	
	void Serializable::downloadMember(const std::string& name, std::string* member, const std::shared_ptr<Scope>& object)
	{
		*member = object->getMember(name).get<std::string>();		
	}
	
	std::string Serializable::getIndentation(const int& depth) const
	{
		std::string indentation = "";
		for(int i = 0; i<depth; i++)
		{
			indentation+="\t";
		}
		return indentation;
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
		int depth = 0;
		file.open(fileName+".ros");
		serialize(file, depth);
		file.close();
	}
	
	void Serializer::deserialize()
	{
		RosieVM vm("myTest", Syntax());
		vm.compile();
		vm.run();
		
		deserialize(vm.getState().getActiveScope());
	}
			
	void Serializer::addMember(const std::string& name, Serializable* member)
	{
		members.insert(std::pair<std::string, std::shared_ptr<Serializable>>(name, std::shared_ptr<Serializable>(member)));
	}
	
	
	void Serializer::serialize(std::ostream& stream, int& depth) const
	{
		for(const std::pair<std::string, std::shared_ptr<Serializable>> pair : members)
		{
			uploadMember(pair.first, *(pair.second), stream, depth);
		}
	}
	
	void Serializer::deserialize(const std::shared_ptr<Scope>& object)
	{
		for(std::pair<std::string, std::shared_ptr<Serializable>> pair : members)
		{
			downloadMember(pair.first, pair.second.get(), object);
		}
	}

	
	Position::Position(const float& x0, const float& y0, const std::string& text):text(text)
	{
		X.push_back(x0);
		X.push_back(y0);
	}
	
	std::vector<float>& Position::getX()
	{
		return X;
	}
	
	std::string& Position::getText()
	{
		return text;
	}
	
	void Position::serialize(std::ostream& stream, int& depth) const
	{
		uploadMember("X", X, stream, depth);
		uploadMember("text", text, stream, depth);
	}
	
	void Position::deserialize(const std::shared_ptr<Scope>& object)
	{
		downloadMember("X", &X, object);
		downloadMember("text", &text, object);
	}
	
	Mesh::Mesh(Position position0):position(position0)
	{}

	Position& Mesh::getPosition()
	{
		return position;
	}
	
	void Mesh::serialize(std::ostream& stream, int& depth) const
	{
		uploadMember("position", position, stream, depth);
	}
	
	void Mesh::deserialize(const std::shared_ptr<Scope>& object)
	{
		downloadMember("position", &position, object);
	}
}
