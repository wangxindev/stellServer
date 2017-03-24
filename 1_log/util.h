#pragma once

namespace logUtil
{

#include <string>
#include <sstream>


	class util
	{
	public:
		util() {};
		~util() {};
		template  <class T>
		static std::string toString(T type);
	};

	template<class T>
	inline std::string util::toString(T type)
	{
		std::string str;
		std::stringstream strStream;
		strStream << type;
		strStream >> str;
		return str;
	}

#define toStringByDefine( s ) # s
#define toStringByClassUtil(s) (util::toString(s))
}
