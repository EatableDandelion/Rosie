#include "VMObjects.h"

namespace Rosie
{
	void IVariable::addVariable(const std::string& name, const int& type, const Handle& handle){}
	void IVariable::addFunction(const int& id, const std::string& name){}
	std::shared_ptr<IVariable> IVariable::add(const std::shared_ptr<IVariable> other){return nullptr;}
	std::shared_ptr<IVariable> IVariable::subtract(const std::shared_ptr<IVariable> other){return nullptr;}
	std::shared_ptr<IVariable> IVariable::multiply(const std::shared_ptr<IVariable> other){return nullptr;}
	std::shared_ptr<IVariable> IVariable::divide(const std::shared_ptr<IVariable> other){return nullptr;}
	std::shared_ptr<IVariable> IVariable::negate(){return nullptr;}

	
	Variable::Variable(const float& floatValue):variable(std::make_shared<Primitive>(floatValue))
	{}

	Variable::Variable(const int& integerValue):variable(std::make_shared<Primitive>(integerValue))
	{}
	
	Variable::Variable(const bool& booleanValue):variable(std::make_shared<Primitive>(booleanValue))
	{}
	
	Variable::Variable(const std::string& stringValue):variable(std::make_shared<Primitive>(stringValue))
	{}
	
	Variable::Variable(const TokenType& tokenType):variable(std::make_shared<Primitive>(tokenType))
	{}
	
	Variable::Variable(const std::shared_ptr<IVariable>& variable):variable(std::move(variable))
	{}
	
	Variable::Variable():variable(std::make_shared<CompositeVariable>())
	{}
	
	Variable Variable::operator+(const Variable& other)
	{
		return Variable(variable->add(other.variable));
	}
	
	Variable Variable::operator-(const Variable& other)
	{
		return Variable(variable->subtract(other.variable));
	}
	
	Variable Variable::operator*(const Variable& other)
	{
		return Variable(variable->multiply(other.variable));
	}
	
	Variable Variable::operator/(const Variable& other)
	{
		return Variable(variable->divide(other.variable));
	}
	
	Variable Variable::operator-()
	{
		return Variable(variable->negate());
	}
	
	void Variable::addVariable(const std::string& name, const int& type, const Handle& handle)
	{
		variable->addVariable(name, type, handle);
	}
	
	void Variable::addFunction(const int& id, const std::string& name)
	{
		variable->addFunction(id, name);
	}	
	
	std::shared_ptr<IVariable> Variable::getVariable() const
	{
		return variable;
	}
	
	Primitive::Primitive(const float& floatValue)
	{
		set(floatValue);
	}

	Primitive::Primitive(const int& integerValue)
	{
		set(integerValue);
	}
	
	Primitive::Primitive(const bool& booleanValue)
	{
		set(booleanValue);
	}
	
	Primitive::Primitive(const std::string& stringValue)
	{
		set(stringValue);
	}
	
