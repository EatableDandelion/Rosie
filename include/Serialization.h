#pragma once

#include "Rosie.h"

namespace Rosie
{
	/*
	template<typename T>
	T get(const std::string& name, std::shared_ptr<RosieObject>& object)
	{
		return object->getValue<T>(name);
	}
	
	class Serializable;
	
	template<typename T>
	class PrimitiveMember;
	
	class Serializable
	{
		public:
			Serializable(const std::string& name);
			
			Serializable(const Serializable& copyObject);
			
			Serializable(const std::string& name, Serializable* copyObject);
			
			void addMember(const std::string& name, Serializable* member);
			void addMember(const std::string& name, float* member);
			void addMember(const std::string& name, int* member);
			void addMember(const std::string& name, bool* member);
			void addMember(const std::string& name, std::string* member);
			
			void deserialize(RosieVM& vm);
			
			
		protected:
			virtual void deserialize(std::shared_ptr<RosieObject> parent);
			std::string name;
		
		private:			
			std::vector<std::shared_ptr<Serializable>> members;
	};
	
	template<typename T>
	class PrimitiveMember : public Serializable
	{
		public:
			PrimitiveMember(const std::string& name, T* value):Serializable(name), value(std::shared_ptr<T>(value))
			{}
		
		protected:
			virtual void deserialize(std::shared_ptr<RosieObject> parent)
			{
				*value = Rosie::get<T>(name, parent);
			}

		private:
			std::shared_ptr<T> value;
	};*/
	
	class Deserializer;
	
	class Serializable
	{
		protected:
			/** To be overriden by the derived classes */
			virtual void deserialize(const std::shared_ptr<RosieObject>& object) = 0;
			
			void setMember(const std::string& name, Serializable* member, const std::shared_ptr<RosieObject>& object);
			
			void setMember(const std::string& name, float* member, const std::shared_ptr<RosieObject>& object);
			
			friend class Deserializer;
	};
	
	class Deserializer
	{
		public:
			void run(RosieVM& vm);
			
			void addMember(const std::string& name, Serializable* member);
		
		private:
			std::unordered_map<std::string, std::shared_ptr<Serializable>> members;
	};
	
	struct Position : public Serializable
	{
		public:
			Position(const float& x0);
			
			void setX(const float& x);
			float getX() const;
			
			void deserialize(const std::shared_ptr<RosieObject>& object);
			
		private:
			float x;
	};
	
	class Mesh : public Serializable
	{
		public:
			Mesh(Position position);
		
			Position getPosition() const;
			
			void deserialize(const std::shared_ptr<RosieObject>& object);
		
		private:
			Position position;
	};
}
