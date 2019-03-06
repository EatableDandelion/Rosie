#include "Instruction.h"

namespace Rosie
{
	Address::Address(const std::size_t& location, const AddressType& type):id(location), type(type)
	{}
	
	Address::Address(const Address& address):id(address.id), type(address.type)
	{}
	
	Address::Address():id(0), type(AddressType::VARIABLE)
	{}
	
	Variable::Variable(const float& floatValue):type(0)
	{
		f = floatValue;
	}

	Variable::Variable(const int& integerValue):type(1)
	{
		i = integerValue;
	}
	
	Variable::Variable(const bool& booleanValue):type(2)
	{
		b = booleanValue;
	}
	
	Variable::Variable()
	{}
	
	Instruction::Instruction(const std::shared_ptr<Instruction>& parent):m_parent(std::weak_ptr<Instruction>(parent))
	{
		id = parent->id+1;
	}
			
	Instruction::Instruction()
	{
		id = 0;
	}
	
	std::shared_ptr<Instruction> Instruction::next(std::shared_ptr<Instruction>& nextInstruction)
	{
		nextInstruction->m_parent = m_parent;
		return m_parent.lock()->sub(nextInstruction);
	}
			
	std::shared_ptr<Instruction> Instruction::sub(std::shared_ptr<Instruction>& child)
	{
		m_arguments.push_back(child);
		return child;
	}

	void Instruction::addToken(const Token& token)
	{
		tokens.push_back(token);
	}
	
	std::shared_ptr<Instruction> Instruction::getRoot()
	{
		if(auto shrptr = m_parent.lock())//If not null
		{
			return shrptr->getRoot();
		}
		else
		{
			return shared_from_this();
		}
	}
	
	void Instruction::print() const
	{
		for(Token token : tokens)
		{
			std::cout << token.value << " ";
		}
		std::cout << std::endl;
		
		for(std::shared_ptr<Instruction> child : m_arguments)
		{
			child->print();
		}
	}
}