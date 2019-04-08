#include "Utils.h"

namespace Rosie
{
	std::size_t getId(const std::string& name)
	{
		return std::hash<std::string>{}(name);
	}
}
