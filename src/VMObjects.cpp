#include "VMObjects.h"

namespace Rosie
{	
	/*	
	Id::Id(const std::vector<int>& ids):m_id(ids)
	{}
	
	Id::Id(const std::string& textId)
	{
		for(const std::string& id : Rosie::split(textId, "."))
		{
			m_id.push_back(id);
		}
	}*/
	
	/*std::string Id::toString() const
	{
		std::string res = std::to_string(m_id[0]);
		for(int i = 1; i<m_id.size(); i++)
		{
			res+="."+std::to_string(m_id[i]);
		}
		return res;
	}*/
	
	Handle::Handle(const int& id, const Category& category):category(category)
	{
		m_id.push_back(id);
	}
	
	Handle::Handle(const std::vector<int>& ids, const Category& category):m_id(ids), category(category)
	{}
	
	Handle::Handle(const std::string& textId, const Category& category):category(category)
	{
		for(const std::string& id : Rosie::split(textId, "."))
		{
			m_id.push_back(std::stoi(id));
		}
	}
	
	int Handle::getId() const
	{
		return m_id[0];
	}
	
	Category Handle::getCategory() const
	{
		return category;
	}
	
	bool Handle::operator==(const Handle& other) const
	{
		return m_id == other.m_id;
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
			value = std::make_shared<Scope>();
			type = 4;
		}
	}
	
	Variable::Variable(const std::shared_ptr<Scope>& object):value(std::shared_ptr<Scope>(object))
	{
		type = 4;
	}
	
	Variable::Variable(): value(std::make_shared<Scope>())
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
			return get<std::shared_ptr<Scope>>()->toString();
		}
		return "";
	}

	
	void Scope::addMember(const std::string& name, const int& type, const Handle& handle)
	{
		members.add(handle, Rosie::getId(name), Variable(TokenType(type)));
	}
	
	void Scope::setMember(const Handle& handle, const Variable& variable)
	{
		members[handle] = variable;
	}
	
	Variable Scope::getMember(const Handle& handle)
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
	
	Variable Scope::getMember(const std::string& name)
	{
		std::size_t firstDot = name.find_first_of(".");
		
		if(firstDot != std::string::npos)
		{
			std::string memberName = name.substr(0, firstDot);
			return  members[Rosie::getId(memberName)].get<std::shared_ptr<Scope>>()->getMember(name.substr(firstDot+1, name.length()));
		}
		else
		{
			return members[Rosie::getId(name)];
		}
	}
	
	std::shared_ptr<Scope> Scope::getScope(const std::string& name)
	{
		return members[Rosie::getId(name)].get<std::shared_ptr<Scope>>();
	}
	
	bool Scope::hasMember(const Handle& handle) const
	{
		return members.contains(handle);
	}
			
	std::string Scope::toString() const
	{
		std::string res = "[";
		for(const Variable& member : members.getValues())
		{
			res+=member.toString();
		}
		return res+"]";
	}	
	
	void Scope::setParent(const std::shared_ptr<Scope> parent)
	{
		m_parent = std::weak_ptr<Scope>(parent);
	}
	
	std::shared_ptr<Scope> Scope::getParent() const
	{
		return m_parent.lock();
	}
	
	void Scope::copyMembers(const std::shared_ptr<Scope>& object)
	{
		members = object->members;
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
			
			

	State::State(const std::string& fileName):fileName(fileName), activeScope(std::make_shared<Scope>())
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
		std::shared_ptr<Scope> oldScope(activeScope);
		activeScope = std::move(getVariable(handle).get<std::shared_ptr<Scope>>());
		activeScope->setParent(oldScope);
	}
	
	void State::endScope()
	{
		activeScope = std::move(scopeStack.top());
		scopeStack.pop();
	}
	
	std::shared_ptr<Scope> State::getActiveScope() const
	{
		return activeScope;
	}
}
