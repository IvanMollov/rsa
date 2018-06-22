#pragma once

#include "../utils/log.hpp"

#include <string>
#include <vector>

#include <stdexcept>


class App
{
private:
	struct Option
	{
		std::string short_option;
		std::string long_option;
		std::string description;
	};

	enum OptionID
	{
		FILE,
		THREADS,
		PRINT,
		QUIET,
		HELP,
		_COUNT_
	};

private:
	// get data for option
	static Option get_option(OptionID id);
	static void print_help(Log& out);

public:
	static bool is_valid(const App* instance);
	static App* get_instance(const std::vector<std::string>& arguments, Log& output, Log& log, Log& error_log);

private:
	App() = delete;
	App(Log& output, Log& log, Log& error_log) :
		_threads_specified(false), _threads(0), _print(false), _quiet(false), _help(false), _file(""),
		_output(output), _log(log), _error_log(error_log)
	{}
	App(const App&) = delete;
	App(App&&) = delete;
	~App() = default;

	App& operator=(const App&) = delete;
	App& operator=(App&&) = delete;

public:
	void execute() const;

	unsigned int get_threads_count() const
	{ return _threads; }

	std::string get_file_path() const
	{ return _file; }

private:
	bool are_threads_specified() const
	{ return _threads_specified; }

	bool is_valid_file_path() const;

private:
	Log& _output;
	Log& _log;
	Log& _error_log;

	bool _help;
	bool _quiet;
	bool _print;
	unsigned int _threads;
	std::string _file;

	bool _threads_specified;
};