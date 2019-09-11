#include "VMObjects.h"

namespace Rosie
{	
	Handle::Handle(const int& id, const Category& category):m_id(std::make_shared<AddressId>(id)), category(category)
	{}
	
	Handle::Handle(const std::initializer_list<int>& id, const Category& category):m_id(std::make_shared<AddressId>(id)), category(category)
	{}
	
	Handle::Handle(const std::string& textId, const Category& category):m_id(std::make_shared<AddressId>(textId)), category(category)
	{}
	
	std::shared_ptr<AddressId> Handle::operator->() const
	{
		return m_id;
	}
	
	Category Handle::getCategory() const
	{
		return category;
	}
	
	bool Handle::operator==(const Handle& other) const
	{
		return m_id == other.m_id;
	}


	NullPointerError::NullPointerError(const std::string& variableName):varName(variableName)
	{}

	const char*  NullPointerError::what() const throw()
	{
		ErrorDisplay display;
		display.show("Null pointer exception:");
		display.show(varName+" is undefined.");
		return "";
	}

	Variable::Variable(const float& floatValue, const std::string& name):defined(true), name(name)
	{
		value = floatValue;
		type = 0;
	}

	Variable::Variable(const int& integerValue, const std::string& name):defined(true), name(name)
	{
		value = (float)integerValue;
		type = 0;
	}
	
	Variable::Variable(const bool& booleanValue, const std::string& name):defined(true), name(name)
	{
		value = booleanValue;
		type = 2;
	}
	
	Variable::Variable(const std::string& stringValue, const std::string& name):defined(true), name(name)
	{
		value = stringValue;
		type = 3;
	}
	
	Variable::Variable(const TokenType& tokenType, const std::string& name):defined(false), name(name)
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
	
	Variable::Variable(const std::shared_ptr<Scope>& object, const std::string& name):value(std::shared_ptr<Scope>(object)), defined(true), name(name)
	{
		type = 4;
	}
	
	Variable::Variable(): value(std::make_shared<Scope>()), defined(true)
	{
		type = 4; 
	}
	
	Variable::Variable(const Variable& other):value(other.value), type(other.type), defined(other.defined), name(other.name)
	{}
	
	Variable& Variable::operator=(const Variable& other)
	{
		value = other.value;
		type = other.type;
		defined = other.defined;
		return *this;
	}
	
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
	
	void Variable::addMember(const std::string& name, const int& type, const Handle& handle, const int& depth)
	{
		get<std::shared_ptr<Scope>>()->addMember(name, type, handle, depth);
	}
	
	void Variable::setMember(const Handle& handle, const Variable& variable, const int& depth)
	{
		get<std::shared_ptr<Scope>>()->setMember(handle, variable, depth);
	}
	
	Variable Variable::getMember(const Handle& handle, const int& depth)
	{
		return get<std::shared_ptr<Scope>>()->getMember(handle, depth);
	}
	
	bool Variable::hasMember(const Handle& handle, const int& depth) const
	{
		return get<std::shared_ptr<Scope>>()->hasMember(handle, depth);
	}
	
	std::string Variable::toString() const
	{
		if(type == 0)
		{
			return name+" = "+std::to_string(get<float>())+ " (float)"; 
		}
		else if(type == 1)
		{
			return name+" = "+std::to_string(get<int>())+ " (int)"; 
		}
		else if(type == 2)
		{
			return name+" = "+std::to_string(get<bool>())+ " (boolean)"; 
		}
		else if(type == 3)
		{
			return name+" = "+get<std::string>()+ " (string)"; 
		}
		else if(type == 4)
		{
			return name+" = "+get<std::shared_ptr<Scope>>()->toString();
		}
		return "";
	}
	
	std::string Variable::getName() const
	{
		return name;
	}

	
	void Scope::addMember(const std::string& name, const int& type, const Handle& handle, const int& depth)
	{
		if(handle->getDepth() > depth)
		{
			members[handle->getIndex(depth)].addMember(name, type, handle, depth+1);
		}
		else if(handle->getDepth() == depth)
		{	
			members.add(handle->getIndex(depth), Rosie::getId(getShortName(name)), Variable(TokenType(type), name));
		}
	}
	
	void Scope::setMember(const Handle& handle, const Variable& variable, const int& depth)
	{
		if(handle->getDepth() > depth)
		{
			members[handle->getIndex(depth)].setMember(handle, variable, depth+1);
		}
		else if(handle->getDepth() == depth)
		{
			members[handle->getIndex(depth)] = variable;
		}
	}
	
	Variable Scope::getMember(const Handle& handle, const int& depth)
	{	
		if(handle->getDepth() > depth)
		{
			return members[handle->getIndex(depth)].getMember(handle, depth+1);
		}
		else
		{
			if(hasMember(handle, depth))
			{
				return members[handle->getIndex(depth)];	
			}
			else
			{
				if(getParent() != nullptr)
				{
					return getParent()->getMember(handle, depth-1);
				}
				else
				{
					std::cout << "Variable "<< handle->toString() <<" not found." << std::endl;
					return Variable();
				}
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
	
	bool Scope::hasMember(const Handle& handle, const int& depth) const
	{
		if(handle->getDepth() > depth)
		{
			return members[handle->getIndex(depth)].hasMember(handle, depth+1);
		}
		else if(handle->getDepth() == depth)
		{
			return members.contains(handle->getIndex(depth));
		}
		else
		{
			return false;
		}
	}
			
	std::string Scope::toString() const
	{
		std::string res = "[";
		bool hasMember = false;
		for(const Variable& member : members.getValues())
		{
			res+=member.toString()+", ";
			hasMember = true;
		}
		if(hasMember)
		{
			res.pop_back();
			res.pop_back();//remove last comma.
		}
		return res+"]";
	}	
	
	void Scope::setParent(const std::shared_ptr<Scope> parent)
	{
		m_parent = std::weak_ptr<Scope>(parent);
	}
	
	std::string Scope::getShortName(const std::string& longName) const
	{
		std::size_t lastDot = longName.find_last_of(".");
		
		if(lastDot != std::string::npos)
		{
			return longName.substr(lastDot+1, longName.length());
		}
		else
		{
			return longName;
		}
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
			
			

	State::State(const std::string& fileName):fileName(fileName), root(std::make_shared<Scope>())
	{}
	
	void State::addVariable(const std::string& name, const int& tokenType, const Handle& handle)
	{
		root->addMember(name, tokenType, handle, 0);
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
		root->setMember(dest, getVariable(src), 0);
	}
	
	Variable State::getVariable(const Handle& handle)
	{
		if(handle.getCategory() == Category::CONSTANT)
		{
			return constants[handle->getIndex(0)];			
		}
		else if(handle.getCategory() == Category::VARIABLE)
		{
			if(handle->getIndex(0) == 0)
			{
				return callStack.pop();
			}
			else
			{
				return root->getMember(handle, 0);	
			}
		}
		else if(handle.getCategory() == Category::INTEGER)
		{
			return Variable(handle->getIndex(0));
		}
		else //to be implemented
		{
			return Variable(handle->getIndex(0));
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
	
	std::shared_ptr<Scope> State::getActiveScope() const
	{
		return root;
	}
}
