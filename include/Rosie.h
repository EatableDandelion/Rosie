#pragma once

#include "Interpreter.h"
#include "Instructions.h"

namespace Rosie{	
	class RosieVM
	{
		public:
			RosieVM(const std::string& fileName, const Syntax& syntax);
			
			void compile();
			
			void run();
			
			template<typename T>
			T getValue(const std::string& name)
			{
				return state.getValue<T>(name);
			}

		private:
			std::string fileName;
			State state;
			Syntax syntax;
	};
}
