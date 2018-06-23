#include "app.hpp"

#include "../utils/utils.hpp"
#include "../huffman-coding/frequency-table.hpp"

#include <thread>

App* App::get_instance(const std::vector<std::string>& arguments, Log& output, Log& log, Log& error_log)
{
	using namespace std;

	App* instance = new App(output, log, error_log);

	instance->_help = Utils::OptionUtils::has_option(get_option(HELP).short_option, arguments);
	instance->_quiet = Utils::OptionUtils::has_option(get_option(QUIET).short_option, arguments);
	instance->_print = Utils::OptionUtils::has_option(get_option(PRINT).short_option, arguments);

	if (instance->_quiet) {
		instance->_error_log.set_quiet(instance->_quiet);
		instance->_output.set_quiet(instance->_quiet);
		instance->_log.set_quiet(instance->_quiet);
	}

	instance->_threads_specified = Utils::OptionUtils::has_option(get_option(THREADS).short_option, arguments);
	if (instance->_threads_specified) {
		string threads_string = Utils::OptionUtils::get_option_value(get_option(THREADS).short_option, arguments);
		try {
			instance->_threads = stoi(threads_string);
		}
		catch (const exception& e) {
			instance->_error_log << e.what() << "\n";

			return instance = nullptr;
		}
	}
	else {
		instance->_log << "Threads count not specified!" << "\n";
		instance->_log << "Setting threads count to maximum threads available!" << "\n";

		instance->_threads = thread::hardware_concurrency();
		instance->_log << "Threads count: " << instance->_threads << "\n";
	}

	bool file_specified = Utils::OptionUtils::has_option(get_option(FILE).short_option, arguments);
	if (file_specified) {
		string file_path = Utils::OptionUtils::get_option_value(get_option(FILE).short_option, arguments);
		bool can_open = Utils::FileUtils::can_open(file_path);
		if (can_open) {
			instance->_file = file_path;
		}
		else {
			instance->_error_log << "File " << file_path << " can not be opened for reading!" << "\n";

			return instance = nullptr;
		}
	}
	else if (!instance->_help) {
		instance->_error_log << "File not specified " << "\n";

		return instance = nullptr;
	}

	assert(instance);

	if (!App::is_valid(instance)) {
		instance->_error_log << "Application could not be started with specified data!" << "\n";	
		
		instance = nullptr;
	}

	return instance;
}

void App::execute() const
{
	using namespace std;

	assert(App::is_valid(this));

	if (_help) {
		print_help(_output);
	}
	else {
		using namespace FrequencyTable;
		FrequencyTableType ft;

		unsigned int threads_count = are_threads_specified() ? _threads : std::thread::hardware_concurrency();
		try {
			ft = FrequencyTable::get_frequency_table(_file, threads_count, _log);
		}
		catch (const exception& e) {
			_error_log << e.what() << "\n";
		}

		if (_print)
			FrequencyTable::print_frequency_table(_output, ft);
	}
}

void App::print_help(Log& out)
{
	using namespace std;

	out << "Frequency Table" << "\n"
		<< "\n";
	out << "Commands available:" << "\n";

	out << get_option(FILE).short_option << "(" << get_option(FILE).long_option << "): " << get_option(FILE).description << "\n";
	out << get_option(THREADS).short_option << "(" << get_option(THREADS).long_option << "): " << get_option(THREADS).description << "\n";
	out << get_option(PRINT).short_option << "(" << get_option(PRINT).long_option << "): " << get_option(PRINT).description << "\n";
	out << get_option(QUIET).short_option << "(" << get_option(QUIET).long_option << "): " << get_option(QUIET).description << "\n";
	out << get_option(HELP).short_option << "(" << get_option(HELP).long_option << "): " << get_option(HELP).description << "\n";
}

bool App::is_valid(const App* instance)
{
	bool file = instance->is_valid_file_path();
	bool threads = instance->are_threads_specified();
	bool print = instance->_print;
	bool quiet = instance->_quiet;
	bool help = instance->_help;

	bool valid = true;
	if (help && (file || threads || print))
		valid = false;
	else if (!help && !file)
		valid = false;
	else if (print && quiet || help && quiet)
		valid = false;

	return valid;
}

App::Option App::get_option(OptionID id)
{
	static const std::vector<Option> options = {
		{ "-f", "--file",    "path to a file // REQUIRED" },
		{ "-t", "--threads", "number of threads to start // if not specified - maximal non hardware concurent" },
		{ "-p", "--print",   "prints the frequency table" },
		{ "-q", "--quiet",   "runs with no output" },
		{ "-h", "--help",    "shows information for the program" }
	};

	return options[id];
}

bool App::is_valid_file_path() const
{
	return Utils::FileUtils::can_open(_file);
}
