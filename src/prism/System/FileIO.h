#pragma once

#include <string>
#include <future>

namespace Prism::System
{
	std::string ReadFile(const std::string& filepath);
	std::future<std::string> AsyncReadFile(const std::string& filepath);
}
