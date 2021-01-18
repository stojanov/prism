#include "FileIO.h"

#include <fstream>
#include <sstream>
#include "prism/System/System.h"

namespace Prism::System
{
	std::string ReadFile(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::in | std::ios::binary);

		PR_ASSERT(file.is_open(), "Error opening file " + filepath); // temp assertion

		std::ostringstream stream;
		stream << file.rdbuf();
		return stream.str();
	}
	
	std::future<std::string> AsyncReadFile(const std::string& filepath)
	{
		return std::async(std::launch::async, [&filepath]() { return ReadFile(filepath); });
	}
}
