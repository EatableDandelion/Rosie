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
	
	struct Variable
	{
		public:
			Variable(const float& floatValue);
			Variable(const int& integerValue);
			Variable(const bool& booleanValue);
			Variable(const std::string& stringValue);
			Variable();

			
			template<typename T>
			T get() const
			{
				return std::get<T>(value);
			}
			
			void set(const float& newValue);
			void set(const int& newValue);
			void set(const bool& newValue);
			void set(const std::string& newValue);
			void set(const Variable& other);
			void addMember(const std::string& name, const Variable& member);
			std::shared_ptr<Variable> getMember(const std::string& name) const;
			
			friend std::ostream& operator <<(std::ostream& os, Variable& var)
			{
				if(var.type == 0)
				{
					os << std::to_string(var.get<float>())+ " (float)"; 
				}
				else if(var.type == 1)
				{
					os << std::to_string(var.get<int>())+ " (int)"; 
				}
				else if(var.type == 2)
				{
					os << std::to_string(var.get<bool>())+ " (boolean)"; 
				}
				else if(var.type == 3)
				{
					os << var.get<std::string>()+ " (string)"; 
				}
				else if(var.type == 4)
				{
					os << "[";
					for(const auto& member : var.members)
					{
						os << *(member.second) << ", ";
					}
					os << "]";
				}
				return os;
			}
			
		private:
			int type;
			std::variant<float, int, bool, std::string> value;
			std::unordered_map<std::size_t, std::shared_ptr<Variable>> members;
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
	
	template<typename T>
	struct Function
	{
		public:
			Function(const std::string& name, const std::function<void(std::vector<T>&, std::stack<T>&)> func, const int& id):name(name), id(id)
			{
				m_func = func;
			}
			
			Function(const Function<T>& other):name(other.name), m_func(other.m_func), id(other.id)
			{}
			
			Function()
			{}
		
			void execute(std::vector<T>& arguments, std::stack<T>& results) const
			{
				m_func(arguments, results);
			}
	
			std::string getName() const
			{
				return name;
			}
	
			int getId() const
			{
				return id;
			}
			
		private:
			std::string name;
			int id;
			std::function<void(std::vector<T>&, std::stack<T>&)> m_func;
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
	
	struct State
	{
		public:
			State(const Syntax& syntax);
			void addVariable(const std::string& name, const int& type, const Handle& handle);
			void addConstant(const int& id, const Variable& cst);
			void addMethod(const Function<Variable>& func);
			void push(const Variable& variable);
			void push(const Handle& handle);
			Variable pop();
			//bool empty() const;
			void copyVariable(Handle& dest, const Handle& src);
			Variable getVariable(const Handle& handle);
			void execute(const std::string& methodName);
			
		private:
			Syntax syntax;
			DualMap<Handle, std::size_t, Variable> variables;
			std::unordered_map<int, Variable> constants;
			std::stack<Variable> callStack;
			//DualMap<int, std::string, Function<Variable>> methods;
			std::unordered_map<std::string, Function<Variable>> methods;
	};	
}
