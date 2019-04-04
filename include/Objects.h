#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <variant>
#include <stack>
#include <unordered_map>

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
			Type(const std::string& name);
			Type();
	
			void setId(const std::size_t& typeId);
			void addMember(const int& memberTypeId, const std::string& name);
			std::size_t getSize() const;
			
			std::string name;
			int id;
		
		private:
			std::unordered_map<std::size_t, int> members;
			std::size_t size;
	};
	
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
				else
				{
					os << "Error: undefined variable type: type id = "+std::to_string(var.type);
				}
				return os;
			}
			
		private:
			std::size_t type;
			std::variant<float, int, bool, std::string> value;
	};
	
	enum AddressType
	{
		INTEGER,
		VARIABLE,
		CONSTANT,
		FUNCTION,
	};
	
	struct Address
	{
		public:
			Address(const std::size_t& id, const std::size_t& category = 0, const std::string& name = "");
			Address(const Address& address);
			Address();
			
			std::size_t getId() const;
			std::string getName() const;
			std::size_t getCategory() const;
			Type getType() const;
			std::string getString() const;
			
			/*std::size_t addMember(const std::size_t& category, const std::string& name, const Type& type); //add a member, returns the size of the member added
		
			Address getMemberAddress(const std::string& name) const;
		
			Address getMemberAddress(const int& offset) const;*/
		
			std::size_t size() const;
		
		private:
			std::size_t id;
			std::string name;
			std::size_t category;
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
