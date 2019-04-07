#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <variant>
#include <stack>
#include <unordered_map>
#include <map>

namespace Rosie{
	std::size_t getId(const std::string& name);
	
	template<typename K1, typename K2, typename V>
	class DualMap //map that can be searched for either K1 OR K2
	{
		public:
			void add(const K1& key1, const K2& key2, const V& value)
			{
				values.insert(std::pair<K2,V>(key2, value));
				keymap.insert(std::pair<K1,K2>(key1, key2));
			}
		
			bool contains(const K1& key1) const
			{
				return keymap.find(key1) != keymap.end();
			}
		
			bool contains(const K2& key2) const
			{
				return values.find(key2) != values.end();
			}
		
			V operator[](const K1& key1) const
			{
				return values.at(keymap.at(key1));
			}
		
			V operator[](const K2& key2) const
			{
				return values.at(key2);
			}
			
			int size() const
			{
				return values.size();
			}
		
			void erase(const K1& key1)
			{
				values.erase(keymap.at(key1));
				keymap.erase(key1);
			}
		
			void erase(const K2& key2)
			{
				values.erase(key2);
				for(const auto& it : keymap)
				{
					if(it.second == key2)
					{
						keymap.erase(it.first);
					}
				}
			}
			
			std::vector<V> getValues() const
			{
				std::vector<V> res;
				for(const auto& pair : values)
				{
					res.push_back(pair.second);
				}
				return res;
			}
		
		
		private:
			std::unordered_map<K2, V> values;
			std::unordered_map<K1, K2> keymap;
	};
	
	struct Type
	{
		public:
			bool operator==(const Type& other);
			
		private:
			Type(const std::size_t& id, const int& size);
			std::size_t id;
			int size;
			std::unordered_map<std::size_t, std::size_t> members; //key = name, value = typeId
			
			void addMember(const std::size_t& name, const std::size_t& typeId);
			
			friend struct TypeCollection;
	};
	
	struct TypeCollection
	{
		public:
			TypeCollection();
			int addType(const std::string& name, const int& size);
			void addMemberToType(const Type& type, const std::string& memberName, const std::string& memberType);
			int getSize(const Type& type);
			Type getType(const std::string& name) const;
			bool hasType(const std::string& name) const;
			
		private:
			std::map<std::size_t, Type> types;
	}
	
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
			
			friend std::ostream& operator <<(std::ostream& os, const Variable& var)
			{
				if(var.type == types.getType("float"))
				{
					os << std::to_string(var.get<float>())+ " (float)"; 
				}
				else if(var.type == types.getType("int"))
				{
					os << std::to_string(var.get<int>())+ " (int)"; 
				}
				else if(var.type == types.getType("boolean"))
				{
					os << std::to_string(var.get<bool>())+ " (boolean)"; 
				}
				else if(var.type == types.getType("string"))
				{
					os << var.get<std::string>()+ " (string)"; 
				}
				else
				{
					os << "Error: undefined variable type: type id = "+std::to_string(var.type);
				}
				return os;
			}
			
		private:
			TypeCollection types;
			Type type;
			std::variant<float, int, bool, std::string> value;
	};
	
	enum Category
	{	
		CONSTANT,
		VARIABLE,
		FUNCTION,
		INTEGER
	};
		
	struct Address
	{
		public:
			Address(const int& id, const Category& category = Category::CONSTANT, const Type& type = Type(), const std::string& name = "");
			Address(const Address& address);
			Address();
			
			int getId() const;
			std::string getName() const;
			Category getCategory() const;
			Type getType() const;
			std::string getString() const;
			
			void addMember(const std::string& name, const Category& category, const Type& type); //add a member, returns the size of the member added
		
			Address getMemberAddress(const std::string& name) const;
		
			Address getMemberAddress(const int& offset) const;
		
			int size() const;
		
		private:
			int id;
			std::string name;
			Category category;
			Type type;
			DualMap<int, std::size_t, std::shared_ptr<Address>> members;
	};
	
	struct State
	{
		public:
			void addVariable(const int& id);
			void addConstants(const std::vector<Variable>& csts);
			void push(const Variable& variable);
			void push(const Address& address);
			Variable pop();
			bool empty() const;
			void copyVariable(Address& dest, const Address& src);
			Variable getVariable(const Address& address);
			
		private:
			std::unordered_map<int, Variable> variables;
			std::vector<Variable> constants;
			std::stack<Variable> callStack;
	};
	
		
	enum TokenType
	{
	/*0*/	KEYWORD,	//if, while, return
	/*1*/	SEPARATOR, 	//( ) ;
	/*2*/	OPERATOR,	//+ =
	/*3*/	COMPARATOR, //>, <, ==, &&
	/*4*/	VARNAME, 	//all the variables
	/*5*/	VARTYPE,	//float, int, double, string, var
	/*6*/	CSTSTRING,	//"a"
	/*7*/	CSTFLOAT,	//58.0
	/*8*/	CSTINT,		//5
	/*9*/	CSTBOOLEAN,	//true, false
	/*10*/	FUNCNAME,	//any function
	/*11*/	CONSTRUCTOR,//class constructors
	/*12*/	UNDEFINED	//base value
	};
	
	const std::vector<std::string> typeNames {	"Keyword", 
												"Separator", 
												"Operator", 
												"Comparator", 
												"Variable name", 
												"Variable type",
												"String constant",
												"Float constant",
												"Integer constant",
												"Boolean constant",
												"Function name",
												"Constructor",
												"Undefined"};
	
	struct Token
	{
		Rosie::TokenType type = UNDEFINED;
		std::string value;
		
		void operator+=(const char c);
		
		bool operator==(const std::string& stringValue);
		
		bool operator!=(const std::string& stringValue);
		
		int length() const;
		
		void clear();
		
		friend std::ostream& operator <<(std::ostream& os, const Token& token)
		{
			os << token.getString();
			return os;
		}
		
		void setAddress(const Address& address);
		
		Address getAddress() const;
		
		std::string getString() const;
		
		private:
			Address m_address;
	};

}
