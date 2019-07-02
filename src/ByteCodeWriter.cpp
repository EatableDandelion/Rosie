#include <ByteCodeWriter.h>

namespace Rosie
{
	void error(const std::string& text, const Lexer& lexer)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Error line "<<std::to_string(lexer.getLineIndex())<<", at token ["<<lexer.getToken()<<"]:"<< std::endl;
		std::cout << lexer.getLine() << std::endl;
		std::cout << text << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
	}
	
	Memory::Memory():variables(Category::VARIABLE,1), functions(Category::FUNCTION, 1), scopePrefix("")
	{}
	
	Address Memory::getAddress(const Token& token, const Lexer& lexer)
	{
		if(hasFunctionAddress(token))
		{
			return getFunctionAddress(token, lexer);
		}
		else if(hasVarAddress(token))
		{
			return getVarAddress(token);
		}
		else
		{
			return newCstAddress(token);
		}
	}
	
	Address Memory::newCstAddress(const Token& token)
	{
		std::string value = token.value;
		TokenType type = token.type;
		
		Address address(constants.size(), Category::CONSTANT, value);
		address.setType(type);
		
		constants.push_back(Constant(type, value));

		return address;//Constants have a negative index to differentiate from addresses.	
	}
	
	Address Memory::newVarAddress(const std::string& name, const TokenType& type)
	{
		Address newAddress = variables.newAddress(rename(name), type);
		return newAddress;
	}
	
	Address Memory::newVarAddress(const Token& token, const TokenType& type)
	{
		return newVarAddress(token.value, type);
	}
	
	Address Memory::getVarAddress(const Token& token)
	{
		for(std::string possibleName : getPossibleNames(token.value))
		{
			if(variables.hasAddress(possibleName))
			{
				return variables.getAddress(possibleName);
			}
		}
		
		std::cout << "Error, variable " << token << " undefined." <<std::endl;
		return Address(0, Category::VARIABLE);
	}
	
	bool Memory::hasVarAddress(const Token& token)
	{
		for(std::string possibleName : getPossibleNames(token.value))
		{
			if(variables.hasAddress(possibleName))
			{
				return true;
			}
		}
		return false;
	}
	
	Address Memory::setAddress(const Token& destToken, const Address& srcAddress)
	{
		Address destAddress;
		if(hasVarAddress(destToken))
		{
			destAddress = getVarAddress(destToken);
		}
		else
		{
			destAddress = newVarAddress(destToken, srcAddress.getType());
		}
		addInstruction<SetInstruction>(destAddress, srcAddress);
		
		return destAddress;
	}
	
	Address Memory::newFunctionAddress(const std::string& name)
	{
		return functions.newAddress(name);
	}
	
	Address Memory::getFunctionAddress(const Token& token, const Lexer& lexer)
	{
		if(!hasFunctionAddress(token))
		{
			Rosie::error("Error, function " + token.getString() + " undefined.", lexer);
		}
		return functions.getAddress(token.value);
	}
	
	bool Memory::hasFunctionAddress(const Token& token)
	{
		return functions.hasAddress(token.value);
	}
	
	std::vector<Constant> Memory::getConstants() const
	{
		return constants;
	}
		
	std::vector<Address> Memory::getVariables() const
	{
		return variables.getAddresses();
	}
		
	std::vector<Address> Memory::getFunctions() const
	{
		return functions.getAddresses();
	}
	
	std::vector<std::string> Memory::getCommands() const
	{
		return instructions.getCommands();
	}
	
	void Memory::setArguments(std::stack<Address>& activeStack, const int& nbArgs)
	{
		if(nbArgs == -1)
		{
			while(!activeStack.empty())
			{
				addInstruction<ArgumentInstruction>(activeStack.top());
				activeStack.pop();
			}
		}
		else
		{
			for(int i = 0; i< nbArgs; i++)
			{
				Address arg = activeStack.top();
				activeStack.pop();	
				addInstruction<ArgumentInstruction>(arg);
			}
		}
	}
	
	void Memory::callFunction(std::stack<Address>& activeStack, const Token& token, const Lexer& lexer, const TokenType& returnType)
	{
		addInstruction<CallInstruction>(getFunctionAddress(token, lexer).getId());
		Address stackAddress = Address(0, Category::VARIABLE);
		stackAddress.setType(returnType);
		activeStack.push(stackAddress);
	}
	
	void Memory::startScope(const Address& destAddress)
	{
		scopePrefix = destAddress.getName();
		
		std::size_t dotPos = scopePrefix.find_last_of(".");
		if(dotPos == std::string::npos)
		{
			scopes.push_back(scopePrefix);
		}
		else
		{
			scopes.push_back(scopePrefix.substr(dotPos+1, scopePrefix.length()));
		}
		
	}
		
	void Memory::endScope()
	{
		scopes.pop_back();
		std::size_t dotPos = scopePrefix.find_last_of(".");
		if(dotPos == std::string::npos)
		{
			scopePrefix = "";
		}
		else
		{
			scopePrefix = scopePrefix.substr(0, dotPos);
		}
		
	}
	
	std::string Memory::rename(const std::string& name) const
	{
		if(scopePrefix == "")
		{
			return name;
		}
		else
		{
			return scopePrefix+"."+name;
		}
	}
	
	std::vector<std::string> Memory::getPossibleNames(const std::string& name) const
	{
		std::vector<std::string> res;
		res.push_back(name);
		std::deque<std::string> scopesCopy = scopes;
		std::string possibleName = "";
		while(!scopesCopy.empty())
		{
			possibleName += scopesCopy.front()+".";
			res.push_back(possibleName+name);
			scopesCopy.pop_front();
		}
		return res;
	}
	
	
	Program::Program():memory(std::make_shared<Memory>())
	{}
	
	void Program::startScope(const Address& destAddress)
	{
		//addInstruction<ScopeInstruction>(destAddress);
		memory->startScope(destAddress);
	}
	
	void Program::endScope()
	{
		//addInstruction<ScopeInstruction>();
		memory->endScope();
	}
	
	Address Program::getStackAddress() const
	{
		return Address(0, Category::VARIABLE);
	}
	
	Address Program::getStackAddress(const TokenType& type)
	{
		Address res = getStackAddress();
		res.setType(type);
		return res;
	}
	
	
	
	HeaderWriter::HeaderWriter(const std::string& fileName):fileName(fileName)
	{}
	
	void HeaderWriter::write(Program& program)
	{	
		std::ofstream file;
		file.open(fileName);
		
		for(Constant constant : program->getConstants())
		{
			file << 0 << " " << constant.getValue() << " " << constant.getTypeId() << std::endl;
		}
		
		for(Address variable : program->getVariables())
		{
			file << 1 << " " << variable.getName() << " " << variable.getId() << " " << variable.getTypeId() << std::endl;
		}
		
		for(Address function : program->getFunctions())
		{
			file << 2 << " " << function.getName() << " " << function.getId() << std::endl;
		}
		
		file.close();
	}
	
	
	
	HeaderReader::HeaderReader(const std::string& fileName):fileName(fileName), cstIndex(0)
	{}
	
	void HeaderReader::read(State& state)
	{
		std::string line;
		std::ifstream file(fileName);
		int typeId = 0;
		if(file.is_open())
		{
			while(getline(file,line))
			{
				int category;
				char name[20];
				int int1;
				int int2;
				sscanf(line.c_str(), "%d %s %d %d", &category, &name, &int1, &int2);
				
				if(category == 0) // It's a constant
				{
					defineConstant(state, std::string(name), int1);
				}
				else if(category == 1) // It's a variable
				{
					defineVariable(state, std::string(name), int1, int2);
				}
				else if(category == 2) // It's a function
				{
					defineFunction(state, std::string(name), int1);
				}
				
				std::cout << line << std::endl;
			}
			file.close();
		}

	}
	
	void HeaderReader::defineConstant(State& state, const std::string& value, const int& typeId)
	{
		TokenType type = TokenType(typeId);
		
		if(type == TokenType::CSTINT)
		{			
			state.addConstant(cstIndex, Variable(std::stoi(value)));
		}
		else if(type == TokenType::CSTFLOAT)
		{
			state.addConstant(cstIndex, Variable(std::stof(value)));
		}
		else if(type == TokenType::CSTBOOLEAN)
		{
			if(value == "true")
			{
				state.addConstant(cstIndex, Variable(true));
			}
			else
			{
				state.addConstant(cstIndex, Variable(false));
			}
		}
		else if(type == TokenType::CSTSTRING)
		{
			state.addConstant(cstIndex, Variable(value));
		}
		cstIndex++;
	}
	
	void HeaderReader::defineVariable(State& state, const std::string& name, const int& id, const int& typeId) const
	{
		state.addVariable(name, typeId, Handle(id, Category::VARIABLE));
	}
	
	void HeaderReader::defineFunction(State& state, const std::string& name, const int& id) const
	{
		state.addFunction(id, name);
	}
	
	
	ByteCodeWriter::ByteCodeWriter(const std::string& fileName):fileName(fileName)
	{}
	
	void ByteCodeWriter::write(Program& program) const
	{	
		std::ofstream file;
		file.open(fileName);
		
		for(std::string command : program->getCommands())
		{
			file << command << std::endl;
		}
		
		file.close();
	}
	
	
	
	
	
	ByteCodeReader::ByteCodeReader(const std::string& fileName, const Syntax& syntax):fileName(fileName)
	{
		addInstruction<SetInstruction>();
		addInstruction<ArgumentInstruction>();
		addInstruction<CallInstruction>(syntax);
		addInstruction<ScopeInstruction>();
	}
	
	void ByteCodeReader::read(State& state) const
	{
		std::string command;
		std::ifstream file(fileName);
		int instructionId = 0;
		if(file.is_open())
		{
			while(getline(file,command))
			{
			  	instructionId = std::stoi(command.substr(std::size_t(0), command.find("|", std::size_t(0))));

				if(instructions.find(instructionId) != instructions.end())
				{
					std::cout << command << std::endl;
					std::cout << instructions.at(instructionId)->getName() << std::endl;
					std::cout << std::endl;
					instructions.at(instructionId)->read(command.substr(command.find("|", std::size_t(0))+1, command.size()), state);
				}
				else
				{
					std::cout << "Instruction "+std::to_string(instructionId)+" unknown." << std::endl;
				}
			}
			file.close();
		}

	}
}
