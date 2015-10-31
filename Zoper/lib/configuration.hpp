#ifndef __LIB_CONFIGURATION_HPP__
#define __LIB_CONFIGURATION_HPP__

#include <string>
#include <map>
#include <fstream>
#include "types.hpp"

namespace lib
{
	class Configuration
	{
	public:
		Configuration(const std::string &fName);
		virtual ~Configuration();

		typedef std::map < std::string, std::string > CMap;
		typedef std::pair<std::string, std::string> CMapLine;

		s32 getAsInt(const std::string &section, const std::string &subSection) const;
	private:
		void readAllConfig();
		CMap _data;
		std::string _fileName;
	};
}

#endif