	Primitive::Primitive(const TokenType& tokenType)
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
			type = 4;
		}
	}
	
	Primitive::Primitive()
	{
		type = 4;
	}
	
	void Primitive::set(const float& newValue)
	{
		value = newValue;
		type = 0;
	}
	
	void Primitive::set(const int& newValue)
	{
		set((float)newValue);
	}
	
	void Primitive::set(const bool& newValue)
	{
		value = newValue;
		type = 2;
	}
	
	void Primitive::set(const std::string& newValue)
	{
		value = newValue;
		type = 3;
	}
	
	void Primitive::set(const Primitive& other)
	{
		value = other.value;
		type = other.type;
	}
	
	std::shared_ptr<IVariable> Primitive::add(const std::shared_ptr<IVariable> other_t)
	{/** This is ugly but running out of ideas here... */
		if(std::shared_ptr<Primitive> other = std::dynamic_pointer_cast<Primitive>(other_t))
		{
			return std::make_shared<Primitive>(Primitive(std::get<0>(value)+std::get<0>(other->value)));
		}
		return nullptr;
	}
	
	std::shared_ptr<IVariable> Primitive::subtract(const std::shared_ptr<IVariable> other_t)
	{
		if(std::shared_ptr<Primitive> other = std::dynamic_pointer_cast<Primitive>(other_t))
		{
			return std::make_shared<Primitive>(Primitive(std::get<0>(value)-std::get<0>(other->value)));
		}
		return nullptr;
	}
	
	std::shared_ptr<IVariable> Primitive::multiply(const std::shared_ptr<IVariable> other_t)
	{
		if(std::shared_ptr<Primitive> other = std::dynamic_pointer_cast<Primitive>(other_t))
		{
			return std::make_shared<Primitive>(Primitive(std::get<0>(value)*std::get<0>(other->value)));
		}
		return nullptr;
	}
	
	std::shared_ptr<IVariable> Primitive::divide(const std::shared_ptr<IVariable> other_t)
	{
		if(std::shared_ptr<Primitive> other = std::dynamic_pointer_cast<Primitive>(other_t))
		{
			return std::make_shared<Primitive>(Primitive(std::get<0>(value)/std::get<0>(other->value)));
		}
		return nullptr;
	}
	
	std::shared_ptr<IVariable> Primitive::negate()
	{
		return std::make_shared<Primitive>(Primitive(-std::get<0>(value)));
	}
	
	std::string Primitive::toString() const
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
		return "";
		/*else if(var.type == 4)
		{
			os << "[";
			for(const auto& member : var.members)
			{
				os << *(member.second) << ", ";
			}
			os << "]";
		}*/
	}
	
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
			
			

	
	void CompositeVariable::addVariable(const std::string& name, const int& tokenType, const Handle& handle)
	{
		variables.add(handle, Rosie::getId(name), Variable(TokenType(tokenType)));
	}
	
	void CompositeVariable::addFunction(const int& id, const std::string& name)
	{
		functions.add(id, name, Function<CallStack&>(name, id));
	}
	
	void CompositeVariable::addConstant(const int& id, const Variable& csts)
	{
		constants[id] = csts;
	}
	
	void CompositeVariable::setFunction(const std::string& name, const std::function<void(CallStack&)>& func)
	{
		functions[name].setFunction(func);
	}
	
	void CompositeVariable::copyVariable(Handle& dest, const Handle& src)
	{	
		variables[dest] = getVariable(src);
	}
	
	Variable CompositeVariable::getVariable(const Handle& handle)
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
				if(!variables.contains(handle))
				{
					variables.add(handle, std::size_t(0), Variable());
				}
				return variables[handle];				
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
	
	void CompositeVariable::execute(const int& functionId)
	{
		functions[functionId].execute(callStack);	
	}
	
	std::string CompositeVariable::toString() const
	{
		/*std::string res = "[";
		for(const auto& member : var.members)
		{
			res += member.second->toString() + ", ";
		}
		return res+"]";*/
		return "fdsa";
	}
	
	void CompositeVariable::push(const Variable& variable)
	{
		callStack.push(variable);
	}
	
	void CompositeVariable::push(const Handle& handle)
	{
		push(getVariable(handle));
	}
	
	State::State():scope(std::make_shared<CompositeVariable>())
	{}
	
	void State::addVariable(const std::string& name, const int& tokenType, const Handle& handle)
	{
		scope->addVariable(name, tokenType, handle);
	}
	
	void State::addFunction(const int& id, const std::string& name)
	{
		scope->addFunction(id, name);
	}
	
	void State::addConstant(const int& id, const Variable& csts)
	{
		scope->addConstant(id, csts);
	}
	
	void State::setFunction(const std::string& name, const std::function<void(CallStack&)>& func)
	{
		scope->setFunction(name, func);
	}
	
	void State::push(const Variable& variable)
	{
		scope->push(variable);
	}
	
	void State::push(const Handle& handle)
	{
		scope->push(handle);
	}
	
	void State::copyVariable(Handle& dest, const Handle& src)
	{	
		scope->copyVariable(dest, src);
	}
	
	Variable State::getVariable(const Handle& handle)
	{
		return scope->getVariable(handle);
	}
	
	void State::execute(const int& functionId)
	{
		scope->execute(functionId);	
	}
	
	void State::startScope(const Handle& handle)
	{
		//Variable newScope = getVariable(handle);
		//scope = newScope;
	}
	
	void State::endScope()
	{
		
	}
}
