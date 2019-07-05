#include "VMObjects.h"

namespace Rosie
{		
	
	Handle::Handle(const int& id, const Category& category):id(id), category(category)
	{}
	
	int Handle::getId() const
	{
		return id;
	}
	
	Category Handle::getCategory() const
	{
		return category;
	}
	
	bool Handle::operator==(const Handle& other) const
	{
		return id == other.id;
	}


	Variable::Variable(const float& floatValue)
	{
		value = floatValue;
		type = 0;
	}

	Variable::Variable(const int& integerValue)
	{
		value = (float)integerValue;
		type = 0;
	}
	
	Variable::Variable(const bool& booleanValue)
	{
		value = booleanValue;
		type = 2;
	}
	
	Variable::Variable(const std::string& stringValue)
	{
		value = stringValue;
		type = 3;
	}
	
	Variable::Variable(const TokenType& tokenType)
	{
		if(tokenType == TokenType::CSTFLOAT)
		{
			type = 0;
		}
		else if(tokenType == TokenType::CSTINT)
		{
			type = 1;
		}
		else if(tokenType == TokenType::CSTBOOLEAN)
		{
			type = 2;
		}
		else if(tokenType == TokenType::CSTSTRING)
		{
			type = 3;
		}
		else
		{
			value = std::make_shared<Object>();
			type = 4;
		}
	}
	
	Variable::Variable(const std::shared_ptr<Object>& object):value(std::shared_ptr<Object>(object))
	{
		type = 4;
	}
	
	Variable::Variable(): value(std::make_shared<Object>())
	{
		type = 4;
	}
	
	Variable::Variable(const Variable& other):value(other.value), type(other.type)
	{}
	
	Variable Variable::operator+(const Variable& other)
	{
		return Variable(std::get<0>(value)+std::get<0>(other.value));
	}
	
	Variable Variable::operator-(const Variable& other)
	{
		return Variable(std::get<0>(value)-std::get<0>(other.value));
	}
	
	Variable Variable::operator*(const Variable& other)
	{
		return Variable(std::get<0>(value)*std::get<0>(other.value));
	}
	
	Variable Variable::operator/(const Variable& other)
	{
		return Variable(std::get<0>(value)/std::get<0>(other.value));
	}
	
	Variable Variable::operator-()
	{
		return Variable(-std::get<0>(value));
	}
	
	std::string Variable::toString() const
	{
		if(type == 0)
		{
			return std::to_string(get<float>())+ " (float)"; 
		}
		else if(type == 1)
		{
			return std::to_string(get<int>())+ " (int)"; 
		}
		else if(type == 2)
		{
			return std::to_string(get<bool>())+ " (boolean)"; 
		}
		else if(type == 3)
		{
			return get<std::string>()+ " (string)"; 
		}
		else if(type == 4)
		{
			return "To be implemented";
		}
		return "";
	}
	
	
	void Object::addMember(const std::string& name, const int& type, const Handle& handle)
	{
		members.add(handle, Rosie::getId(name), Variable(TokenType(type)));
	}
	
	void Object::setMember(const Handle& handle, const Variable& variable)
	{
		members[handle] = variable;
	}
	
	Variable Object::getMember(const Handle& handle)
	{
		if(hasMember(handle))
		{
			return members[handle];	
		}
		else
		{
			if(getParent() != nullptr)
			{
				return getParent()->getMember(handle);
			}
			else
			{
				std::cout << "Variable "<< handle.getId() <<" not found." << std::endl;
				return Variable();
			}
		}
	}
	
	Variable Object::getMember(const std::string& name)
	{
		std::size_t firstDot = name.find_first_of(".");
		
		if(firstDot != std::string::npos)
		{
			std::string memberName = name.substr(0, firstDot);
			return  members[Rosie::getId(memberName)].get<std::shared_ptr<Object>>()->getMember(name.substr(firstDot+1, name.length()));
		}
		else
		{
			return members[Rosie::getId(name)];
		}
	}
	
	bool Object::hasMember(const Handle& handle) const
	{
		return members.contains(handle);
	}
			
	std::string Object::toString() const
	{
		return "Object";
	}	
	
	void Object::setParent(const std::shared_ptr<Object> parent)
	{
		m_parent = std::weak_ptr<Object>(parent);
	}
	
	std::shared_ptr<Object> Object::getParent() const
	{
		return m_parent.lock();
	}
	
	
	Variable CallStack::pop()
	{
		Variable top = queue.top();
		queue.pop();
		return top;
	}
	
	Variable CallStack::peek()
	{
		return queue.top();
	}
	
	void CallStack::push(const Variable& variable)
	{
		queue.push(variable);
	}
	
	bool CallStack::empty() const
	{
		return queue.empty();
	}
			
			

	State::State(const std::string& fileName):fileName(fileName), activeScope(std::make_shared<Object>())
	{}
	
	void State::addVariable(const std::string& name, const int& tokenType, const Handle& handle)
	{
		activeScope->addMember(name, tokenType, handle);
	}
	
	void State::addFunction(const int& id, const std::string& name)
	{
		functions.add(id, name, Function<CallStack&>(name, id));
	}
	
	void State::addConstant(const Variable& csts)
	{
		constants.push_back(csts);
	}
	
	void State::setFunction(const std::string& name, const std::function<void(CallStack&)>& func)
	{
		functions[name].setFunction(func);
	}
	
	void State::copyVariable(Handle& dest, const Handle& src)
	{
		activeScope->setMember(dest, getVariable(src));
	}
	
	Variable State::getVariable(const Handle& handle)
	{
		if(handle.getCategory() == Category::CONSTANT)
		{
			return constants[handle.getId()];			
		}
		else if(handle.getCategory() == Category::VARIABLE)
		{
			if(handle.getId() == 0)
			{
				return callStack.pop();
			}
			else
			{
				return activeScope->getMember(handle);	
			}
		}
		else if(handle.getCategory() == Category::INTEGER)
		{
			return Variable(handle.getId());
		}
		else //to be implemented
		{
			return Variable(handle.getId());
		}
	}
	
	void State::execute(const int& functionId)
	{
		functions[functionId].execute(callStack);	
	}
	
	std::string State::toString() const
	{
		std::string res = "[";
		/*for(Variable var : variables.getValues())
		{
			res += var.toString() + ", ";
		}*/
		return res+"]";
	}
	
	void State::push(const Variable& variable)
	{
		callStack.push(variable);
	}
	
	void State::push(const Handle& handle)
	{
		push(getVariable(handle));
	}
	
	std::vector<std::string> State::getFunctionsList() const
	{
		return functions.getKey2();
	}
	
	std::string State::getFileName() const
	{
		return fileName;
	}
	
	void State::startScope(const Handle& handle)
	{
		scopeStack.push(activeScope);
		std::shared_ptr<Object> oldScope(activeScope);
		activeScope = std::move(getVariable(handle).get<std::shared_ptr<Object>>());
		activeScope->setParent(oldScope);
	}
	
	void State::endScope()
	{
		activeScope = std::move(scopeStack.top());
		scopeStack.pop();
	}
}
