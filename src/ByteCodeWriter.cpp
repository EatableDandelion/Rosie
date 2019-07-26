#include <ByteCodeWriter.h>

namespace Rosie
{
	
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
		
		addDeclaration<ConstantHeader>(value, type);
		
		return address;//Constants have a negative index to differentiate from addresses.	
	}
	
	Address Memory::newVarAddress(const std::string& name, const TokenType& type)
	{
		Address newAddress = variables.newAddress(rename(name), type);
		addDeclaration<VariableHeader>(newAddress.getId(), name, type);
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
		
		throw BaseError("Error, variable " + token.getString() + " undefined.");

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
	
	Address Memory::setAddress(const std::string& newName, const Address& srcAddress)
	{
		Address destAddress = newVarAddress(newName, srcAddress.getType());
		addInstruction<SetInstruction>(destAddress, srcAddress);
		return destAddress;
	}
	
	/*Address Memory::addToCollection(const Token& collectionToken, const int& index, const Address& srcAddress)
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
		addInstruction<CollectionAddInstruction>(destAddress, index, srcAddress);
	}*/
	
	Address Memory::newFunctionAddress(const std::string& name)
	{
		Address res = functions.newAddress(name);
		addDeclaration<FunctionHeader>(res.getId(), name);
		return res;
	}
	
	Address Memory::getFunctionAddress(const Token& token, const Lexer& lexer)
	{
		if(!hasFunctionAddress(token))
		{
			throw SyntaxError("Error, function " + token.getString() + " undefined.", lexer);
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
	
	std::vector<std::string> Memory::getHeader() const
	{
		return header.getCommands();
	}
	
	void Memory::setArgument(const Token& token, Lexer& lexer) //Set args for the array like {a, 2.0}
	{
		//TODO, uniformize the function argument with the array argument
		if(!scopeArgIndex.empty())
		{
			Address argAddress = getAddress(token, lexer);
			setAddress(std::to_string(scopeArgIndex.top()), argAddress);
			scopeArgIndex.top()++;
		}
	}
	
	void Memory::setArguments(std::stack<Address>& activeStack, const int& nbArgs) //Set args for the functions, like print
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
		addDeclaration<ScopeInstruction>(destAddress);
		addInstruction<ScopeInstruction>(destAddress);
		
		scopeArgIndex.push(0);
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
		addDeclaration<ScopeInstruction>();
		addInstruction<ScopeInstruction>();
		
		scopeArgIndex.pop();
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
	
	
	Program::Program(const std::string& fileName, const std::vector<std::string>& nativeFunctions):memory(std::make_shared<Memory>()), functionNames(nativeFunctions), fileName(fileName)
	{}
	
	void Program::startScope(const Address& destAddress)
	{
		memory->startScope(destAddress);
	}
	
	void Program::endScope()
	{
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
	
	bool Program::hasFunction(const std::string& name)
	{
		return std::find(functionNames.begin(), functionNames.end(), name) != functionNames.end();
	}
	
	std::string Program::getFileName() const
	{
		return fileName;
	}
	
	
	ByteCodeWriter::ByteCodeWriter(const std::string& fileName, const std::string& extension):fileName(fileName+extension)
	{}
	
	void ByteCodeWriter::write(const std::vector<std::string>& lines) const
	{	
		std::ofstream file;
		file.open(fileName);
		
		for(std::string command : lines)
		{
			file << command << std::endl;
		}
		
		file.close();
	}
	
	
	
	
	
	ByteCodeReader::ByteCodeReader(const std::string& extension, const bool& verbose):extension(extension), verbose(verbose)
	{}
	
	void ByteCodeReader::read(State& state) const
	{
		std::string command;
		std::ifstream file(state.getFileName()+extension);
		int instructionId = 0;
		if(file.is_open())
		{
			while(getline(file,command))
			{
			  	instructionId = std::stoi(command.substr(std::size_t(0), command.find("|", std::size_t(0))));

				if(instructions.find(instructionId) != instructions.end())
				{
					std::cout << command;
					if(verbose)
					{
						std::cout << "\t" << "\t" << instructions.at(instructionId)->getName();
					}
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
