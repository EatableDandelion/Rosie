#pragma once

#include <iostream>
#include <fstream>
#include "Rosie.h"

namespace Rosie
{	
	class Serializer;
	
	class Serializable
	{
		protected:
			/** To be overriden by the derived classes */
			virtual void serialize(std::ostream& stream, int& depth) const = 0;
			virtual void deserialize(const std::shared_ptr<Scope>& object) = 0;
			
			void uploadMember(const std::string& name, const Serializable& member, std::ostream& stream, int& depth) const;
			void uploadMember(const std::string& name, const float& member, std::ostream& stream, int& depth) const;
			void uploadMember(const std::string& name, const int& member, std::ostream& stream, int& depth) const;
			void uploadMember(const std::string& name, const bool& member, std::ostream& stream, int& depth) const;
			void uploadMember(const std::string& name, const std::string& member, std::ostream& stream, int& depth) const;
			
			template<typename T>
			void uploadMember(const std::string& name, const std::vector<T>& members, std::ostream& stream, int& depth) const
			{
				stream << getIndentation(depth) << name << " = [";
				for(int i = 0; i<members.size()-1; i++)
				{
					stream << members[i] << ", ";
				}
				stream << members[members.size()-1] << "];" << std::endl;
			}
			
			void downloadMember(const std::string& name, Serializable* member, const std::shared_ptr<Scope>& object);
			void downloadMember(const std::string& name, float* member, const std::shared_ptr<Scope>& object);
			void downloadMember(const std::string& name, int* member, const std::shared_ptr<Scope>& object);
			void downloadMember(const std::string& name, bool* member, const std::shared_ptr<Scope>& object);
			void downloadMember(const std::string& name, std::string* member, const std::shared_ptr<Scope>& object);
			
			template<typename T>
			void downloadMember(const std::string& name, std::vector<T>* members, const std::shared_ptr<Scope>& object)
			{
				for(int i = 0; i<members->size(); i++)
				{
					downloadMember(std::to_string(i+1), &(*members)[i], object->getScope(name));
				}
			}
			
			friend class Serializer;
		
		private:
			std::string getIndentation(const int& depth) const;
	};
	
	class Serializer : public Serializable
	{
		public:
			Serializer(const std::string& fileName);
			~Serializer();
			
			void serialize();
			void deserialize();
			
			void addMember(const std::string& name, Serializable* member);
		
		protected:
			virtual void serialize(std::ostream& stream, int& depth) const;
			virtual void deserialize(const std::shared_ptr<Scope>& object);
		
		private:
			std::ofstream file;
			std::unordered_map<std::string, std::shared_ptr<Serializable>> members;
			const std::string fileName;
	};
	
	struct Position : public Serializable
	{
		public:
			Position(const float& x0, const float& y0, const std::string& text);
			
			std::vector<float>& getX();
			
			std::string& getText();
			
			void serialize(std::ostream& stream, int& depth) const;
			
			void deserialize(const std::shared_ptr<Scope>& object);
			
		private:
			std::vector<float> X;
			std::string text;
	};
	
	class Mesh : public Serializable
	{
		public:
			Mesh(Position position);
		
			Position& getPosition();
			
			void serialize(std::ostream& stream, int& depth) const;
			
			void deserialize(const std::shared_ptr<Scope>& object);
		
		private:
			Position position;
	};
}
