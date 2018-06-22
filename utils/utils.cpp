#include "utils.hpp"

#include <fstream>
#include <stdexcept>

namespace Utils
{
	namespace Files
	{
		std::streampos get_file_size(const std::string& file_path)
		{
			std::streampos file_size = -1;

			std::ifstream file(file_path, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
			if (file.is_open()) {
				file_size = file.tellg();
				file.close();
			}

			return file_size;
		}
		bool can_open(const std::string& file_path)
		{
			std::ifstream file(file_path, std::ifstream::in | std::ifstream::binary);
			bool opened = file.is_open();

			file.close();
			return opened;
		}
	} // !Files

	namespace Options
	{
		bool has_option(const std::string& option, const std::vector<std::string>& arguments)
		{
			return std::find(arguments.begin(), arguments.end(), option) != arguments.end();
		}
		std::string get_option_value(const std::string& option, const std::vector<std::string>& arguments)
		{
			auto option_position = std::find(arguments.begin(), arguments.end(), option);

			if (option_position != arguments.end()) {
				return *(++option_position);
			}

			throw std::invalid_argument(option + " -- Option not found!");
		}
	} // !Options

	namespace Time
	{
		Time now()
		{
			return std::chrono::high_resolution_clock::now();
		}

		Duration since(const Time& start)
		{
			Time finish = now();
			return finish - start;
		}
	} // !Times
} // !Utils