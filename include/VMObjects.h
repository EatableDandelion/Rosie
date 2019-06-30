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
	class State;
	
	class IVariable
	{
		public:
			virtual void addVariable(const std::string& name, const int& type, const Handle& handle);
			virtual void addFunction(const int& id, const std::string& name);
			virtual std::shared_ptr<IVariable> add(const std::shared_ptr<IVariable> other);
			virtual std::shared_ptr<IVariable> subtract(const std::shared_ptr<IVariable> other);
			virtual std::shared_ptr<IVariable> multiply(const std::shared_ptr<IVariable> other);
			virtual std::shared_ptr<IVariable> divide(const std::shared_ptr<IVariable> other);
			virtual std::string toString() const = 0;
	};
	
	class Variable
	{
		public:
			Variable(const float& floatValue);
			Variable(const int& integerValue);
			Variable(const bool& booleanValue);
			Variable(const std::string& stringValue);
			Variable(const TokenType& tokenType);
			Variable(const std::shared_ptr<IVariable>& variable);
			Variable();
			
			Variable operator+(const Variable& other);
			Variable operator-(const Variable& other);
			Variable operator*(const Variable& other);
			Variable operator/(const Variable& other);
			
			friend std::ostream& operator <<(std::ostream& os, Variable& var)
			{
				std::string text = (var.variable)->toString();
				os << text; 
				return os;
			}
			
			void addVariable(const std::string& name, const int& type, const Handle& handle);
			void addFunction(const int& id, const std::string& name);		
			std::shared_ptr<IVariable> getVariable() const;
			
		private:
			std::shared_ptr<IVariable> variable;
	};
	
	class Primitive : public IVariable
	{
		public:
			Primitive(const float& floatValue);
			Primitive(const int& integerValue);
			Primitive(const bool& booleanValue);
			Primitive(const std::string& stringValue);
			Primitive(const TokenType& tokenType);
			Primitive();
			
			virtual std::shared_ptr<IVariable> add(const std::shared_ptr<IVariable> other);
			virtual std::shared_ptr<IVariable> subtract(const std::shared_ptr<IVariable> other);
			virtual std::shared_ptr<IVariable> multiply(const std::shared_ptr<IVariable> other);
			virtual std::shared_ptr<IVariable> divide(const std::shared_ptr<IVariable> other);
			
			template<typename T>
			T get() const
			{
				return std::get<T>(value);
			}

			void set(const float& newValue);
			void set(const int& newValue);
			void set(const bool& newValue);
			void set(const std::string& newValue);
			void set(const Primitive& other);
			
			virtual std::string toString() const;
			
		private:
			int type;
			std::variant<float, int, bool, std::string> value;
	};

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
	
	class CompositeVariable : public IVariable
	{
		public:
			void addVariable(const std::string& name, const int& type, const Handle& handle);
			void addFunction(const int& id, const std::string& name);
			void addConstant(const int& id, const Variable& cst);
			void setFunction(const std::string& name, const std::function<void(CallStack&)>& func);		
			void copyVariable(Handle& dest, const Handle& src);
			Variable getVariable(const Handle& handle);	
			void execute(const int& functionId);
			virtual std::string toString() const;
			void push(const Variable& variable);
			void push(const Handle& handle);
			
		private:
			DualMap<Handle, std::size_t, Variable> variables;
			std::unordered_map<int, Variable> constants;
			DualMap<int, std::string, Function<CallStack&>> functions;
			CallStack callStack;
			std::weak_ptr<IVariable> parent;
	};
	
	class State
	{
		public:
			State();
			
			void addVariable(const std::string& name, const int& type, const Handle& handle);
			void addFunction(const int& id, const std::string& name);
			void addConstant(const int& id, const Variable& cst);
			void setFunction(const std::string& name, const std::function<void(CallStack&)>& func);		
			void copyVariable(Handle& dest, const Handle& src);
			Variable getVariable(const Handle& handle);	
			void execute(const int& functionId);
			void push(const Variable& variable);
			void push(const Handle& handle);
			
			void startScope(const Handle& handle);
			void endScope();
				
		private:				
			std::shared_ptr<CompositeVariable> scope;
			
	};
}
