#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <stack>
#include <variant>
#include "Utils.h"
#include "InterpreterObjects.h"
#include "Syntax.h"

namespace Rosie
{
	struct Handle
	{
		public:
			Handle(const int& id, const Category& category);
		
			int getId() const;
			Category getCategory() const;
			bool operator==(const Handle& other) const;
		
		private:
			int id;
			Category category;
	};
}
namespace std 
{
   	template<>
	struct hash<Rosie::Handle>{
		public:
		    size_t operator()(const Rosie::Handle& handle) const
		    {
				return hash<int>()(handle.getId());
		    }
   	 };
}

namespace Rosie
{
	struct RosieObject;
	class State;
	
	struct Variable
	{
		public:
			Variable(const float& floatValue);
			Variable(const int& integerValue);
			Variable(const bool& booleanValue);
			Variable(const std::string& stringValue);
			Variable(const TokenType& tokenType);
			Variable(const std::shared_ptr<RosieObject>& object);
			Variable();
			Variable(const Variable& other);
			
			Variable operator+(const Variable& other);
			Variable operator-(const Variable& other);
			Variable operator*(const Variable& other);
			Variable operator/(const Variable& other);
			Variable operator-();
			
			std::string toString() const;
			
			template<typename T>
			T get() const
			{
				return std::get<T>(value);
			}
			
			//Position p = vm.getVariable<Position>("p");
			
			friend std::ostream& operator<<(std::ostream& os, Variable& var)
			{
				os << var.toString(); 
				return os;
			}

		private:
			int type;
			std::variant<float, bool, int, std::string, std::shared_ptr<RosieObject>> value;
			
	};

	
	struct RosieObject
	{
		public:
			virtual std::string toString() const;
			Variable getMember(const std::string& name);
			std::shared_ptr<RosieObject> getScope(const std::string& name);
			
			void copyMembers(const std::shared_ptr<RosieObject>& object);
			
			template<typename T>
			T getValue(const std::string& name)
			{
				return getMember(name).get<T>();
			}
			
		private:
			DualMap<Handle, std::size_t, Variable> members;
			std::weak_ptr<RosieObject> m_parent;
			friend class State;
			
			void addMember(const std::string& name, const int& type, const Handle& handle);
			void setMember(const Handle& handle, const Variable& variable);
			Variable getMember(const Handle& handle);
			bool hasMember(const Handle& handle) const;
			void setParent(const std::shared_ptr<RosieObject> parent);
			std::shared_ptr<RosieObject> getParent() const;
	};

	
	template<typename... A>
	struct Function
	{
		public:
			Function(const std::string& name, const int& id):name(name), id(id)
			{}
			
			Function(const Function<A...>& other):name(other.name), m_func(other.m_func), id(other.id)
			{}
			
			Function()
			{}
		
			void execute(A... arguments) const
			{
				m_func(arguments...);
			}
	
			std::string getName() const
			{
				return name;
			}
	
			void setFunction(const std::function<void(A...)>& func)
			{
				m_func = func;
			}
	
			int getId() const
			{
				return id;
			}
			
		private:
			std::string name;
			int id;
			std::function<void(A...)> m_func;
	};
	
	struct CallStack
	{
		public:
			Variable pop();
			Variable peek();
			void push(const Variable& variable);
			bool empty() const;
			
		private:
			std::stack<Variable> queue;
	};
	
	class State
	{
		public:
			State(const std::string& fileName);
			
			void addVariable(const std::string& name, const int& type, const Handle& handle);
			void addFunction(const int& id, const std::string& name);
			void addConstant(const Variable& cst);
			void setFunction(const std::string& name, const std::function<void(CallStack&)>& func);		
			void copyVariable(Handle& dest, const Handle& src);
			Variable getVariable(const Handle& handle);	
			void execute(const int& functionId);
			void push(const Variable& variable);
			void push(const Handle& handle);
			std::vector<std::string> getFunctionsList() const;
			std::string getFileName() const;
			std::string toString() const;
			void startScope(const Handle& handle);
			void endScope();
			
			template<typename T>
			T getValue(const std::string& name)
			{
				return activeScope->getValue<T>(name);
			}
			
			std::shared_ptr<RosieObject> getActiveScope() const;
				
		private:
			std::shared_ptr<RosieObject> activeScope;
			std::vector<Variable> constants;
			DualMap<int, std::string, Function<CallStack&>> functions;
			CallStack callStack;
			std::string fileName;
			std::stack<std::shared_ptr<RosieObject>> scopeStack;
	};
}
