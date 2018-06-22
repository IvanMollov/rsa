#pragma once

#include <chrono>

#include <string>
#include <vector>

namespace Utils
{
	namespace Files
	{
		std::streampos get_file_size(const std::string& file_path);
		bool can_open(const std::string& file_path);
	} // !Files

	namespace Options
	{
		bool has_option(const std::string& option, const std::vector<std::string>& arguments);
		std::string get_option_value(const std::string& option, const std::vector<std::string>& arguments);
	} // !Options

	namespace Time
	{
		using Time = std::chrono::time_point<std::chrono::steady_clock>;
		using Duration = std::chrono::duration<double>;

		Time now();
		Duration since(const Time& start);
	} // !Times
} // !Utils