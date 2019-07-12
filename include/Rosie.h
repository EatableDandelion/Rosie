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
			
			/*template<typename T>
			T get(const std::string& name)
			{
				return state.getValue<T>(name);
			}*/
			
			/*template<typename T>
			std::shared_ptr<T> getObject(const std::string& name)
			{
				std::shared_ptr<Object> objectPtr = getValue<std::shared_ptr<Object>>(name);
				return (std::dynamic_pointer_cast<T>(objectPtr));
			}*/
			
			/*template<typename T>
			T getValue(T t, const std::string& name)
			{
				T res;
				
			}*/
			
			State getState() const;
			
		private:
			std::string fileName;
			State state;
			Syntax syntax;
	};
}
