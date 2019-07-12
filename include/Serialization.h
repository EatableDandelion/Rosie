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
			virtual void serialize(std::ostream& stream, std::string& indentation) const = 0;
			virtual void deserialize(const std::shared_ptr<RosieObject>& object) = 0;
			
			void uploadMember(const std::string& name, const Serializable& member, std::ostream& stream, std::string& indentation) const;
			void uploadMember(const std::string& name, const float& member, std::ostream& stream, std::string& indentation) const;
			void uploadMember(const std::string& name, const int& member, std::ostream& stream, std::string& indentation) const;
			void uploadMember(const std::string& name, const bool& member, std::ostream& stream, std::string& indentation) const;
			void uploadMember(const std::string& name, const std::string& member, std::ostream& stream, std::string& indentation) const;
			
			void downloadMember(const std::string& name, Serializable* member, const std::shared_ptr<RosieObject>& object);
			void downloadMember(const std::string& name, float* member, const std::shared_ptr<RosieObject>& object);
			void downloadMember(const std::string& name, int* member, const std::shared_ptr<RosieObject>& object);
			void downloadMember(const std::string& name, bool* member, const std::shared_ptr<RosieObject>& object);
			void downloadMember(const std::string& name, std::string* member, const std::shared_ptr<RosieObject>& object);
			
			friend class Serializer;
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
			virtual void serialize(std::ostream& stream, std::string& indentation) const;
			virtual void deserialize(const std::shared_ptr<RosieObject>& object);
		
		private:
			std::ofstream file;
			std::unordered_map<std::string, std::shared_ptr<Serializable>> members;
			const std::string fileName;
	};
	
	struct Position : public Serializable
	{
		public:
			Position(const float& x0, const std::string& text);
			
			float& getX();
			
			std::string& getText();
			
			void serialize(std::ostream& stream, std::string& indentation) const;
			
			void deserialize(const std::shared_ptr<RosieObject>& object);
			
		private:
			float x;
			std::string text;
	};
	
	class Mesh : public Serializable
	{
		public:
			Mesh(Position position);
		
			Position& getPosition();
			
			void serialize(std::ostream& stream, std::string& indentation) const;
			
			void deserialize(const std::shared_ptr<RosieObject>& object);
		
		private:
			Position position;
	};
}
