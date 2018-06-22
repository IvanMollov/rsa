#pragma once

#include <chrono>

#include <string>
#include <vector>

namespace Utils
{
	namespace FileUtils
	{
		std::streampos get_file_size(const std::string& file_path);
		bool can_open(const std::string& file_path);
	} // !FileUtils

	namespace OptionUtils
	{
		bool has_option(const std::string& option, const std::vector<std::string>& arguments);
		std::string get_option_value(const std::string& option, const std::vector<std::string>& arguments);
	} // !OptionUtils

	namespace TimeUtils
	{
		using Time = std::chrono::high_resolution_clock::time_point;
		using Duration = std::chrono::duration<double>;

		Time now();
		Duration since(const Time& start);
	} // !Times
} // !Utils